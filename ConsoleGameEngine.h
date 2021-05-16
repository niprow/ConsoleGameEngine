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

#define cgechar wchar_t
#define cgestring std::wstring
#define cgeostringstream std::wostringstream
#define cgecout std::wcout
#define cgecin std::wcin
#define cgepopen _popen
#define cgepclose _pclose

#define _FBLOCK L' '
#define EMPTY_STRING L""
#define CGE_YES L"y"
#define CGE_NO L"n"

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

/* Keyboard Virtual Keys: */
#define CGE_KEY_A 'A'
#define CGE_KEY_B 'B'
#define CGE_KEY_C 'C'
#define CGE_KEY_D 'D'
#define CGE_KEY_E 'E'
#define CGE_KEY_F 'F'
#define CGE_KEY_G 'G'
#define CGE_KEY_H 'H'
#define CGE_KEY_I 'I'
#define CGE_KEY_J 'J'
#define CGE_KEY_K 'K'
#define CGE_KEY_L 'L'
#define CGE_KEY_M 'M'
#define CGE_KEY_N 'N'
#define CGE_KEY_O 'O'
#define CGE_KEY_P 'P'
#define CGE_KEY_Q 'Q'
#define CGE_KEY_R 'R'
#define CGE_KEY_S 'S'
#define CGE_KEY_T 'T'
#define CGE_KEY_U 'U'
#define CGE_KEY_V 'V'
#define CGE_KEY_W 'W'
#define CGE_KEY_X 'X'
#define CGE_KEY_Y 'Y'
#define CGE_KEY_Z 'Z'
//TODO
#endif

#ifdef LINUX
/* Dependency: sudo apt install libx11-dev */
#include <X11/Xlib.h>

#define cgechar char
#define cgestring std::string
#define cgeostringstream std::ostringstream
#define cgecout std::cout
#define cgecin std::cin
#define cgepopen popen
#define cgepclose pclose

#define _FBLOCK ' '
#define EMPTY_STRING ""
#define CGE_YES "y"
#define CGE_NO "n"

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

/* Keyboard Virtual Keys: */
#define CGE_KEY_A 'a'
#define CGE_KEY_B 'b'
#define CGE_KEY_C 'c'
#define CGE_KEY_D 'd'
#define CGE_KEY_E 'r'
#define CGE_KEY_F 'f'
#define CGE_KEY_G 'g'
#define CGE_KEY_H 'h'
#define CGE_KEY_I 'i'
#define CGE_KEY_J 'j'
#define CGE_KEY_K 'k'
#define CGE_KEY_L 'l'
#define CGE_KEY_M 'm'
#define CGE_KEY_N 'n'
#define CGE_KEY_O 'o'
#define CGE_KEY_P 'p'
#define CGE_KEY_Q 'q'
#define CGE_KEY_R 'r'
#define CGE_KEY_S 's'
#define CGE_KEY_T 't'
#define CGE_KEY_U 'u'
#define CGE_KEY_V 'v'
#define CGE_KEY_W 'w'
#define CGE_KEY_X 'x'
#define CGE_KEY_Y 'y'
#define CGE_KEY_Z 'z'
//TODO

/* x11 Display to get input from console window */
Display* d;
/* x11 Autorepeat: true if usually enabled */
bool autoreapeat = false;
#endif

#include <stdio.h>
#include <utility>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <map>

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

/* -------------------- declaration -------------------- */
/* Runs cmd command and returns output */
cgestring PromptCommand(std::string cmd);

#ifdef LINUX
/*
* get ID from terminal window and enable XEvents for KeyPress, KeyRelease and FocusChange
* to be able to update CGEKeyRegistry
*/
bool SelectXInput();

/* checks if Autorepeat is usually enabled to reenable it if necessary */
bool IsAutorepeatEnabled();

/* Sets x11 autorepeat */
void SetAutorepeat(bool ar);
#endif

#ifdef WINDOWS
/* Trys to enable Virtual Terminal mode on Windows. Returns true if succesfull */
bool EnableTVMode();
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

    void update(bool value);

public:
    bool isDown();

    /*
    * Returns if key got pressed since last call.
    * If it got pressed resets value to false
    */
    bool gotPressed();

    /*
    * Returns if key got released since last call.
    * If it got released resets value to false
    */
    bool gotReleased();
};

