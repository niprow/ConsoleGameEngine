/*
* Copyright (C) 2021  Nils Prommersberger
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

/*------------------------------- DEBUG -------------------------------*/
//#define DEBUG_WINDOWS

#ifdef DEBUG_WINDOWS
#include <fstream>

bool Debug_Out = false;

void DebugOut(const std::wstring out) {
    std::wofstream file_out;
    file_out.open(L"./debug.txt");
    file_out << out;
    file_out.close();
}

void DebugOut(bool b) {
    Debug_Out = b;
}
#endif
/*------------------------------- DEBUG END -------------------------------*/
#ifdef WINDOWS
#include <Windows.h>

typedef wchar_t cgechar;

#define ESC L'\x1b'
#define CSI L"\x1b["
#endif

#ifdef __linux__
#define _CGEKey CGEKeyLinux
#define cgechar char
#define cgestring std::string
#define cgeostringstream std::ostringstream
#define cgecout std::cout

#define _FBLOCK ' '
#define EMPTY_STRING ""

#define ESC '\x1b'
#define CSI "\x1b["

#define VTS_SEPERATOR ';'
#define VTS_HIDE_CURSER CSI "?25l"
#define VTS_SHOW_CURSER CSI "?25h"
#define VTS_ENABLE_CURSER_BLINKING CSI "?12h"
#define VTS_DISABLE_CURSER_BLINKING CSI "?12l"
#define VTS_USE_ALTERNATE_SCREEN_BUFFER CSI "?1049l"
#define VTS_USE_MAIN_SCREEN_BUFFER CSI "?1049l"
#define VTS_SET_CURSOR_END 'H'
#define VTS_DELETE_CHAR_END 'P'
#define VTS_CHANGE_TEXT_FORMAT_END 'm'

#endif



#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <condition_variable>
#include <chrono>
#include <vector>

/*text formats*/
#define TF_COLOR_FOREGROUND_BLACK      30
#define TF_COLOR_FOREGROUND_RED        31
#define TF_COLOR_FOREGROUND_GREEN      32
#define TF_COLOR_FOREGROUND_YELLOW     33
#define TF_COLOR_FOREGROUND_BLUE       34
#define TF_COLOR_FOREGROUND_MAGENTA    35
#define TF_COLOR_FOREGROUND_CYAN       36
#define TF_COLOR_FOREGROUND_WHITE      37
#define TF_COLOR_FOREGROUND_DEFAULT    39

#define TF_COLOR_BACKGROUND_BLACK      40
#define TF_COLOR_BACKGROUND_RED        41
#define TF_COLOR_BACKGROUND_GREEN      42
#define TF_COLOR_BACKGROUND_YELLOW     43
#define TF_COLOR_BACKGROUND_BLUE       44
#define TF_COLOR_BACKGROUND_MAGENTA    45
#define TF_COLOR_BACKGROUND_CYAN       46
#define TF_COLOR_BACKGROUND_WHITE      47
#define TF_COLOR_BACKGROUND_DEFAULT    49


using namespace std::chrono_literals;

class CGEKey {
    friend class CGEKeyRegistry;
protected:
    int virtual_key;
    bool got_pressed = false;
    bool is_down = false;
    bool got_released = false;

    CGEKey() { } 
    CGEKey(int vitual_key) :
    virtual_key(vitual_key) { }
    ~CGEKey() { }

    virtual void update() = 0;
public:
    bool isDown() {
        return is_down;
    }
    bool gotPressed() {
        return got_pressed;
    }
    bool gotReleased() {
        return got_released;
    }
};

#ifdef WINDOWS
class CGEKeyWindows : public CGEKey {
private:
    SHORT last_state = 0;
protected:
    void update() override {
        SHORT state = GetKeyState(virtual_key);
            if (state < 0) {
                is_down = true;
                got_pressed = last_state >= 0;
            } 
            else {
                is_down = false;
                if(got_pressed) got_pressed = false;
            }
            last_state = state;
    }

    CGEKeyWindows(int virtual_key) :
    virtual_key(virtual_key) { } 
};  
#endif

#ifdef __linux__
class CGEKeyLinux : public CGEKey {
    friend class CGEKeyRegistry;
protected:
    CGEKeyLinux(int virtual_key) :
    CGEKey(virtual_key) { }
    void update() override {
        //TODO
    }
};
#endif

class CGEKeyRegistry {
    friend class ConsoleGameEngine;
private:
    std::vector<CGEKey*> key_register;

