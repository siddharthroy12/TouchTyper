#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include <string>
#include <vector>
#include "../libs/raylib/src/raylib.h"

enum class CursorStyle { BLOCK = 0 , LINE = 1, UNDERLINE = 2 };

struct FontData {
    Font font;
    int size;
};

struct Sounds {
    Sound clickSound1;
    Sound clickSound2;
    Sound clickSound3;
};

struct Fonts {
    FontData titleFont;
    FontData typingTestFont;
    FontData tinyFont;
    FontData bigFont;
};

struct Theme {
    std::string name;
    Color background;
    Color cursor;
    Color text;
    Color wrong;
    Color correct;
    Color highlight;
};

struct WordList {
    std::string name;
    std::vector<std::string> words;
};

enum class TestMode { TIME = 0, WORDS };
enum class Screen { TEST, RESULT };

struct TestSettings {
    bool usePunctuation = false;
    bool useNumbers = false;
    TestMode testMode = TestMode::TIME;
    std::vector<int> testModeAmounts = {120, 60, 30, 15};
    int selectedAmount = 1;
};

struct Context {
    int screenWidth;
    int screenHeight;
    Screen currentScreen;
    Sounds sounds;
    std::vector<WordList> wordsLists;
    int selectedWordList = 0;
    std::string sentence;
    std::string input;
    double testStartTime = 0;
    bool testRunning = false;
    double testEndTime = 0;
    bool soundOn = true;
    int wpm = 0;
    int cpm = 0;
    int raw = 0;
    int incorrecLetters = 0;
    int correctLetters = 0;
    int furthestVisitedIndex = -1;
    int accuracy = 0;
    Fonts fonts;
    std::vector<Theme> themes;
    int selectedTheme = 2;
    CursorStyle cursorStyle = CursorStyle::BLOCK;
    TestSettings testSettings;
    bool mouseOnClickable = false;
    void load();
    void unload();
};

#endif