class CGEKeyRegistry {
    friend class ConsoleGameEngine;
private:
    std::map<int, CGEKey*> key_register;

    CGEKeyRegistry() { }
    ~CGEKeyRegistry() {
        for (std::pair<int, CGEKey*> pair : key_register) {
            delete pair.second;
        }
    }

    /* update all keys of key_registry */
    void update();

public:
    /* register key to get checked and updated in key_register */
    CGEKey* registerKey(int cge_key);
};

class cgetostringstream : public cgeostringstream {
public:
    /* sets cursor to position <x>:<y> beginning with 0:0 */
    void setCursor(int x, int y);

    void changeTextFormat(int format);

    void setBackgroundColor(int red, int green, int blue);
};

class CGEGraphics {
public:
    virtual cgestring getOutput() = 0;
    virtual void switchBuffer() = 0;
};

template <class U>
class CGEBuffer {
    int width;
    U* buffer;
public:
    CGEBuffer(int width, int height) :
        width(width),
        buffer(new U[width * height]) { }
    ~CGEBuffer() {
        delete[] buffer;
    }

    U& get(int x, int y);
};

template <class U>
class CGEMap : public CGEGraphics {
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

    /* switches active and passive buffer */
    void switchBuffer() override;

    CGEBuffer<U>& getMainBuffer();

    CGEBuffer<U>& getActiveBuffer();

    CGEBuffer<U>& getPassiveBuffer();
};

class CGEChar {
public:
    bool changed = true;
    int foreground_color = TF_COLOR_FOREGROUND_DEFAULT;
    int background_color = TF_COLOR_BACKGROUND_DEFAULT;
    cgechar c = _FBLOCK;
};

class CGECharMap : public CGEMap<CGEChar> {
    friend class ConsoleGameEngine;
private:
    CGECharMap(int widht, int height) : 
        CGEMap<CGEChar>(widht, height) { }
    ~CGECharMap() { }

    cgestring getOutput() override;

public:
    void setChar(int x, int y, cgechar c);

    void setForegroundColor(int x, int y, int foreground_color);

    void setBackgroundColor(int x, int y, int background_color);

    cgechar getChar(int x, int y);

    int getForegroundColor(int x, int y);

    int getBackgroundColor(int x, int y);
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
    unsigned char green = 0;
    unsigned char blue = 0;

public:
    bool gotChanged();

    void setChanged(bool changed);

    bool isTransparent();

    void setTransparent(bool transparent);

    void setColor(unsigned char red, unsigned char green, unsigned char blue);

    unsigned char getRed();

    unsigned char getGreen();

    unsigned char getBlue();

    /* does only compare colors */
    bool operator==(CGEPixel& other);

    /* does only compare colors */
    bool operator!=(CGEPixel& other);
};

class CGETexture {
private:
    CGEBuffer<CGEPixel> buffer;
    bool changed = true;
    bool visible = true;
    const int priority;
    int width;
    int height;
    int x = 0;
    int y = 0;
public:
    CGETexture(int priority, int width, int height) :
        priority(priority),
        width(width),
        height(height),
        buffer(width, height) { }
    ~CGETexture() {}

    /* Set position of texture. It can be out of bones. */
    void setPosition(int x, int y);

    void setPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue);

    void setVisible(bool visible);

    int getPriority();

    int getX();

    int getY();
};

class CGEPixelMap : public CGEMap<CGEPixel> {
    friend class ConsoleGameEngine;
private:
    int pixel_width;
    int pixel_height;
    std::vector<CGETexture*> textures;
    std::map<int, CGEBuffer<CGEPixel>> priority_buffers;

    CGEPixelMap(int widht, int height, int pixel_width, int pixel_height) :
        CGEMap(widht, height),
        pixel_width(pixel_width),
        pixel_height(pixel_height) { }
    ~CGEPixelMap() { }

    cgestring getOutput() override;

public:
    void setPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue);

    void handleTexture(CGETexture* texture);
};

