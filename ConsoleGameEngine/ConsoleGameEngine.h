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

void DebugOut(const std::string out) {
    std::ofstream file_out;
    file_out.open("./debug.txt");
    file_out << out;
    file_out.close();
}

void DoDebug(bool b) {
    Debug_Out = b;
}
#endif
/*------------------------------- DEBUG END -------------------------------*/

#ifdef WIN32
#define WINDOWS
#endif

#ifdef __WIN32
#ifndef WINDOWS
#define WINDOWS
#endif
#endif

#ifdef __WIN32__
#ifndef WINDOWS
#define WINDOWS
#endif
#endif

#ifdef __linux__
#define LINUX
#endif

#ifdef WINDOWS
#include <Windows.h>

#define _CGEKeyRegistry CGEKeyRegistryWindows
#define cgechar wchar_t
#define cgestring std::wstring
#define cgeostringstream std::wostringstream
#define cgecout std::wcout
#define cgecin std::wcin

#define _FBLOCK L' '
#define EMPTY_STRING L""

#define ESC L'\x1b'
#define CSI L"\x1b["

#define VTS_SEPERATOR L';'
#define VTS_HIDE_CURSER CSI L"?25l"
#define VTS_SHOW_CURSER CSI L"?25h"
#define VTS_ENABLE_CURSER_BLINKING CSI L"?12h"
#define VTS_DISABLE_CURSER_BLINKING CSI L"?12l"
#define VTS_USE_ALTERNATE_SCREEN_BUFFER CSI L"?1049l"
#define VTS_USE_MAIN_SCREEN_BUFFER CSI L"?1049l"
#define VTS_SET_CURSOR_END L'H'
#define VTS_DELETE_CHAR_END L'P'
#define VTS_CHANGE_TEXT_FORMAT_END L'm'
#define VTS_CHANGE_BACKGROUND_COLOR CSI L"48;2;"
#endif

#ifdef LINUX
/* Dependency: sudo apt install libx11-dev */
#include <X11/Xlib.h>

#define _CGEKeyRegistry CGEKeyRegistryLinux
#define cgechar char
#define cgestring std::string
#define cgeostringstream std::ostringstream
#define cgecout std::cout
#define cgecin std::cin

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
#define VTS_CHANGE_BACKGROUND_COLOR CSI "48;2;"

/* x11 Display to get input from console window */
Display* d;
/* x11 Autorepeat: true if usually enabled */
bool autoreapeat = false;
#endif

#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <map>
#include <utility>

/* text formats */
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

#ifdef LINUX
/*
* get ID from terminal window and enable XEvents for KeyPress, KeyRelease and FocusChange
* to be able to update CGEKeyRegistry
*/
static bool SelectXInput() {
    //TODO
    d = XOpenDisplay(NULL);
    system("env | grep \"WINDOW *ID *=\"");
    Window w;
    std::cin >> w;
    XSelectInput(d, w, KeyPressMask | KeyReleaseMask | FocusChangeMask);
    return true;
}

static bool IsAutorepeatEnabled() {
    //TODO
    system("xset q | \"auto *repeat: *on\""); 
    return true; 
} 

/* Sets x11 autorepeat */
static void SetAutorepeat(bool ar) {
    if (ar) {
        system("xset r on");
    }
    else {
        system("xset r off");
    }
}
#endif

class CGEKey {
    friend class CGEKeyRegistry;
    friend class CGEKeyRegistryLinux;
    friend class CGEKeyRegistryWindows;
protected:
    bool is_down = false;
    bool got_pressed = false;
    bool got_released = false;

    CGEKey() { }
    ~CGEKey() { }

    void update(bool value) {
        if(is_down != value) {
            if(value) {
                got_pressed = true;
                is_down = true;
            }
            else {
                got_released = true;
                is_down = false;
            }
        }
    }
public:
    bool isDown() {
        return is_down;
    }
    /*
    * Returns if key got pressed since last call. 
    * If it got pressed resets value to false
    */
    bool gotPressed() {
        if (got_pressed) {
            got_pressed = false;
            return true;
        }
        return false;
    }
    /*
    * Returns if key got released since last call. 
    * If it got released resets value to false
    */
    bool gotReleased() {
        if(got_released) {
            got_released = false;
            return true;
        }
        return false;
    }
};

class CGEKeyRegistry {
    friend class ConsoleGameEngine;
protected:
    std::map<int, CGEKey*> key_register;

    CGEKeyRegistry() { }
    ~CGEKeyRegistry() {
        for(std::pair<int, CGEKey*> pair : key_register) {
            delete pair.second;
        }
     }

