#include "helpers.hpp"
#include "typingTest.hpp"
#include <vector>
#include <cmath>
#include "../libs/raylib/src/raymath.h"
#include "constants.hpp"

Vector2 cursorPostion = {0, 0};
Vector2 newCursorPosition = {0, 0};
int yOffset = 0;

void typingTest(Context &context) {
    // We are using a monospace font so every character will have same with
    Vector2 sizeOfCharacter = MeasureTextEx(context.typingTestFontData.font, "a",
                                            context.typingTestFontData.size, 1);

    // To make it responsive
    int width = std::min(context.screenWidth-(PADDING*2), MAX_WIDTH);
    int height = sizeOfCharacter.y * 3;

    // Center of the screen
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);

    // Where the first character will be placed
    Vector2 startingPosition;
    startingPosition.x = center.x - width/2.0;
    startingPosition.y = (center.y - height/2.0) - yOffset;

    // Positions of character of the sentence on the screen
    Vector2 currentPositon = startingPosition;

    std::string word;

    // Draw Container
    BeginScissorMode(startingPosition.x, startingPosition.y + yOffset+1, width, height);

    // Animate cursor
    cursorPostion.x = Lerp(cursorPostion.x, newCursorPosition.x, 0.5);
    cursorPostion.y = Lerp(cursorPostion.y, newCursorPosition.y, 0.5);

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
                Color color = context.theme.text;
                std::string c(1, character);

                // Draw cursor
                if (index == context.input.size()-1) {
                    newCursorPosition = currentPositon;
                    yOffset = (line * sizeOfCharacter.y) - sizeOfCharacter.y;
                    DrawRectangle(cursorPostion.x, currentPositon.y,
                                  sizeOfCharacter.x, sizeOfCharacter.y,
                                  context.theme.cursor);
                }

                // Check if the input is correct or wrong
                if (index+1 < context.input.size()) {
                    if (context.sentence[index+1] == context.input[index+1]) {
                        color = context.theme.correct;
                    } else {
                        color = context.theme.wrong;
                        if (context.sentence[index+1] != ' ') {
                            wordMistake = true;
                        }

                        if (c == std::string(" ")) {
                            DrawRectangle(currentPositon.x, currentPositon.y,
                                          sizeOfCharacter.x, sizeOfCharacter.y,
                                          context.theme.wrong);
                        }
                    }
                }

                // Draw the text
                DrawTextEx(context.typingTestFontData.font, c.c_str(),
                           currentPositon, context.typingTestFontData.size,
                           1, color);

                currentPositon.x += sizeOfCharacter.x;
                index++;
            }

            Vector2 underlineEnd = currentPositon;
            underlineEnd.y += sizeOfCharacter.y;
            underlineEnd.x -= sizeOfCharacter.x;

            if (wordMistake) {
                DrawLineEx(underlineStart, underlineEnd, 2, context.theme.wrong);
            }

            word = "";
        } else {
            word += context.sentence[i];
        }
    }

    EndScissorMode();

}
