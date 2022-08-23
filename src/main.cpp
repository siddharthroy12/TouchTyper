#include "../libs/raylib/src/raylib.h"
#include "Context.hpp"
#include "Theme.hpp"
#include "constants.hpp"
#include "typingTest.hpp"
#include "header.hpp"
#include <iostream>
#include <vector>

char getInputCharacter() {
    int key = GetKeyPressed();
    std::cout << key << std::endl;
    return 0;
}

int main(void) {
    InitWindow(800, 450, "TouchTyper");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);
    Context context;
    context.theme.background = {12, 13, 17, 255};
    context.theme.text = {69, 72, 100, 255};
    context.theme.cursor = {153, 214, 234, 255};
    context.theme.wrong = RED;
    context.theme.correct = {126, 186, 181, 255};
    context.sentence = "end for also world better right now if you can me do that what now for etc hello then life than when where for arrow node python c++ this dotnet dot com google facebook amazon netflix reddit";
    context.typingTestFontData.font = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf", 32, nullptr, 0);
    context.typingTestFontData.size = 32;
    context.titleFontData.font = LoadFontEx("assets/fonts/LexendDeca-Regular.ttf", 40, nullptr, 0);
    context.titleFontData.size = 40;
    std::vector<double> deltas(context.sentence.size());
    double previousTypedTime = GetTime(); // In seconds
    int cpm;
    int typed = 0;

    while (!WindowShouldClose()) {
        context.screenHeight = GetScreenHeight();
        context.screenWidth = GetScreenWidth();

        int key = GetCharPressed();

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

            // Calculate delta time for each character
            if (deltas[context.input.size()-1] == 0) {
                deltas[context.input.size()-1] = GetTime() - previousTypedTime;
                typed++;

                if (typed == 1) {
                    previousTypedTime = GetTime();
                }
            }
            previousTypedTime = GetTime();
        }

        double sum = 0;
        for (auto el : deltas) {
            sum += el;
        }

        // Calculate average wpm
        std::cout << "WPM: " << (((1 / (sum / typed)) * 60) / 5) << std::endl;

        BeginDrawing();
        ClearBackground({12, 13, 17, 255});
        header(context);
        typingTest(context);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
