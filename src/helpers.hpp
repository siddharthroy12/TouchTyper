#ifndef HELPERS_HPP
#define HELPERS_HPP
#include "Context.hpp"
#include "../libs/raylib/src/raylib.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

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

template<class BidiIter >
BidiIter random_unique(BidiIter begin, BidiIter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        BidiIter r = begin;
        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}

inline void restartTest(Context &context) {
    context.sentence = "";
    int amount = context.testSettings.testModeAmounts[context.testSettings.selectedAmount];

    if (context.testSettings.testMode == TestMode::TIME) {
        amount = 100;
    }

    random_unique(context.words.begin(), context.words.end(), amount);

    for(int i = 0; i < amount; ++i) {
        context.sentence += context.words[i];
        context.sentence += ' ';
    }
    context.sentence.pop_back();

    context.input = "";
    context.currentScreen = Screen::TEST;
    context.wpm = 0;
    context.cpm = 0;
    context.accuracy = 0;
    context.raw = 0;
    context.testRunning = false;
}

inline void endTest(Context &context) {
    context.testRunning = false;
    context.currentScreen = Screen::RESULT;
    context.testEndTime = getTimeInMin();
}

inline bool getFileContent(std::string fileName, std::vector<std::string> & vecOfStrs) {
    // Open the File
    std::ifstream in(fileName.c_str());

    // Check if object is valid
    if(!in) {
        std::cerr << "Cannot open the File : " << fileName << std::endl;
        return false;
    }

    std::string str;
    // Read the next line from File untill it reaches the end.
    while (std::getline(in, str)) {
        // Line contains string of length > 0 then save it in vector
        if(str.size() > 0)
            vecOfStrs.push_back(str);
    }

    //Close The File
    in.close();
    return true;
}
#endif
