#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <string>
#include "../libs/raylib/src/raylib.h"

struct FontData {
    Font font;
    int size;
};

struct Fonts {
    FontData titleFont;
    FontData typingTestFont;
    FontData tinyFont;
};

struct Theme {
    Color background;
    Color cursor;
    Color text;
    Color wrong;
    Color correct;
    Color highlight;
};

enum class TestMode { TIME, WORDS };

struct TestSettings {
    bool usePunctuation = false;
    bool useNumbers = false;
    TestMode testMode = TestMode::TIME;
    int time = 60;
    int words = 100;
};

struct Context {
    int screenWidth;
    int screenHeight;
    float deltaTime;
    std::string sentence;
    std::string input;
    int cpm;
    int wpm;
    Fonts fonts;
    Theme theme;
    TestSettings testSettings;
    bool mouseOnClickable = false;
};

#endif
