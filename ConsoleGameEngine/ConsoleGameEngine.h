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
//#define DEBUG

#ifdef DEBUG
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


#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>
#include <sstream>
#include <condition_variable>
#include <chrono>
#include <vector>

#define ESC L'\x1b'
#define CSI L"\x1b["
#define wchar wchar_t

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
private:
    SHORT last_state = 0;
    int virtual_key;
    bool got_pressed = false;
    bool is_down = false;

    CGEKey(int vitual_key) :
        virtual_key(vitual_key) {}
public:
    bool isDown() {
        return is_down;
    }
    /*returns true once pressed, does not reset*/
    bool gotPressed() {
        if (got_pressed)
        {
            got_pressed = false;
            return true;
        }
        return false;
    }
};

class CGEKeyRegistry {
    friend class ConsoleGameEngine;
private:
    std::vector<CGEKey*> key_register;

    CGEKeyRegistry(){}
    ~CGEKeyRegistry(){}

    void update() {
        for (CGEKey* key : key_register) {
            SHORT state = GetKeyState(key->virtual_key);
            if (state < 0) {
                key->is_down = true;
                if (key->last_state >= 0) {
                    key->got_pressed = true;
                }
            } 
            else {
                key->is_down = false;
            }
            key->last_state = state;
        }
    }
    
public:
    CGEKey* registerKey(int virtual_key) {
        CGEKey* key = new CGEKey(virtual_key);
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
        wchar c = ' ';
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
    void setChar(int x, int y, wchar c) {
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
    std::wstring getOutput() {
        std::wostringstream stream;
        int last_color_bg = -1;
        int last_color_fg = -1;
        int current_x = -1;
        int current_y = -1;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (getPassiveBuffer().get(x, y).changed) {
                    if (!(current_x == x && current_y == y)) {
                        stream << CSI << (y + 1)  << L';' << (x + 1) << 'H';
                    }
                    if (getPassiveBuffer().get(x, y).backgroud_color != last_color_bg) {
                        stream << CSI << getPassiveBuffer().get(x, y).backgroud_color << L'm';
                        last_color_bg = getPassiveBuffer().get(x, y).backgroud_color;
                    }
                    if (getPassiveBuffer().get(x, y).foreground_color != last_color_fg) {
                        stream << CSI << getPassiveBuffer().get(x, y).foreground_color << L'm';
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
    std::wostringstream out;
    std::wostringstream render_out;
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

    void hideCurser() {
        std::wcout << CSI L"?25l";
    }

    void showCurser() {
        std::wcout << CSI L"?25h";
    }

    void enableCursorBlinking() {
        std::wcout << CSI L"?12h";
    }

    void disableCursorBlinking() {
        std::wcout << CSI L"?12l";
    }

    void useAlternativeScreenBuffer() {
        std::wcout << CSI L"?1049l";
    }

    void useMainScreenBuffer() {
        std::wcout << CSI L"?1049l";
    }

    void setCurser(int x, int y) {
        std::wcout << CSI << y << L";" << x << L"H";
    }

    void deleteChar(int n) {
        std::wcout << CSI << n << L"P";
    }

    CGEMap* init(int width, int height) {
        enableTVMode();
        useAlternativeScreenBuffer();
        hideCurser();

        map = new CGEMap(width, height);

        return map;
    }

    void changeTextFormat(const wchar* format) {
        std::wcout << CSI << format << L'm';
    }

    void oChangeTextFormat(const wchar* format) {
        std::wcout << CSI << format << L'm';
    }

private:
    void render(std::wstring output) {
        if (output != L"") std::wcout << output;
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

            std::wstring output;
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
