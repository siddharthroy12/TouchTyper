#include "../libs/raylib/src/raylib.h"
#include "Context.hpp"
#include "Theme.hpp"
#include "typingTest.hpp"
#include <iostream>

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
    context.font = LoadFontEx("assets/mononoki-Regular.ttf", 28, nullptr, 0);

    while (!WindowShouldClose()) {
        context.screenHeight = GetScreenHeight();
        context.screenWidth = GetScreenWidth();

        int key = GetCharPressed();

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (context.input.size()) {
                // Delete whole word
                if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
                    while (context.input.size() && context.input[context.input.size()-1] == ' ' ) {
                        context.input.pop_back();
                    }

                    while ((context.input[context.input.size()-1] != ' ') && context.input.size()) {
                        context.input.pop_back();
                    }
                } else {
                    context.input.pop_back();
                }
            }
        }

        if (key && (context.input.size() < context.sentence.size())) {
            context.input += key;
        }

        BeginDrawing();
        ClearBackground({12, 13, 17, 255});
        typingTest(context);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