    CGEKeyRegistry() { }
    ~CGEKeyRegistry() { }

    void update() {
        for (CGEKey* key : key_register) {
            key->update();
        }
    }
    
public:
    CGEKey* registerKey(int virtual_key) {
        CGEKey* key = new _CGEKey(virtual_key);
        key_register.push_back(key);
        return key;
    }
};

class CGEMap {
    friend class ConsoleGameEngine;
private:
    struct CGEMapChar {
        bool changed = true;

        int foreground_color = TF_COLOR_FOREGROUND_DEFAULT;
        int backgroud_color = TF_COLOR_BACKGROUND_DEFAULT;
        cgechar c = _FBLOCK;
    };

    struct CGEMapBuffer {
        CGEMapChar* buffer;
        int width;

        CGEMapBuffer(int width, int height) :
        width(width) {
            buffer = new CGEMapChar[width * width];
        }
        ~CGEMapBuffer() {
            delete[] buffer;
        }

        CGEMapChar& get(int x, int y) {
            return buffer[y * width + x];
        }
    };

    int width;
    int height;

    bool flag_buffer = true;
    CGEMapBuffer field;
    CGEMapBuffer field_buffer_1;
    CGEMapBuffer field_buffer_2;

    CGEMapBuffer& getActiveBuffer() {
        return (flag_buffer ? field_buffer_1 : field_buffer_2);
    }

    CGEMapBuffer& getPassiveBuffer() {
        return (!flag_buffer ? field_buffer_1 : field_buffer_2);
    }
    
    CGEMap(int width, int height) :
        width(width),
        height(height), 
        field(width, height), 
        field_buffer_1(width, height), 
        field_buffer_2(width, height) { }
    ~CGEMap() { }

    void switchFlag() {
        flag_buffer = !flag_buffer;
    }
public:
    void setChar(int x, int y, cgechar c) {
        field.get(x, y).c = c;
        getActiveBuffer().get(x, y).c = c;
        getActiveBuffer().get(x, y).changed = true;

    }

    void setForegroudColor(int x, int y, int foregroud_color) {
        field.get(x, y).foreground_color = foregroud_color;
        getActiveBuffer().get(x, y).foreground_color = foregroud_color;
        getActiveBuffer().get(x, y).changed = true;
    }

    void setBackgroudColor(int x, int y, int backgroud_color) {
        field.get(x, y).backgroud_color = backgroud_color;
        getActiveBuffer().get(x, y).backgroud_color = backgroud_color;
        getActiveBuffer().get(x, y).changed = true;
    }

private:
    cgestring getOutput() {
        cgeostringstream stream;
        int last_color_bg = -1;
        int last_color_fg = -1;
        int current_x = -1;
        int current_y = -1;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (getPassiveBuffer().get(x, y).changed) {
                    if (!(current_x == x && current_y == y)) {
                        stream << CSI << (y + 1)  << VTS_SEPERATOR << (x + 1) << VTS_SET_CURSOR_END;
                    }
                    if (getPassiveBuffer().get(x, y).backgroud_color != last_color_bg) {
                        stream << CSI << getPassiveBuffer().get(x, y).backgroud_color << VTS_CHANGE_TEXT_FORMAT_END;
                        last_color_bg = getPassiveBuffer().get(x, y).backgroud_color;
                    }
                    if (getPassiveBuffer().get(x, y).foreground_color != last_color_fg) {
                        stream << CSI << getPassiveBuffer().get(x, y).foreground_color << VTS_CHANGE_TEXT_FORMAT_END;
                        last_color_fg = getPassiveBuffer().get(x, y).foreground_color;
                    }
                    stream << getPassiveBuffer().get(x, y).c;

                    getPassiveBuffer().get(x, y).changed = false;
                    current_y = y;
                    current_x = x + 1;
                }
            }
        }
        return stream.str();
    }
};

class ConsoleGameEngine {
public:
    CGEKeyRegistry key_registry;
private:
    cgeostringstream out;
    cgeostringstream render_out;
    CGEMap* map = nullptr;

    void (*game_loop) (double);
    bool run = true;
public:
    ConsoleGameEngine(void (*game_loop)(double)) :
        game_loop(game_loop) { }

    /*starts the program*/
    int start() {
        if (map == nullptr) return -3;

        std::thread thread_mechanics(&ConsoleGameEngine::thread_mechanics, this);
        std::thread thread_graphics(&ConsoleGameEngine::thread_graphics, this);

        thread_mechanics.join();
        thread_graphics.join();

        useMainScreenBuffer();
        showCurser();

        return 0;
    }

