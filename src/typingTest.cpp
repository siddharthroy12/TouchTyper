#include "helpers.hpp"
#include "typingTest.hpp"
#include <vector>
#include <cmath>
#include "../libs/raylib/src/raymath.h"
#include "constants.hpp"

Vector2 cursorPostion = {0, 0};
Vector2 newCursorPosition = {0, 0};
float cursorSpeed = 20;
int yOffset = 0;

std::vector<std::vector<char>> keyboard = {
    {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',']'},
    {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\''},
    {'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'},
    {' '}
};

void typingTest(Context &context) {
    // We are using a monospace font so every character will have same with
    Vector2 sizeOfCharacter = MeasureTextEx(context.fonts.typingTestFont.font, "a",
                                            context.fonts.typingTestFont.size, 1);

    Theme theme = context.themes[context.selectedTheme];

    // To make it responsive
    int width = std::min(context.screenWidth-(PADDING*2), MAX_WIDTH);
    int height = sizeOfCharacter.y * 3;

    // Center of the screen
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);

    // Where the first character will be placed
    Vector2 startingPosition;
    startingPosition.x = center.x - width/2.0;
    startingPosition.y = (center.y - height/2.0) - 100;
    if (startingPosition.y < 95) {
        startingPosition.y = 95;
    }
    startingPosition.y -= yOffset;

    // Positions of character of the sentence on the screen
    Vector2 currentPositon = startingPosition;

    std::string word;

    // Draw live Score
    /*
    Vector2 liveScoreTextPos;
    liveScoreTextPos.y = startingPosition.y - sizeOfCharacter.y + yOffset;
    liveScoreTextPos.x = startingPosition.x;

    int time = context.testRunning ? ((GetTime() - context.testStartTime)) : 0;

    drawMonospaceText(context.fonts.typingTestFont.font,
               TextFormat("Time: %ds WPM:%d", time, context.wpm),
               liveScoreTextPos, context.fonts.typingTestFont.size, theme.text);
    */

    // Begin Drawing sentence
    BeginScissorMode(startingPosition.x, startingPosition.y + yOffset+1, width, height > 1 ? height : 1);

    // Animate cursor
    cursorPostion.x = Lerp(cursorPostion.x, newCursorPosition.x, cursorSpeed * GetFrameTime());
    cursorPostion.y = Lerp(cursorPostion.y, newCursorPosition.y, cursorSpeed * GetFrameTime());

    // To keep track of current line
    int line = 1;

    for (int i = 0; i < context.sentence.size(); i++) {
        // Detect the end of the word
        if (context.sentence[i] == ' ' || i == (context.sentence.size() - 1)) {
            // Add that ending space character too
            word += context.sentence[i];

            // Calculate the width of the word
            int widthOfWord = word.size() * sizeOfCharacter.x;

            // Go to new line if word is overflowing
            if (currentPositon.x + widthOfWord > center.x + width/2.0) {
                currentPositon.y += sizeOfCharacter.y;
                currentPositon.x = startingPosition.x;
                line++;
            }

            // Index of the character where the cursor should be
            int index = i - word.size();

            Vector2 underlineStart = currentPositon;
            underlineStart.y += sizeOfCharacter.y;
            bool wordMistake = false;

            // Draw each character
            for (auto character : word) {
                Color color = theme.text;
                std::string c(1, character);

                // Draw cursor
                if (index == context.input.size()-1) {
                    newCursorPosition = currentPositon;
                    yOffset = line > 2 ? ((line-1) * sizeOfCharacter.y) - sizeOfCharacter.y : 1;
                    DrawRectangle(cursorPostion.x+1, currentPositon.y,
                                  sizeOfCharacter.x, sizeOfCharacter.y,
                                  theme.cursor);
                }

                // Check if the input is correct or wrong
                if (index+1 < context.input.size()) {
                    if (context.sentence[index+1] == context.input[index+1]) {
                        color = theme.correct;
                    } else {
                        color = theme.wrong;
                        if (context.sentence[index+1] != ' ') {
                            wordMistake = true;
                        }

                        if (c == std::string(" ")) {
                            DrawRectangle(currentPositon.x, currentPositon.y,
                                          sizeOfCharacter.x, sizeOfCharacter.y,
                                          theme.wrong);
                        }
                    }
                }

                // Draw the text
                DrawTextEx(context.fonts.typingTestFont.font, c.c_str(),
                           currentPositon, context.fonts.typingTestFont.size,
                           1, color);

                currentPositon.x += sizeOfCharacter.x;
                index++;
            }

            Vector2 underlineEnd = currentPositon;
            underlineEnd.y += sizeOfCharacter.y;
            underlineEnd.x -= sizeOfCharacter.x;

            if (wordMistake) {
                DrawLineEx(underlineStart, underlineEnd, 2, theme.wrong);
            }

            word = "";
        } else {
            word += context.sentence[i];
        }
    }

    EndScissorMode();

    // Draw Keybaord
    const int sizeOfKey = 35;
    const int margin = 5;
    sizeOfCharacter = MeasureTextEx(context.fonts.tinyFont.font, "a",
                                            context.fonts.tinyFont.size, 1);
    startingPosition.y += yOffset;

    for (int i = 0; i < keyboard.size(); i++) {
        auto row = keyboard[i];
        int totalWidth = row[0] == ' ' ? 200 : (sizeOfKey * row.size()) + margin * (row.size()-1);
        Vector2 position;
        position.x = center.x - (totalWidth/2.0);
        position.y = (startingPosition.y + (sizeOfCharacter.y * 8)) + (sizeOfKey * i) + margin * i;

        for (auto key : row) {
            std::string c(1, key);
            Rectangle rect;
            rect.x = position.x;
            rect.y = position.y;
            rect.width = row[0] == ' ' ? 200 : sizeOfKey;
            rect.height = sizeOfKey;
            DrawRectangleRoundedLines(rect, 0.1, 5, 1, theme.text);
            if (IsKeyDown(toupper(key))) {
                DrawRectangleRounded(rect, 0.1, 5, theme.cursor);
            }
            Vector2 keyPosition;
            keyPosition.x = (rect.x + (sizeOfKey/2.0)) - (sizeOfCharacter.x/2.0);
            keyPosition.y = (rect.y + (sizeOfKey/2.0)) - (sizeOfCharacter.y/2.0);
            drawMonospaceText(context.fonts.tinyFont.font, c.c_str(), keyPosition, context.fonts.tinyFont.size, theme.text);

            position.x += sizeOfKey + margin;
        }
    }
}
