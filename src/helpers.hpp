#ifndef HELPERS_HPP
#define HELPERS_HPP
#include "Context.hpp"
#include "../libs/raylib/src/raylib.h"
#include <string>

inline Vector2 getCenter(int width, int height) {
    Vector2 result;
    result.x = width/2.0;
    result.y = height/2.0;
    return result;
}

inline void drawMonospaceText(Font font, std::string text, Vector2 position, float fontSize, Color color) {
    Vector2 sizeOfCharacter = MeasureTextEx(font, "a", fontSize, 1);

    for (auto letter : text) {
        std::string c(1, letter);

        DrawTextEx(font, c.c_str(), position, fontSize, 1, color);
        position.x += sizeOfCharacter.x;
    }
}

inline double getTimeInMin() {
    return GetTime()/60;
}

inline void restartTest(Context &context) {
    // TODO: Generate new sentence

    context.input = "";
    context.currentScreen = Screen::TEST;
    context.wpm = 0;
    context.cpm = 0;
    context.accuracy = 0;
    context.raw = 0;
    context.testRunning = false;
}



#endif
