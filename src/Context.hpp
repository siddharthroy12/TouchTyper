#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <string>
#include "../libs/raylib/src/raylib.h"
#include "Theme.hpp"

struct Context {
    int screenWidth;
    int screenHeight;
    float deltaTime;
    std::string sentence;
    std::string input;
    int cpm;
    Font font;
    Theme theme;
};

#endif