    /*stops the program, call in game_loop*/
    void stop() {
        run = false;
    }

    ~ConsoleGameEngine() {
        delete map;
    }

#ifdef WINDOWS
    bool enableTVMode() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) {
            return false;
        }

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) {
            return false;
        }

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hOut, dwMode)) {
            return false;
        }
        return true;
    }
#endif

    void hideCurser() {
        std::wcout << VTS_HIDE_CURSER;
    }

    void showCurser() {
        std::wcout << VTS_SHOW_CURSER;
    }

    void enableCursorBlinking() {
        std::wcout << VTS_ENABLE_CURSER_BLINKING;
    }

    void disableCursorBlinking() {
        std::wcout << VTS_DISABLE_CURSER_BLINKING;
    }

    void useAlternativeScreenBuffer() {
        std::wcout << CSI VTS_USE_ALTERNATE_SCREEN_BUFFER;
    }

    void useMainScreenBuffer() {
        std::wcout << CSI VTS_USE_MAIN_SCREEN_BUFFER;
    }

    void setCurser(int x, int y) {
        std::wcout << CSI << y << VTS_SEPERATOR << x << VTS_SET_CURSOR_END;
    }

    void deleteChar(int n) {
        std::wcout << CSI << n << VTS_DELETE_CHAR_END;
    }

    void changeTextFormat(const cgechar* format) {
        std::wcout << CSI << format << VTS_CHANGE_TEXT_FORMAT_END;
    }

    CGEMap* init(int width, int height) {
        #ifdef WINDOWS
        enableTVMode();
        #endif
        useAlternativeScreenBuffer();
        hideCurser();

        map = new CGEMap(width, height);

        return map;
    }

private:
    void render(cgestring output) {
        if (output != EMPTY_STRING) cgecout << output;
    }

    std::condition_variable cv_mech_1;
    std::condition_variable cv_mech_2;
    std::condition_variable cv_graphics_1;
    std::condition_variable cv_graphics_2;

    std::mutex mutex_mech_1;
    std::mutex mutex_mech_2;
    std::mutex mutex_graphics_1;
    std::mutex mutex_graphics_2;

    bool buffer_flag = true;
    bool buffer_flag_graphics = true;

    bool continue_mech_1 = true;
    bool continue_mech_2 = true;
    bool continue_graphics_1 = false;
    bool continue_graphics_2 = false;

    void thread_mechanics() {
        auto start = std::chrono::steady_clock::now();
        while (true) {
            std::unique_lock<std::mutex> lock(buffer_flag ? mutex_mech_1 : mutex_mech_2);
            while (!(buffer_flag ? continue_mech_1 : continue_mech_2)) (buffer_flag ? cv_mech_1 : cv_mech_2).wait(lock);

            key_registry.update();
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_time = end - start;
            game_loop(elapsed_time.count());
            start = std::chrono::steady_clock::now();
            
            

            if (buffer_flag) {
                map->switchFlag();

                mutex_graphics_1.lock();
                continue_graphics_1 = true;
                mutex_graphics_1.unlock();
                cv_graphics_1.notify_all();
                continue_mech_1 = false;
            }
            else {
                map->switchFlag();

                mutex_graphics_2.lock();
                continue_graphics_2 = true;
                mutex_graphics_2.unlock();
                cv_graphics_2.notify_all();
                continue_mech_2 = false;
            }
            buffer_flag = !buffer_flag;
        }
    }

    void thread_graphics() {
        while (true) {
            std::unique_lock<std::mutex> lock(buffer_flag_graphics ? mutex_graphics_1 : mutex_graphics_2);
            while (!(buffer_flag_graphics ? continue_graphics_1 : continue_graphics_2)) (buffer_flag_graphics ? cv_graphics_1 : cv_graphics_2).wait(lock);

            cgestring output;
            if (buffer_flag_graphics) {
                output = map->getOutput();

                mutex_mech_1.lock();
                continue_mech_1 = true;
                mutex_mech_1.unlock();
                cv_mech_1.notify_all();
                continue_graphics_1 = false;
            }
            else {
                output = map->getOutput();

                mutex_mech_2.lock();
                continue_mech_2 = true;
                mutex_mech_2.unlock();
                cv_mech_2.notify_all();
                continue_graphics_2 = false;
            }

            render(output);
            buffer_flag_graphics = !buffer_flag_graphics;
        }
    }
};
