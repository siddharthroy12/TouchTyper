#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <string>
#include <vector>
#include "../libs/raylib/src/raylib.h"

struct FontData {
    Font font;
    int size;
};

struct Fonts {
    FontData titleFont;
    FontData typingTestFont;
    FontData tinyFont;
    FontData bigFont;
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
enum class Screen { TEST, RESULT };

struct TestSettings {
    bool usePunctuation = false;
    bool useNumbers = false;
    TestMode testMode = TestMode::TIME;
    std::vector<int> testModeAmounts = {120, 60, 30, 15};
    int selectedAmount = 0;
    int time = 60;
    int words = 100;
};

struct Context {
    int screenWidth;
    int screenHeight;
    Screen currentScreen;
    std::vector<std::string> words;
    std::string sentence;
    std::string input;
    double testStartTime = 0;
    bool testRunning = false;
    double testEndTime = 0;
    int wpm = 0;
    int cpm = 0;
    int raw = 0;
    int accuracy = 0;
    Fonts fonts;
    Theme theme;
    TestSettings testSettings;
    bool mouseOnClickable = false;
};

#endif
