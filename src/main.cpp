#include "../libs/raylib/src/raylib.h"
#include "Context.hpp"
#include "constants.hpp"
#include "typingTest.hpp"
#include "header.hpp"
#include "helpers.hpp"
#include "result.hpp"
#include <iostream>
#include <vector>
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

    std::unordered_set<int> visitedIndexes;
    int incorrecLetters = 0;
    int correctLetters = 0;

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
            restartTest(context);
        }

        BeginDrawing();
        ClearBackground({12, 13, 17, 255});
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

                if (context.input.size() == 1) {
                    context.testRunning = true;
                    context.testStartTime = getTimeInMin();
                }

                if (context.testRunning && context.input.size() == context.sentence.size()) {
                    context.testRunning = false;
                    context.currentScreen = Screen::RESULT;
                    context.testEndTime = getTimeInMin();
                }

                // Calculate correct and incorrect typed letters
                if (visitedIndexes.find(context.input.size()-1) == visitedIndexes.end()) {
                    if (context.input[context.input.size()-1] != context.sentence[context.input.size()-1]) {
                        incorrecLetters++;
                    } else {
                        correctLetters++;
                    }
                }
                visitedIndexes.insert(context.input.size()-1);
            }

            // Calculate score
            if (context.testRunning && (getTimeInMin()-context.testStartTime) > 0.03) {
                double cpm = correctLetters / (getTimeInMin() - context.testStartTime);
                double wpm = (correctLetters/5.0) / (getTimeInMin() - context.testStartTime);
                double raw = ((correctLetters+incorrecLetters)/5.0) / (getTimeInMin() - context.testStartTime);
                context.cpm = cpm;
                context.wpm = wpm;
                context.raw = raw;
                context.accuracy = ((float)(correctLetters) / (correctLetters + incorrecLetters)) * 100;
            }

            typingTest(context);

        } else if (context.currentScreen == Screen::RESULT) {
            result(context);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