enum CGEState {
    //TODO full handling
    CGE_UNINITIALIZED, //need to call ConsoleGameEngine::init
    CGE_INITIALIZING, //need to call ConsoleGameEngine::setMap
    CGE_INITIALIZED, //ready to start, call ConsoleGameEngine::start
    CGE_RUNNING, //running in mechanics and graphics thread
    CGE_STOPPING, //stopping without specific issue
    CGE_ERROR = 64, //error occured, program will continue but ending with error code
    CGE_CRITICAL_ERROR = 128 //critical error occured, prgram will stop as soon as possible
};

class ConsoleGameEngine {
private:
    int state = CGE_UNINITIALIZED;
    CGEKeyRegistry* key_registry = nullptr;
    CGEGraphics* graphics = nullptr;
    void (*game_loop) (double) = nullptr;

    //Use ConsoleGameEngine::GetInstance to get an instance to interagate with
    ConsoleGameEngine(ConsoleGameEngine& other) { }
    void operator=(ConsoleGameEngine& other) { }
    ConsoleGameEngine() :
        key_registry(new CGEKeyRegistry()) { }
    ~ConsoleGameEngine() {
        if (graphics != nullptr) delete graphics;
        delete key_registry;
    }

public:
    /* Returns instance of ConsoleGameEngine to interagate with */
    static ConsoleGameEngine& GetInstance();

    /* Returns reference to key_registry for registering keys */
    CGEKeyRegistry& getKeyRegistry();

    /* Asks user to continue game and sets state of ConsoleGameEngine depending on anser */
    void error(std::string error_out);

    /* Starts the game engine. Returns 0 if no error occured */
    int start();

    /* Stops the game engine. Call in game_loop */
    void stop();

    /* Sets font size of terminal which is applied while game engine is running */
    void setFontSize(int width, int height);

    void hideCursor();

    void showCurosr();

    void enableCursorBlinking();

    void disableCursorBlinking();

    void useAlternativeScreenBuffer();

    void useMainScreenBuffer();

    /*
    * Initializes game engine. Has to be called to get in state CGE_INITIALIZING.
    * Parameter is a pointer to the game_loop with the paramter double <time_passed>
    */
    void init(void (*game_loop) (double));

    /*
    * Sets Map/Graphics and returns pointer to it.
    * Has to be called to get in state CGE_INITIALIZED
    */
    CGECharMap* setCharMap(int width, int height);

    /*
    * Sets Map/Graphics and returns pointer to it.
    * Has to be called to get in state CGE_INITIALIZED
    */
    CGEPixelMap* setPixelMap(int widht, int height, int pixel_width, int pixel_height);

private:
    /* Returns true if engine shall continue to run */
    bool run();

    /*
    * Sets state of game engine. Once CGE_ERROR is set, it will remain in bit code.
    * CGE_CRITICAL_ERROR will never be replaced and the game engine will stop as soon as possible
    */
    void setState(int state);

    /* Returns true if new_state is executeable */
    bool checkState(CGEState new_state);

    /* Sets Map/Graphics */
    template <class T>
    T* setMap(T* map);

    /* Sends output to terminal */
    void render(cgestring output);

    //Variables for thread handling:
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

    /* Game mechanics thread. Keys getting updated, game_loop called. */
    void thread_mechanics();

    /* Game graphics thread. message wich is send to terminal is generated and sent from the passive buffer from graphics */
    void thread_graphics();
};

/* -------------------- definition -------------------- */
inline cgestring PromptCommand(std::string cmd) {
    char buffer[255];
    cgeostringstream result;
    FILE* pipe = cgepopen(cmd.c_str(), "r");
    if (!pipe) {
        ConsoleGameEngine::GetInstance().error("fail popen to get command input");
        return EMPTY_STRING;
    }
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result << buffer;
    }
    cgepclose(pipe);
    return result.str();
}

