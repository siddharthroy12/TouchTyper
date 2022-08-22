#include "helpers.hpp"
#include "typingTest.hpp"
#include <vector>
#include <cmath>

#define MAX_WIDTH 900
#define FONT_SIZE 28

void typingTest(Context &context) {
    // We are using a monospace font so every character will have same with
    Vector2 sizeOfCharacter = MeasureTextEx(context.font, "a", FONT_SIZE, 1);

    int width = std::min(context.screenWidth, MAX_WIDTH);
    int height = std::ceil((context.sentence.size()*sizeOfCharacter.x)/ width) * sizeOfCharacter.y;
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);

    // Where the first character will be placed
    Vector2 startingPosition;
    startingPosition.x = center.x - width/2.0;
    startingPosition.y = center.y - height/2.0;

    // Positions of every character of the sentence on the screen
    std::vector<Vector2> positionsOfCharacters(context.sentence.size());
    Vector2 currentPositon = startingPosition;

    std::string word;

    // Draw Container
    //DrawRectangle(startingPosition.x, startingPosition.y, width, height, BLACK);


    for (int i = 0; i < context.sentence.size(); i++) {
        if (context.sentence[i] == ' ' or i == (context.sentence.size() - 1)) {
            word += context.sentence[i];
            int widthOfWord = word.size() * sizeOfCharacter.x;

            // Go to new line if word is overflowing
            if (currentPositon.x + widthOfWord > center.x + width/2.0) {
                currentPositon.y += sizeOfCharacter.y;
                currentPositon.x = startingPosition.x;
            }

            int index = i - word.size();

            for (auto character : word) {
                Color color = context.theme.text;
                std::string c(1, character);

                // Draw cursor
                if (index == context.input.size()-1) {
                    DrawRectangle(currentPositon.x, currentPositon.y, sizeOfCharacter.x, sizeOfCharacter.y, context.theme.cursor);
                }

                if (index+1 < context.input.size()) {
                    if (context.sentence[index+1] == context.input[index+1]) {
                        color = context.theme.correct;
                    } else {
                        color = context.theme.wrong;
                        c = std::string(1, context.input[index+1]);

                        if (c == std::string(" ")) {
                            DrawRectangle(currentPositon.x, currentPositon.y, sizeOfCharacter.x, sizeOfCharacter.y, context.theme.wrong);
                        }
                    }
                }

                if (character == ' ') {
                    //DrawRectangle(currentPositon.x, currentPositon.y, sizeOfCharacter.x, sizeOfCharacter.y, context.theme.wrong);
                }
                DrawTextEx(context.font, c.c_str(), currentPositon, FONT_SIZE, 1, color);
                currentPositon.x += sizeOfCharacter.x;
                index++;
            }

            word = "";
        } else {
            word += context.sentence[i];
        }
    }
}
