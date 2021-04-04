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

#ifndef UNICODE
#error "unicode is needed"
#endif

#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>
#include <sstream>
#include <condition_variable>
#include <chrono>
#include <vector>

#define ESC L"\x1b"
#define CSI L"\x1b["

#define wchar wchar_t

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
    ConsoleGameEngine* cge;
    int width;
    int height;
    wchar* map;
    CGEMap(int width, int height, ConsoleGameEngine* cge) :
        cge(cge),
        width(width),
        height(height)
    {
        map = new wchar[(width + 1) * height]{ L' ' };
        for (int i = 0; i < height; i++) {
            map[width + width * i] = L'\n';
        }
    }

    ~CGEMap() {
        delete[] map;
    }
public:
    wchar getChar(int x, int y) {
        return map[(width + 1) * y + x];
    }

    void setChar(int x, int y, wchar c) {
        map[(width + 1) * y + x] = c;
    }
};

class ConsoleGameEngine {
public:
    CGEKeyRegistry key_registry;
private:
    std::wstring current_old = L"";

    std::wostringstream out;
    std::wostringstream render_out;
    CGEMap* map = nullptr;
    bool map_enabled = false;

    void (*game_loop) (double);
    bool run = true;
public:
    ConsoleGameEngine(void (*game_loop)(double)) :
        game_loop(game_loop) { }

    void start() {
        /*init*/
        enableTVMode();
        useAlternativeScreenBuffer();
        hideCurser();

        std::thread thread_mechanics(&ConsoleGameEngine::thread_mechanics, this);
        std::thread thread_graphics(&ConsoleGameEngine::thread_graphics, this);

        thread_mechanics.join();
        thread_graphics.join();

        useMainScreenBuffer();
        showCurser();
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

    /* get the outstream to set output thtat is drawn manually */
    std::wostringstream* const getOut() {
        return &out;
    }

    void resetOut() {
        out.str(L"");
        out.clear();
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
        std::wcout << CSI << y << ";" << x << "H";
    }

    void deleteChar(int n) {
        std::wcout << CSI << n << "P";
    }

    CGEMap* enableMap(int width, int height) {
        map_enabled = true;
        map = new CGEMap(width, height, this);
        return map;
    }

    void disableMap() {
        map_enabled = false;
    }

private:
    /*
    * finish drawing in out
    * calculate "Graphics"
    */
    void finish(std::wstring current_new) {
        if (current_old == L"") {
            render_out << current_new;
        }
        else {
            int x = 1;
            int y = 1;
            int counter_current_new = 0;
            int counter_current_old = 0;
            int length_current_new = wcslen(current_new.c_str());
            int length_current_old = wcslen(current_old.c_str());
            bool flag_old = true;
            bool last_drawn = false;
            for (; counter_current_new < length_current_new && flag_old; counter_current_new++) {
                if (counter_current_old >= length_current_old) {
                    flag_old = false;
                    break;
                }
                if (current_new[counter_current_new] == current_old[counter_current_old]) {
                    last_drawn = false;
                }
                else {
                    if (!last_drawn) rSetCurser(x, y);
                    if (current_new[counter_current_new] == L'\n') {
                        int delete_character = 0;
                        while (true) {
                            if (counter_current_old >= length_current_old) {
                                flag_old = false;
                                break;
                            }
                            delete_character++;
                            if (current_old[counter_current_old] == L'\n') break;
                            counter_current_old++;
                        }
                        rDeleteChar(delete_character);
                    }
                    else if (current_old[counter_current_old] == L'\n') {
                        while (true) {
                            if (counter_current_new == length_current_new || current_new[counter_current_new] == L'\n') break;
                            render_out << current_new[counter_current_new];
                            counter_current_new++;
                        }
                    }
                    render_out << current_new[counter_current_new];
                    last_drawn = true;
                }
                if (current_new[counter_current_new] == L'\n') {
                    y++;
                    x = 0;
                }
                counter_current_old++;
                x++;
            }
            if (!flag_old && counter_current_new < length_current_new) {
                if (!last_drawn) rSetCurser(x, y);
                render_out << &current_new[counter_current_new];
            }
            else if (counter_current_old < length_current_old) {
                if (!last_drawn) rSetCurser(x, y);
                int delete_character = 0;
                for (; counter_current_old < length_current_old; counter_current_old++) {
                    if (current_old[counter_current_old] == L'\n') break;
                    delete_character++;
                }
                rDeleteChar(delete_character);
                int delete_line = 0;
                for (; counter_current_old < length_current_old; counter_current_old++) {
                    if (current_old[counter_current_old == L'\n']) delete_line++;
                }
                rCursorDown(1);
                rDeleteLine(delete_line);
            }
        }
        current_old = current_new;
    }

    void render() {
        std::wstring output = render_out.str();
        if (output != L"") {
            std::wcout << output;
        }
    }

    void resetRenderOut() {
        render_out.str(L"");
        render_out.clear();
    }

    void rSetCurser(int x, int y) {
        render_out << CSI << y << L";" << x << L"H";
    }

    void rDeleteChar(int n) {
        render_out << CSI << n << L"P";
    }

    void rCursorDown(int n) {
        render_out << CSI << n << L"B";
    }

    void rDeleteLine(int n) {
        render_out << CSI << n << L"M";
    }

    std::condition_variable cv_mech_1;
    std::condition_variable cv_mech_2;
    std::condition_variable cv_graphics_1;
    std::condition_variable cv_graphics_2;

    std::wstring buffer_out_1;
    std::wstring buffer_out_2;

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
            
            if (map_enabled) out << map->map;

            if (buffer_flag) {
                buffer_out_1 = out.str();
                mutex_graphics_1.lock();
                continue_graphics_1 = true;
                mutex_graphics_1.unlock();
                cv_graphics_1.notify_all();
                continue_mech_1 = false;
            }
            else {
                buffer_out_2 = out.str();
                mutex_graphics_2.lock();
                continue_graphics_2 = true;
                mutex_graphics_2.unlock();
                cv_graphics_2.notify_all();
                continue_mech_2 = false;
            }
            resetOut();
            buffer_flag = !buffer_flag;
        }
    }

    void thread_graphics() {
        while (true) {
            std::unique_lock<std::mutex> lock(buffer_flag_graphics ? mutex_graphics_1 : mutex_graphics_2);
            while (!(buffer_flag_graphics ? continue_graphics_1 : continue_graphics_2)) (buffer_flag_graphics ? cv_graphics_1 : cv_graphics_2).wait(lock);

            if (buffer_flag_graphics) {
                finish(buffer_out_1);

                mutex_mech_1.lock();
                continue_mech_1 = true;
                mutex_mech_1.unlock();
                cv_mech_1.notify_all();
                continue_graphics_1 = false;
            }
            else {
                finish(buffer_out_2);

                mutex_mech_2.lock();
                continue_mech_2 = true;
                mutex_mech_2.unlock();
                cv_mech_2.notify_all();
                continue_graphics_2 = false;
            }

            render();
            resetRenderOut();
            buffer_flag_graphics = !buffer_flag_graphics;
        }
    }
};

