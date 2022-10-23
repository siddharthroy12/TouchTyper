#include "Context.hpp"
#include "helpers.hpp"
#include "typingTest.hpp"
#include <vector>
#include <cmath>
#include "../libs/raylib/src/raymath.h"
#include "constants.hpp"

Vector2 cursorPostion = {0, 0};
Vector2 newCursorPosition = {0, 0};
float cursorSpeed = 20;
float yOffset = 0;
float newYOffset = 0;
float cursorOpacity = 1;
int cursorOpacityDirection = 0;
float cursorStayVisibleTimer = 0;

std::vector<std::vector<char>> keyboard = {
    {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',']'},
    {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\''},
    {'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'},
    {' '}
};

float sinPulse(float frequency) {
    const float pi = 3.14f;
    return 0.5f * (1 + (float)std::sin(2 * pi * frequency * GetTime()));
}

void typingTest(Context &context) {
    // We are using a monospace font so every character will have same with
    Vector2 sizeOfCharacter = MeasureTextEx(context.fonts.typingTestFont.font, "a",
            context.fonts.typingTestFont.size, 1);

    Theme theme = context.themes[context.selectedTheme];

    // To make it responsive
    float width = std::min(context.screenWidth-(PADDING*2), MAX_WIDTH);
    float height = sizeOfCharacter.y * 3;

    // Center of the screen
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);

    // Animate scroll
    float speed = cursorSpeed * GetFrameTime();
    yOffset = Lerp(yOffset, newYOffset, (speed <= 0 || speed > 1) ? 1 : speed);

    // Animate cursor
    cursorPostion.x = Lerp(cursorPostion.x, newCursorPosition.x, (speed <= 0 || speed > 1) ? 1 : speed);
    cursorPostion.y = Lerp(cursorPostion.y, newCursorPosition.y, (speed <= 0 || speed > 1) ? 1 : speed);

    // Cursor blink timer
    if (cursorStayVisibleTimer > 0) {
        cursorStayVisibleTimer -= GetFrameTime();
    } else {
        cursorStayVisibleTimer = 0;
    }

    // Animate cursor blink
    cursorOpacity = sinPulse(1.5);

    // Calculate how many words will be in each line according to the available screen size
    std::vector<std::string> lines;
    std::string currentLine = "";
    std::string currentWord = "";

    for (int i = 0; i < context.sentence.size(); i++) {
        // Detect the end of a word
        if (context.sentence[i] == ' ' || i == (context.sentence.size() - 1)) {
            currentWord += context.sentence[i];

            // Calculate the width of the word
            float widthOfWord = currentWord.size() * sizeOfCharacter.x;

            float widthOfNewLine = widthOfWord + currentLine.size() * sizeOfCharacter.x;

            // Go to new line if word is overflowing
            if (widthOfNewLine > width-(PADDING*2)) {
                lines.push_back(currentLine);
                currentLine = "";
            }

            currentLine += currentWord;
            currentWord = "";
        } else {
            currentWord.push_back(context.sentence[i]);
        }
    }

    lines.push_back(currentLine);

    Rectangle textBox = {
        (float)(center.x - width/2.0),
        (float)((center.y - height/2.0) - 100),
        (float)width,
        (float)(height > 1 ? height : 1), // To fix a bug in Secissor Mode when it crashes when the height is < 1
    };

    if (textBox.y < 95) {
        textBox.y = 95;
    }

    // Begin Drawing sentence
    BeginScissorMode(textBox.x, textBox.y, textBox.width, textBox.height);

    float currentLineY = textBox.y - yOffset;
    int characterIndex = 0;
    int lineNumber = 1;

    for (auto& line : lines) {
        float widthOfLine = sizeOfCharacter.x * line.size();

        float currentLetterX = center.x - (widthOfLine/2);

        for (char& letter : line) {
            Color color = theme.text;

            if (context.input.size() > characterIndex) {
                // Check if the character is wrong
                if (letter == context.input[characterIndex]) {
                    color = theme.correct;
                } else {
                    color = theme.wrong;

                    // Draw underline if space
                    if (letter == ' ') {
                        DrawTextEx(context.fonts.typingTestFont.font, "_",
                                {currentLetterX, currentLineY}, context.fonts.typingTestFont.size,
                                1, color);

                    }
                }
            }

            // Draw Text
            DrawTextEx(context.fonts.typingTestFont.font, std::string(1, letter).c_str(),
                    {currentLetterX, currentLineY}, context.fonts.typingTestFont.size,
                    1, color);

            // Handle cursor
            if (characterIndex == context.input.size()) {
                // Set the offset to make the cursor at center
                newYOffset = lineNumber > 2 ? ((lineNumber-1) * sizeOfCharacter.y) - sizeOfCharacter.y : 1;
                newCursorPosition = { currentLetterX, currentLineY };

                Color cursorColor = theme.cursor;
                float blink = (cursorStayVisibleTimer != 0 )? 1 : cursorOpacity;

                // Draw Cursor
                if (blink > 0.5) {
                    BeginBlendMode(BLEND_SUBTRACT_COLORS);
                    switch (context.cursorStyle) {
                        case CursorStyle::BLOCK:
                            DrawRectangle(cursorPostion.x+1, cursorPostion.y,
                                    sizeOfCharacter.x, sizeOfCharacter.y,
                                    cursorColor);
                            // Make the color of the text inverted
                            //color = theme.background;

                            break;
                        case CursorStyle::LINE:
                            DrawRectangle(cursorPostion.x, cursorPostion.y,
                                    2, sizeOfCharacter.y,
                                    cursorColor);

                            break;
                        case CursorStyle::UNDERLINE:
                            DrawRectangle(cursorPostion.x+1, cursorPostion.y+sizeOfCharacter.y,
                                    sizeOfCharacter.x, 3,
                                    cursorColor);
                            break;
                    }
                    EndBlendMode();
                }
            }

            currentLetterX += sizeOfCharacter.x;
            characterIndex++;
        }

        currentLineY += sizeOfCharacter.y;
        lineNumber++;
    }

    EndScissorMode();

    // Draw Keybaord
    const int sizeOfKey = 35;
    const int margin = 5;
    sizeOfCharacter = MeasureTextEx(context.fonts.tinyFont.font, "a",
            context.fonts.tinyFont.size, 1);

    if (IsKeyPressed(KEY_BACKSPACE)) {
        cursorStayVisibleTimer = 1;
    }

    for (int i = 0; i < keyboard.size(); i++) {
        auto row = keyboard[i];
        int totalWidth = row[0] == ' ' ? 200 : (sizeOfKey * row.size()) + margin * (row.size()-1);
        Vector2 position;
        position.x = center.x - (totalWidth/2.0);
        position.y = (textBox.y + (sizeOfCharacter.y * 8)) + (sizeOfKey * i) + margin * i;

        for (auto key : row) {
            std::string c(1, key);
            Rectangle rect;
            rect.x = position.x;
            rect.y = position.y;
            rect.width = row[0] == ' ' ? 200 : sizeOfKey;
            rect.height = sizeOfKey;
            DrawRectangleRoundedLines(rect, 0.1, 5, 1, theme.text);
            Color color = theme.text;
            Vector2 keyPosition;
            keyPosition.x = (rect.x + (sizeOfKey/2.0)) - (sizeOfCharacter.x/2.0);
            keyPosition.y = (rect.y + (sizeOfKey/2.0)) - (sizeOfCharacter.y/2.0);

            if (IsKeyDown(toupper(key))) {
                BeginBlendMode(BLEND_SUBTRACT_COLORS);
                DrawRectangleRounded(rect, 0.1, 5, theme.cursor);
                EndBlendMode();
                color = theme.background;
                cursorStayVisibleTimer = 1;
            }

            drawMonospaceText(context.fonts.tinyFont.font, c.c_str(), keyPosition, context.fonts.tinyFont.size, color);
            position.x += sizeOfKey + margin;
        }
    }
}
