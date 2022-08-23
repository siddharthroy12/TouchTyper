#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <string>
#include "../libs/raylib/src/raylib.h"
#include "Theme.hpp"

struct FontData {
    Font font;
    int size;
};

struct Context {
    int screenWidth;
    int screenHeight;
    float deltaTime;
    std::string sentence;
    std::string input;
    int cpm;
    FontData titleFontData;
    FontData typingTestFontData;
    Theme theme;
};

#endif
