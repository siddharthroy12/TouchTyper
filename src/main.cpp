#include "../libs/raylib/src/raylib.h"
#include "Context.hpp"
#include "constants.hpp"
#include "typingTest.hpp"
#include "header.hpp"
#include "helpers.hpp"
#include "result.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>

int main(void) {
    InitWindow(800, 450, PROJECT_NAME);
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    //SetTargetFPS(60);

    Context context;
    context.theme.background = {12, 13, 17, 255};
    context.theme.text = {69, 72, 100, 255};
    context.theme.cursor = {153, 214, 234, 255};
    context.theme.wrong = RED;
    context.theme.correct = {126, 186, 181, 255};
    context.theme.highlight = RAYWHITE;
    context.sentence = "end for also world better right now if you can me do that what now for etc hello then";
    context.fonts.typingTestFont.size = 32;
    context.fonts.typingTestFont.font = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf",
            context.fonts.typingTestFont.size, nullptr, 0);
    context.fonts.titleFont.size = 40;
    context.fonts.titleFont.font = LoadFontEx("assets/fonts/LexendDeca-Regular.ttf",
            context.fonts.titleFont.size, nullptr, 0);
    context.fonts.tinyFont.size = 18;
    context.fonts.tinyFont.font = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf",
            context.fonts.tinyFont.size, nullptr, 0);
    context.fonts.bigFont.size = 90;
    context.fonts.bigFont.font = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf",
            context.fonts.bigFont.size, nullptr, 0);

    if (!getFileContent("assets/word_lists/english_1k.txt", context.words)) {
        return 1;
    }

    restartTest(context, false);

    while (!WindowShouldClose()) {
        context.screenHeight = GetScreenHeight();
        context.screenWidth = GetScreenWidth();

        int key = GetCharPressed();

        if (context.mouseOnClickable) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        context.mouseOnClickable = false;

        if (IsKeyPressed(KEY_ENTER)) {
            restartTest(context, IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT));
        }

        if (context.testRunning) {
            int time = (int)((GetTime() - context.testStartTime));
            if (time >= context.testSettings.testModeAmounts[context.testSettings.selectedAmount] &&
                    context.testSettings.testMode == TestMode::TIME) {
                endTest(context);
            }
        }

        BeginDrawing();
        ClearBackground(context.theme.background);
        header(context);

        if (context.currentScreen == Screen::TEST) {
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (context.input.size()) {

                    // CTRL + Backspace
                    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
                        while (context.input.size() && context.input[context.input.size()-1] == ' ' ) {
                            context.input.pop_back();
                        }

                        while ((context.input[context.input.size()-1] != ' ') && context.input.size()) {
                            context.input.pop_back();
                        }
                    } else { // Normal Backspace
                        context.input.pop_back();
                    }
                }
            }

            if (key && (context.input.size() < context.sentence.size())) {
                context.input += key;

                if (context.input.size() == 1 && !context.testRunning) {
                    context.testRunning = true;
                    context.testStartTime = GetTime();
                }

                // Once the sentence is complete end the test
                if (context.testRunning && context.input.size() == context.sentence.size()) {
                    endTest(context);
                }

                // Calculate correct and incorrect typed letters and add more words as we type
                if (context.input.size() > context.furthestVisitedIndex) {
                    if (context.input[context.input.size()-1] != context.sentence[context.input.size()-1]) {
                        context.incorrecLetters++;
                    } else {
                        context.correctLetters++;
                    }

                    if (context.testSettings.testMode == TestMode::TIME) {
                        if (context.sentence[context.input.size()-1] == ' ') {
                            context.sentence += ' ';
                            context.sentence += generateSentence(context, 1);
                        }
                    }
                }

                context.furthestVisitedIndex = std::max(context.furthestVisitedIndex, (int)context.input.size());
            }

            // Calculate score
            if (context.testRunning && (GetTime()-context.testStartTime) > 3) {
                double wpm = (context.correctLetters) * (60 / (GetTime() - context.testStartTime)) / 5.0;
                double raw = (context.correctLetters+context.incorrecLetters) * (60 / (GetTime() - context.testStartTime)) / 5.0;
                context.wpm = wpm;
                context.raw = raw;
                context.accuracy = ((float)(context.correctLetters) / (context.correctLetters + context.incorrecLetters)) * 100;
            }

            typingTest(context);

        } else if (context.currentScreen == Screen::RESULT) {
            result(context);
        }

        // Draw shortcut
        std::string shortcut = "enter  - new test";
        Vector2 sizeOfCharacter = MeasureTextEx(context.fonts.tinyFont.font, "a",
                context.fonts.tinyFont.size, 1);

        Vector2 position = getCenter(context.screenWidth, context.screenHeight);
        position.y = context.screenHeight - (PADDING + sizeOfCharacter.y);
        position.x -= (sizeOfCharacter.x*shortcut.size())/2.0;
        Rectangle rec;
        rec.x = position.x-4;
        rec.y = position.y-2;
        rec.height = (sizeOfCharacter.y) + 4;
        rec.width = (sizeOfCharacter.x * 5) + 8;
        drawMonospaceText(context.fonts.tinyFont.font, shortcut.c_str(), position, context.fonts.tinyFont.size, context.theme.text);
        DrawRectangleRounded(rec, 0.2, 5, context.theme.text);
        drawMonospaceText(context.fonts.tinyFont.font, "enter", position, context.fonts.tinyFont.size, context.theme.background);

        shortcut = "shift  +  enter  -  repeat test";
        position.x  = getCenter(context.screenWidth, context.screenHeight).x - (sizeOfCharacter.x*shortcut.size())/2.0;
        position.y -= sizeOfCharacter.y + 10;
        drawMonospaceText(context.fonts.tinyFont.font, shortcut.c_str(), position, context.fonts.tinyFont.size, context.theme.text);
        rec.x = position.x-4;
        rec.y = position.y-2;
        rec.width = (sizeOfCharacter.x * 5) + 8;
        DrawRectangleRounded(rec, 0.2, 5, context.theme.text);
        drawMonospaceText(context.fonts.tinyFont.font, "shift", position, context.fonts.tinyFont.size, context.theme.background);
        position.x += sizeOfCharacter.x * 10;
        rec.x = position.x-4;
        rec.width = (sizeOfCharacter.x * 5) + 8;
        DrawRectangleRounded(rec, 0.2, 5, context.theme.text);
        drawMonospaceText(context.fonts.tinyFont.font, "enter", position, context.fonts.tinyFont.size, context.theme.background);


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