    virtual void update() = 0;
    
public:
    CGEKey* registerKey(int virtual_key) {
        key_register[virtual_key] = new CGEKey();
        return key_register[virtual_key];
    };
};

#ifdef WINDOWS
class CGEKeyRegistryWindows : public CGEKeyRegistry {
    friend class ConsoleGameEngine;
private:
    void update() override {

    }
    CGEKeyRegistryWindows() :
        CGEKeyRegistry() { }
    ~CGEKeyRegistryWindows() { }
};
#endif

#ifdef LINUX 
class CGEKeyRegistryLinux : public CGEKeyRegistry {
    friend class ConsoleGameEngine;
private:
    CGEKeyRegistryLinux() :
        CGEKeyRegistry() { }
    ~CGEKeyRegistryLinux() { }

    void update() override {
        int virtual_key;
        XEvent event;
        CGEKey* key;
        while (XCheckMaskEvent(d, KeyPressMask | KeyReleaseMask | FocusChangeMask, &event)) {
            virtual_key = XLookupKeysym(&event.xkey, 0);
            switch (event.type) {
                case KeyPress: {
                    if (key_register.contains(virtual_key)) key_register[virtual_key]->update(true);
                    break;
                }
                case KeyRelease: {
                    if (key_register.contains(virtual_key)) key_register[virtual_key]->update(false);
                    break;
                }
                case FocusIn: {
                    if (autoreapeat) SetAutorepeat(false);
                    break;
                }
                case FocusOut: {
                    if (autoreapeat) SetAutorepeat(true);
                    break;
                }
            }
        }
    }
};
#endif

class cgetostringstream : public cgeostringstream {
public:
    /* starts with 0;0 */
    void setCursor(int x, int y) {
        *this << CSI << y + 1 << VTS_SEPERATOR << x + 1 << VTS_SET_CURSOR_END;
    }
    void changeTextFormat(int format) {
        *this << CSI << format << VTS_CHANGE_TEXT_FORMAT_END;
    }

    void setBackgroundColor(int red, int green, int blue) {
        *this << VTS_CHANGE_BACKGROUND_COLOR << red << VTS_SEPERATOR << green << VTS_SEPERATOR << blue << VTS_CHANGE_TEXT_FORMAT_END;
    }
};

class GraphicsBakery {
public:
    virtual cgestring getOutput() = 0;
    virtual void switchBuffer() = 0;
};

template <class U>
class CGEBuffer {
    U* buffer = nullptr;
    int width;

public:
    CGEBuffer(int width, int height) :
        width(width) {
        buffer = new U[width * height];
    }
    ~CGEBuffer() {
        delete[] buffer;
    }

    U& get(int x, int y) {
        return buffer[y * width + x];
    }
};

template <class U>
class CGEMap : public GraphicsBakery {
    friend class ConsoleGameEngine;
protected:
    int width;
    int height;

    bool flag_buffer = true;
    CGEBuffer<U> buffer_0;
    CGEBuffer<U> buffer_1;
    CGEBuffer<U> buffer_2;

    CGEMap(int width, int height) :
        width(width),
        height(height),
        buffer_0(width, height),
        buffer_1(width, height),
        buffer_2(width, height) { }

    ~CGEMap() { }

    void switchBuffer() override {
        flag_buffer = !flag_buffer;
    }

    CGEBuffer<U>& getMainBuffer() {
        return buffer_0;
    }

    CGEBuffer<U>& getActiveBuffer() {
        return (flag_buffer ? buffer_1 : buffer_2);
    }

    CGEBuffer<U>& getPassiveBuffer() {
        return (!flag_buffer ? buffer_1 : buffer_2);
    };
};

class CGEChar {
public:
    bool changed = true;
    int foreground_color = TF_COLOR_FOREGROUND_DEFAULT;
    int backgroud_color = TF_COLOR_BACKGROUND_DEFAULT;
    cgechar c = _FBLOCK;
};

class CGECharMap : public CGEMap<CGEChar> {
    friend class ConsoleGameEngine;
private:
    CGECharMap(int width, int height) : CGEMap<CGEChar>(width, height) { }
    ~CGECharMap() { }

