#include "header.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include <vector>

std::vector<std::string> firstOptions = {"numbers", "punctuation"};
std::vector<std::string> secondOptions = {"words", "time"};


void options(Context &context, std::vector<std::string> &options, Vector2 &startingPosition, Vector2 &sizeOfCharacter) {
    for (auto word : options) {
        Vector2 optionPosition = startingPosition;
        optionPosition.x -= word.size() * sizeOfCharacter.x;
        Color color = context.theme.text;
        Rectangle optionRect;
        optionRect.x = optionPosition.x;
        optionRect.y = optionPosition.y;
        optionRect.width = word.size() * sizeOfCharacter.x + sizeOfCharacter.x;
        optionRect.height = sizeOfCharacter.y;

        if (CheckCollisionPointRec(GetMousePosition(), optionRect)) {
            color = context.theme.highlight;
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

            }
        }

        if ((word == firstOptions[1] && context.testSettings.usePunctuation) ||
            (word == firstOptions[0] && context.testSettings.useNumbers) ||
            (word == secondOptions[1] && context.testSettings.testMode == TestMode::TIME) ||
            (word == secondOptions[0] && context.testSettings.testMode == TestMode::WORDS)) {
            color = context.theme.correct;
        }

        for (auto letter : word) {
            std::string c(1, letter);
            DrawTextEx(context.fonts.tinyFont.font,
                c.c_str(), optionPosition,
                context.fonts.tinyFont.size, 1, color);
            optionPosition.x += sizeOfCharacter.x;
        }

        startingPosition = optionPosition;
        startingPosition.x -= word.size() * sizeOfCharacter.x + sizeOfCharacter.x;
    }
}

void header(Context &context) {
    int width = std::min(context.screenWidth-(PADDING*2), MAX_WIDTH);

    // Center of the screen
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);

    Vector2 topLeftPosition;
    topLeftPosition.x = center.x - width/2.0;
    topLeftPosition.y = PADDING;

    Vector2 topRightPosition;
    topRightPosition.x = center.x + width/2.0;
    topRightPosition.y = PADDING;

    // Draw Title
    DrawTextEx(context.fonts.titleFont.font,
            PROJECT_NAME, topLeftPosition,
            context.fonts.titleFont.size, 1, WHITE);

    // Draw Options
    Vector2 startingPosition = topRightPosition;
    Vector2 sizeOfCharacter = MeasureTextEx(context.fonts.tinyFont.font, "a",
            context.fonts.tinyFont.size, 1);

    options(context, firstOptions, startingPosition, sizeOfCharacter);
    startingPosition = topRightPosition;
    startingPosition.y += sizeOfCharacter.y;
    options(context, secondOptions, startingPosition, sizeOfCharacter);
}
