#include "../libs/raylib/src/raylib.h"
#include "Context.hpp"
#include "constants.hpp"
#include "typingTest.hpp"
#include "header.hpp"
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

double getTimeInMin() {
    return GetTime()/60;
}

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
    context.sentence = "end for also world better right now if you can me do that what now for etc hello then life than when where for arrow node python c++ this dotnet dot com google facebook amazon netflix reddit";
    context.fonts.typingTestFont.size = 32;
    context.fonts.typingTestFont.font = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf",
                                                   context.fonts.typingTestFont.size, nullptr, 0);
    context.fonts.titleFont.size = 40;
    context.fonts.titleFont.font = LoadFontEx("assets/fonts/LexendDeca-Regular.ttf",
                                              context.fonts.titleFont.size, nullptr, 0);
    context.fonts.tinyFont.size = 18;
    context.fonts.tinyFont.font = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf",
                                             context.fonts.tinyFont.size, nullptr, 0);
    std::unordered_set<int> visitedIndexes;
    double testStartTime = 0; // In Minute
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
                testStartTime = getTimeInMin();
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

        // Calculate wpm
        double cpm = correctLetters / (getTimeInMin() - testStartTime);
        double wpm = (correctLetters/5.0) / (getTimeInMin() - testStartTime);

        context.cpm = cpm;
        context.wpm = wpm;

        BeginDrawing();
        ClearBackground({12, 13, 17, 255});
        header(context);
        typingTest(context);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