    cgestring getOutput() override {
        cgetostringstream stream;
        int last_color_bg = -1;
        int last_color_fg = -1;
        int current_x = -1;
        int current_y = -1;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (getPassiveBuffer().get(x, y).changed) {
                    if (!(current_x == x && current_y == y)) {
                        stream.setCursor(x, y);
                    }
                    if (getPassiveBuffer().get(x, y).backgroud_color != last_color_bg) {
                        stream.changeTextFormat(getPassiveBuffer().get(x, y).backgroud_color);
                        last_color_bg = getPassiveBuffer().get(x, y).backgroud_color;
                    }
                    if (getPassiveBuffer().get(x, y).foreground_color != last_color_fg) {
                        stream.changeTextFormat(getPassiveBuffer().get(x, y).foreground_color);
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
public:
    void setChar(int x, int y, cgechar c) {
        getMainBuffer().get(x, y).c = c;
        getActiveBuffer().get(x, y).c = c;
        getActiveBuffer().get(x, y).changed = true;

    }

    void setForegroundColor(int x, int y, int foregroud_color) {
        getMainBuffer().get(x, y).foreground_color = foregroud_color;
        getActiveBuffer().get(x, y).foreground_color = foregroud_color;
        getActiveBuffer().get(x, y).changed = true;
    }

    void setBackgroundColor(int x, int y, int backgroud_color) {
        getMainBuffer().get(x, y).backgroud_color = backgroud_color;
        getActiveBuffer().get(x, y).backgroud_color = backgroud_color;
        getActiveBuffer().get(x, y).changed = true;
    }

    int getChar(int x, int y) {
        return getMainBuffer().get(x, y).c;
    }

    int getForegroundColor(int x, int y) {
        return getMainBuffer().get(x, y).foreground_color;
    }

    int getBackgroundColor(int x, int y) {
        return getMainBuffer().get(x, y).backgroud_color;
    }
};

class CGEPixel {
public:
    CGEPixel() { }
    CGEPixel(unsigned char red, unsigned char green, unsigned char blue) :
    red(red), 
    green(green), 
    blue(blue) { }
    ~CGEPixel() { }
private:
    bool changed = true;
    bool transparent = false;
    unsigned char red = 0;
    unsigned char  green = 0;
    unsigned char  blue = 0;
public:
    bool gotChanged() {
        return changed;
    }

    void setChanged(bool changed) {
        this->changed = changed;
    }

    bool isTransparent() {
        return transparent;
    }

    void setTransparent(bool transparent) {
        this->transparent = transparent;
    }

    void setColor(unsigned char red, unsigned char green, unsigned char blue) {
        this->red = red;
        this->green = green;
        this->blue = blue;
        changed = true;
    }

    unsigned char getRed() {
        return red;
    }

    unsigned char getGreen() {
        return green;
    }

    unsigned char getBlue() {
        return blue;
    }
 
    bool operator==(CGEPixel& other_pixel) {
        return red == other_pixel.red && green == other_pixel.green && blue == other_pixel.blue;
    }

    bool operator!=(CGEPixel& other_pixel) {
        return !(*this == other_pixel);
    }
};

class CGETexture {
private:
    CGEBuffer<CGEPixel> buffer;
    bool changed = true;
    bool visible = true;
    int priority = 0;
    int width;
    int height;
    int x = 0;
    int y = 0;
public:
    CGETexture(int width, int height) :
        width(width),
        height(height), 
        buffer(width, height) { }

    void setPosition(int x, int y) {
       this->x = x;
       this->y = y;
       changed = true;
    }

    void setPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue) {
        buffer.get(x, y).setColor(red, green, blue);
        changed = true;
    }

    void setVisible(bool visible) {
        this->visible = visible;
        changed = true;
    }

    void setPriority(int priority) {
        this->priority = priority;
    }

    int getPriority() {
        return priority;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }
};

class CGEPixelMap : public CGEMap<CGEPixel> {
    friend class ConsoleGameEngine;
private:
    int pixel_width;
    int pixel_height;
    std::vector<CGETexture> textures;
    std::map<int, CGEBuffer<CGEPixel>> priority_buffers;

    CGEPixelMap(int width, int height, int pixel_width, int pixel_height) : 
    CGEMap(width, height), 
    pixel_width(pixel_width), 
    pixel_height(pixel_height) {  }

    ~CGEPixelMap() { }

    cgestring getOutput() override {
        cgetostringstream stream;
        CGEPixel last_drawn_pixel;
        int current_x = -1;
        int current_y = -1;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (getPassiveBuffer().get(x, y).gotChanged()) {
                    if (!(current_x == x && current_y == y)) {
                        stream.setCursor(x * pixel_width, y * pixel_height);
                        current_y = y;
                        current_x = x;
                    }
                    if (last_drawn_pixel != getPassiveBuffer().get(x, y)) {
                        stream.setBackgroundColor(getPassiveBuffer().get(x, y).getRed(), getPassiveBuffer().get(x, y).getGreen(), getPassiveBuffer().get(x, y).getBlue());
                    }
                    for (int j = 0; j < pixel_width; j++) {
                        stream << _FBLOCK;
                        current_x++;
                    }
                    for (int i = 1; i < pixel_height; i++) {
                        stream.setCursor(x * pixel_width, y * pixel_height + i);
                        for (int j = 0; j < pixel_width; j++) {
                            stream << _FBLOCK;
                        }
                        current_y++;
                    }
                    getPassiveBuffer().get(x, y).setChanged(false);
                }
            }
        }
        return stream.str();
    }
public:
    void setPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue) {
        getMainBuffer().get(x, y).setColor(red, green, blue);
        getActiveBuffer().get(x, y).setColor(red, green, blue);
    }

    void handleTexture(CGETexture texture) {

    }
};