#ifdef WINDOWS
inline bool EnableTVMode() {
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
inline bool SelectXInput() {
    //TODO
    d = XOpenDisplay(NULL);
    system("env | grep \"WINDOW *ID *=\"");
    Window w;
    std::cin >> w;
    XSelectInput(d, w, KeyPressMask | KeyReleaseMask | FocusChangeMask);
    return true;
}

inline bool IsAutorepeatEnabled() {
    //TODO
    system("xset q | \"auto *repeat: *on\"");
    return true;
}

inline void SetAutorepeat(bool ar) {
    if (ar) {
        system("xset r on");
    }
    else {
        system("xset r off");
    }
}
#endif

inline void CGEKey::update(bool value) {
    if (is_down != value) {
        if (value) {
            got_pressed = true;
            is_down = true;
        }
        else {
            got_released = true;
            is_down = false;
        }
    }
}

inline bool CGEKey::isDown() {
    return is_down;
}

inline bool CGEKey::gotPressed() {
    if (got_pressed) {
        got_pressed = false;
        return true;
    }
    return false;
}

inline bool CGEKey::gotReleased() {
    if (got_released) {
        got_released = false;
        return true;
    }
    return false;
}

#ifdef WINDOWS
inline void CGEKeyRegistry::update() {
    for (std::pair<int, CGEKey*> kv : key_register) {
        SHORT state = GetKeyState(kv.first);
        if (state < 0) {
            if (!kv.second->is_down) {
                kv.second->is_down = true;
                kv.second->got_pressed = true;
            }
        }
        else {
            if (kv.second->is_down) {
                kv.second->is_down = false;
                kv.second->got_released = true;
            }
        }
    }
}
#endif
#ifdef LINUX
inline void CGEKeyRegistry::update() {
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
#endif

inline CGEKey* CGEKeyRegistry::registerKey(int cge_key) {
    key_register[cge_key] = new CGEKey();
    return key_register[cge_key];
}

inline void cgetostringstream::setCursor(int x, int y) {
    *this << CSI << y + 1 << VTS_SEPERATOR << x + 1 << VTS_SET_CURSOR_END;
}

inline void cgetostringstream::changeTextFormat(int format) {
    *this << CSI << format << VTS_CHANGE_TEXT_FORMAT_END;
}

inline void cgetostringstream::setBackgroundColor(int red, int green, int blue) {
    *this << VTS_CHANGE_BACKGROUND_COLOR << red << VTS_SEPERATOR << green << VTS_SEPERATOR << blue << VTS_CHANGE_TEXT_FORMAT_END;
}

template<class U>
inline U& CGEBuffer<U>::get(int x, int y)
{
    return buffer[y * width + x];
}

template<class U>
inline void CGEMap<U>::switchBuffer() {
    flag_buffer = !flag_buffer;
}

template<class U>
inline CGEBuffer<U>& CGEMap<U>::getMainBuffer() {
    return buffer_0;
}

template<class U>
inline CGEBuffer<U>& CGEMap<U>::getActiveBuffer() {
    return (flag_buffer ? buffer_1 : buffer_2);
}

template<class U>
inline CGEBuffer<U>& CGEMap<U>::getPassiveBuffer() {
    return (!flag_buffer ? buffer_1 : buffer_2);
}

inline cgestring CGECharMap::getOutput() {
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
                if (getPassiveBuffer().get(x, y).background_color != last_color_bg) {
                    stream.changeTextFormat(getPassiveBuffer().get(x, y).background_color);
                    last_color_bg = getPassiveBuffer().get(x, y).background_color;
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

inline void CGECharMap::setChar(int x, int y, cgechar c) {
    getMainBuffer().get(x, y).c = c;
    getActiveBuffer().get(x, y).c = c;
    getActiveBuffer().get(x, y).changed = true;
}

inline void CGECharMap::setForegroundColor(int x, int y, int foreground_color) {
    getMainBuffer().get(x, y).foreground_color = foreground_color;
    getActiveBuffer().get(x, y).foreground_color = foreground_color;
    getActiveBuffer().get(x, y).changed = true;
}

inline void CGECharMap::setBackgroundColor(int x, int y, int background_color) {
    getMainBuffer().get(x, y).background_color = background_color;
    getActiveBuffer().get(x, y).background_color = background_color;
    getActiveBuffer().get(x, y).changed = true;
}

inline cgechar CGECharMap::getChar(int x, int y) {
    return getMainBuffer().get(x, y).c;
}

inline int CGECharMap::getForegroundColor(int x, int y) {
    return getMainBuffer().get(x, y).foreground_color;
}

inline int CGECharMap::getBackgroundColor(int x, int y) {
    return getMainBuffer().get(x, y).background_color;
}

inline bool CGEPixel::gotChanged() {
    return changed;
}

inline void CGEPixel::setChanged(bool changed) {
    this->changed = changed;
}

inline bool CGEPixel::isTransparent() {
    return transparent;
}

inline void CGEPixel::setTransparent(bool transparent) {
    this->transparent = transparent;
}

inline void CGEPixel::setColor(unsigned char red, unsigned char green, unsigned char blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    changed = true;
}

inline unsigned char CGEPixel::getRed() {
    return red;
}

inline unsigned char CGEPixel::getGreen() {
    return green;
}

inline unsigned char CGEPixel::getBlue() {
    return blue;
}

inline bool CGEPixel::operator==(CGEPixel& other) {
    return red == other.red && green == other.green && blue == other.blue;
}

inline bool CGEPixel::operator!=(CGEPixel& other) {
    return !(*this == other);
}

inline void CGETexture::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
    changed = true;
}

inline void CGETexture::setPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue) {
    buffer.get(x, y).setColor(red, green, blue);
    changed = true;
}

inline void CGETexture::setVisible(bool visible) {
    this->visible = visible;
    changed = true;
}

inline int CGETexture::getPriority() {
    return priority;
}

inline int CGETexture::getX() {
    return x;
}

inline int CGETexture::getY() {
    return y;
}

inline cgestring CGEPixelMap::getOutput() {
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

inline void CGEPixelMap::setPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue) {
    getMainBuffer().get(x, y).setColor(red, green, blue);
    getActiveBuffer().get(x, y).setColor(red, green, blue);
}

inline ConsoleGameEngine& ConsoleGameEngine::GetInstance() {
    static ConsoleGameEngine cge;
    return cge;
}

inline CGEKeyRegistry& ConsoleGameEngine::getKeyRegistry() {
    return *key_registry;
}

inline void ConsoleGameEngine::error(std::string error_out) {
    //TODO switch to mainInputBuffer and back...
    cgecout << "Error occured: \n";
    cgecout << error_out.c_str() << '\n';
    cgecout << "Do you want to continue the program?\n";
    cgecout << "Enter: (y: continue) (n: stop as soon as possible)\n";
    while (true) {
        cgestring input;
        cgecin >> input;
        if (input == CGE_YES) {
            setState(CGE_ERROR);
            break;
        }
        else if (input == CGE_NO) {
            setState(CGE_CRITICAL_ERROR);
            break;
        }
    }
}

inline int ConsoleGameEngine::start() {
    if (checkState(CGE_RUNNING)) {
        setState(CGE_RUNNING);

        useAlternativeScreenBuffer();
        hideCursor();

        std::thread thread_mechanics(&ConsoleGameEngine::thread_mechanics, this);
        std::thread thread_graphics(&ConsoleGameEngine::thread_graphics, this);

        thread_mechanics.join();
        thread_graphics.join();

        useMainScreenBuffer();
        //TODO set back to defaults
        showCurosr();
    }
    else {
        error("Could not start ConsoleGameEngine. ConsoleGameEngine::setMap has to be called.");
    }

    //TODO better error handling
    return (state < CGE_ERROR ? 0 : -1);
}

inline void ConsoleGameEngine::stop() {
    state = CGE_STOPPING;
}
#ifdef WINDOWS
inline void ConsoleGameEngine::setFontSize(int width, int height) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
    lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
    lpConsoleCurrentFontEx->dwFontSize.X = width;
    lpConsoleCurrentFontEx->dwFontSize.Y = height;
    SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}
#endif
#ifdef LINUX
inline void ConsoleGameEngine::setFontSize(int width, int height) {
    //TODO
}
#endif

inline void ConsoleGameEngine::hideCursor() {
    cgecout << VTS_HIDE_CURSER;
}

inline void ConsoleGameEngine::showCurosr() {
    cgecout << VTS_SHOW_CURSER;
}

inline void ConsoleGameEngine::enableCursorBlinking() {
    cgecout << VTS_ENABLE_CURSER_BLINKING;
}

inline void ConsoleGameEngine::disableCursorBlinking() {
    cgecout << VTS_ENABLE_CURSER_BLINKING;
}

inline void ConsoleGameEngine::useAlternativeScreenBuffer() {
    cgecout << VTS_USE_ALTERNATE_SCREEN_BUFFER;
}

inline void ConsoleGameEngine::useMainScreenBuffer() {
    cgecout << VTS_USE_MAIN_SCREEN_BUFFER;
}

inline void ConsoleGameEngine::init(void (*game_loop) (double)) {
    if (game_loop == nullptr) {
        error("game_loop cant be nullptr");
    }
    this->game_loop = game_loop;

    //TODO get default font etc. from terminal and save to restore
#ifdef WINDOWS
    EnableTVMode(); //TODO error handling
#endif
#ifdef LINUX
    SelectXInput(); //TODO error handling

    //TODO:
    if (IsAutrepeatEnabled()) {
        autorepeat = true; 
    }
    else {
        autorepeat = false;
    }
#endif

    setState(CGE_INITIALIZING);
}

inline CGECharMap* ConsoleGameEngine::setCharMap(int width, int height) {
    return setMap(new CGECharMap(width, height));
}

inline CGEPixelMap* ConsoleGameEngine::setPixelMap(int width, int height, int pixel_width, int pixel_height) {
    return setMap(new CGEPixelMap(width, height, pixel_width, pixel_height));
}

inline bool ConsoleGameEngine::run() {
    return (state == CGE_RUNNING || state == (CGE_RUNNING | CGE_ERROR));
}

inline void ConsoleGameEngine::setState(int state) {
    if (this->state < CGE_CRITICAL_ERROR) {
        if (this->state < CGE_ERROR) {
            if (state == CGE_ERROR) {
                this->state |= CGE_ERROR;
            }
            else {
                this->state = state;
            }
        }
        else {
            if (state < CGE_ERROR) {
                this->state = (CGE_ERROR | state);
            }
            else {
                this->state = state;
            }
        }
    }
}

inline bool ConsoleGameEngine::checkState(CGEState new_state) {
    if (this->state >= CGE_CRITICAL_ERROR) return false;
    if (this->state >= CGE_ERROR) {
        return (this->state - CGE_ERROR) >= state - 1;
    }
    else {
        return this->state >= state - 1;
    }
}

template<class T>
inline T* ConsoleGameEngine::setMap(T* map) {
    if (checkState(CGE_INITIALIZED)) {
        if (graphics != nullptr) {
            delete graphics;
        }
        this->graphics = map;
        setState(CGE_INITIALIZED);
    }
    else {
        error("ConsoleGameEngine not initializing. Cannot set map.\nConsoleGameEngine::init should have been called.");
        setState(CGE_ERROR);
    }
    return (T*)map;
}

inline void ConsoleGameEngine::render(cgestring output) {
    if (output != EMPTY_STRING) cgecout << output;
}

inline void ConsoleGameEngine::thread_mechanics() {
    auto start = std::chrono::steady_clock::now();
    while (run()) {
        std::unique_lock<std::mutex> lock(buffer_flag ? mutex_mech_1 : mutex_mech_2);
        while (!(buffer_flag ? continue_mech_1 : continue_mech_2)) (buffer_flag ? cv_mech_1 : cv_mech_2).wait(lock);

        key_registry->update();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;
        game_loop(elapsed_time.count());
        start = std::chrono::steady_clock::now();

        if (buffer_flag) {
            graphics->switchBuffer();

            mutex_graphics_1.lock();
            continue_graphics_1 = true;
            mutex_graphics_1.unlock();
            cv_graphics_1.notify_all();
            continue_mech_1 = false;
        }
        else {
            graphics->switchBuffer();

            mutex_graphics_2.lock();
            continue_graphics_2 = true;
            mutex_graphics_2.unlock();
            cv_graphics_2.notify_all();
            continue_mech_2 = false;
        }
        buffer_flag = !buffer_flag;
    }
}

inline void ConsoleGameEngine::thread_graphics() {
    while (run()) {
        std::unique_lock<std::mutex> lock(buffer_flag_graphics ? mutex_graphics_1 : mutex_graphics_2);
        while (!(buffer_flag_graphics ? continue_graphics_1 : continue_graphics_2)) (buffer_flag_graphics ? cv_graphics_1 : cv_graphics_2).wait(lock);

        cgestring output;
        if (buffer_flag_graphics) {
            output = graphics->getOutput();

            mutex_mech_1.lock();
            continue_mech_1 = true;
            mutex_mech_1.unlock();
            cv_mech_1.notify_all();
            continue_graphics_1 = false;
        }
        else {
            output = graphics->getOutput();

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
