#include "header.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include <string>
#include <vector>
#include "../libs/raylib/src/raymath.h"

std::vector<std::string> firstOptions = {"numbers", "punctuation"};
std::vector<std::string> secondOptions = {"words", "time"};
float targetBarHeight = 0;
float barHeight = 0;

void options(Context &context, std::vector<std::string> &options, Vector2 &startingPosition, Vector2 &sizeOfCharacter, bool isAmounts) {
    Theme theme = context.themes[context.selectedTheme];

    for (int i = 0; i < options.size(); i++) {
        auto word = options[i];

        Vector2 optionPosition = startingPosition;
        optionPosition.x -= word.size() * sizeOfCharacter.x;
        Color color = theme.text;
        Rectangle optionRect;
        optionRect.x = optionPosition.x;
        optionRect.y = optionPosition.y;
        optionRect.width = word.size() * sizeOfCharacter.x + sizeOfCharacter.x;
        optionRect.height = sizeOfCharacter.y;

        if (CheckCollisionPointRec(GetMousePosition(), optionRect)) {
            color = theme.highlight;
            context.mouseOnClickable = true;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (word == firstOptions[0]) {
                    context.testSettings.useNumbers = !context.testSettings.useNumbers;
                } else if (word == firstOptions[1]) {
                    context.testSettings.usePunctuation = !context.testSettings.usePunctuation;
                } else if (word == secondOptions[0]) {
                    context.testSettings.testMode = TestMode::WORDS;
                } else if (word == secondOptions[1]) {
                    context.testSettings.testMode = TestMode::TIME;
                }

                if (isAmounts) {
                    context.testSettings.selectedAmount = i;
                }

                if (context.currentScreen == Screen::TEST) {
                    restartTest(context, false);
                }
            }
        }

        if ((word == firstOptions[1] && context.testSettings.usePunctuation) ||
            (word == firstOptions[0] && context.testSettings.useNumbers) ||
            (word == secondOptions[1] && context.testSettings.testMode == TestMode::TIME) ||
            (word == secondOptions[0] && context.testSettings.testMode == TestMode::WORDS) ||
            (isAmounts && i == context.testSettings.selectedAmount)) {
            color = theme.correct;
        }

        drawMonospaceText(context.fonts.tinyFont.font, word, optionPosition, context.fonts.tinyFont.size, color);
        optionPosition.x += sizeOfCharacter.x * word.size();

        startingPosition = optionPosition;
        startingPosition.x -= word.size() * sizeOfCharacter.x + sizeOfCharacter.x;
    }
}

void header(Context &context) {
    int width = std::min(context.screenWidth-(PADDING*2), MAX_WIDTH);

    Theme theme = context.themes[context.selectedTheme];

    // Center of the screen
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);

    Vector2 topLeftPosition;
    topLeftPosition.x = center.x - width/2.0;
    topLeftPosition.y = PADDING;

    Vector2 topRightPosition;
    topRightPosition.x = center.x + width/2.0;
    topRightPosition.y = PADDING;

    // Draw Title
    Color color;
    std::string text;
    if (!context.testRunning) {
        color = theme.correct;
        switch (context.currentScreen) {
            case Screen::TEST:
                text = "start typing";
                break;
            case Screen::RESULT:
                text = "result";
                break;
        }
    } else {
        color = theme.text;
        if (context.testSettings.testMode == TestMode::WORDS) {
            text = TextFormat("%d/%d", context.input.size(), context.sentence.size());
        } else {
            text = TextFormat("%ds", (int)(GetTime() - context.testStartTime));
        }
    }

    if (!context.testRunning) {
        DrawTextEx(context.fonts.titleFont.font,
                text.c_str(), topLeftPosition,
                context.fonts.titleFont.size, 1, color);
    } else {
        drawMonospaceText(context.fonts.titleFont.font, text, topLeftPosition, context.fonts.titleFont.size, color);
    }

    // Draw Options
    Vector2 startingPosition = topRightPosition;
    Vector2 sizeOfCharacter = MeasureTextEx(context.fonts.tinyFont.font, "a",
            context.fonts.tinyFont.size, 1);

    options(context, firstOptions, startingPosition, sizeOfCharacter, false);
    startingPosition = topRightPosition;
    startingPosition.y += sizeOfCharacter.y;
    options(context, secondOptions, startingPosition, sizeOfCharacter, false);

    startingPosition.x = topRightPosition.x;
    startingPosition.y += sizeOfCharacter.y;

    std::vector<std::string> thirdOptions;
    for (auto option : context.testSettings.testModeAmounts) {
        thirdOptions.push_back(std::to_string(option));
    }

    options(context, thirdOptions, startingPosition, sizeOfCharacter, true);

    // Draw progress bar
    float barWidth = 0;
    barHeight = Lerp(barHeight, targetBarHeight, GetFrameTime()*4);
    float percentage = 0;
    int amount = context.testSettings.testModeAmounts[context.testSettings.selectedAmount];

    if (context.testRunning) {
        targetBarHeight = 5;
    } else {
        targetBarHeight = 0;
    }

    switch (context.testSettings.testMode) {
        case TestMode::TIME:
            percentage = (GetTime() - context.testStartTime) / (float)amount;
            break;
        case TestMode::WORDS:
            percentage = (context.furthestVisitedIndex+1.0) / context.sentence.size();
            break;
    }

    DrawRectangle(0, 0, context.screenWidth * percentage, barHeight, theme.correct);

}