class ConsoleGameEngine {
public:
    CGEKeyRegistry* key_registry;
private:
    std::wostringstream out;
    std::wostringstream render_out;
    GraphicsBakery* map = nullptr;

    void (*game_loop) (double);
    bool run = true;
public:
    ConsoleGameEngine(void (*game_loop)(double)) :
        game_loop(game_loop), 
        key_registry(new _CGEKeyRegistry()) { }
    ~ConsoleGameEngine() {
        if (map != nullptr) delete map;
        delete key_registry;
    }
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

    /* stops the program, call in game_loop */
    void stop() {
        run = false;
    }

    void setFontSize(int width, int height) {
#ifdef WINDOWS
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

        PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
        lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
        GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
        lpConsoleCurrentFontEx->dwFontSize.X = width;
        lpConsoleCurrentFontEx->dwFontSize.Y = height;
        SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
#endif
#ifdef LINUX 
        //TODO
#endif
    }

    void hideCurser() {
        cgecout << VTS_HIDE_CURSER;
    }

    void showCurser() {
        cgecout << VTS_SHOW_CURSER;
    }

    void enableCursorBlinking() {
        cgecout << VTS_ENABLE_CURSER_BLINKING;
    }

    void disableCursorBlinking() {
        cgecout << VTS_DISABLE_CURSER_BLINKING;
    }

    void useAlternativeScreenBuffer() {
        cgecout << CSI VTS_USE_ALTERNATE_SCREEN_BUFFER;
    }

    void useMainScreenBuffer() {
        cgecout << CSI VTS_USE_MAIN_SCREEN_BUFFER;
    }

    /* sets cusor to position <x>:<y> beginning with 0:0 */
    void setCurser(int x, int y) {
        cgecout << CSI << y + 1 << VTS_SEPERATOR << x + 1 << VTS_SET_CURSOR_END;
    }

    void deleteChar(int n) {
        cgecout << CSI << n << VTS_DELETE_CHAR_END;
    }

    void changeTextFormat(const cgechar* format) {
        cgecout << CSI << format << VTS_CHANGE_TEXT_FORMAT_END;
    }


    CGECharMap* init_CharMap(int width, int height) {
        preinit();

        if (map != nullptr) {
            delete map;
        }
        map = new CGECharMap(width, height);
        return (CGECharMap*)map;
    }

    /* pixel_width and pixel_height in amount of characters in console */
    CGEPixelMap* init_PixelMap(int width, int height, int pixel_width, int pixel_height) {
        preinit();

        if (map != nullptr) {
            delete map;
        }
        map = new CGEPixelMap(width, height, pixel_width, pixel_height);
        return (CGEPixelMap*)map;
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

#ifdef LINUX
    
#endif

private:
    void preinit() {
#ifdef WINDOWS
        enableTVMode();
#endif
#ifdef LINUX
        SelectXInput();

        if (IsAutorepeatEnabled()) {
            autoreapeat = true;
            //TODO check
            SetAutorepeat(false);
        }

        /* prevent user input in terminal */
        system("stty -echo");
#endif
        useAlternativeScreenBuffer();
        hideCurser();
    }

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

            key_registry->update();
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed_time = end - start;
            game_loop(elapsed_time.count());
            start = std::chrono::steady_clock::now();
            
            

            if (buffer_flag) {
                map->switchBuffer();

                mutex_graphics_1.lock();
                continue_graphics_1 = true;
                mutex_graphics_1.unlock();
                cv_graphics_1.notify_all();
                continue_mech_1 = false;
            }
            else {
                map->switchBuffer();

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
