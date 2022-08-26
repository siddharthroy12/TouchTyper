#include "../libs/raylib/src/raylib.h"
#include "Context.hpp"
#include "constants.hpp"
#include "typingTest.hpp"
#include "header.hpp"
#include "helpers.hpp"
#include "result.hpp"
#include "footer.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_set>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
EM_JS(int, canvasGetWidth, (), {
  return document.getElementById("canvas").clientWidth;
});

EM_JS(int, canvasGetHeight, (), {
  return document.getElementById("canvas").clientHeight;
});
EM_JS(int, browserWindowWidth, (), {
    return window.innerWidth;
});
EM_JS(int, browserWindowHeight, (), {
    return window.innerHeight;
});
#endif


int getWindowWidth() {
#if defined(PLATFORM_WEB)
    return canvasGetWidth();
#else
    if (IsWindowFullscreen()) {
        return GetMonitorWidth(GetCurrentMonitor());
    }
    else {
        return GetScreenWidth();
    }
#endif
}

int getWindowHeight() {
#if defined(PLATFORM_WEB)
    return canvasGetHeight();
#else
    if (IsWindowFullscreen()) {
        return GetMonitorHeight(GetCurrentMonitor());
    }
    else {
        return GetScreenHeight();
    }
#endif
}

Context context;

void loop();

int main(void) {
#if defined(PLATFORM_WEB)
    InitWindow(browserWindowWidth(), browserWindowHeight(), PROJECT_NAME);
#else
    InitWindow(800, 450, PROJECT_NAME);
#endif
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    //SetTargetFPS(60);

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

#if defined(PLATFORM_WEB)
    std::cout << "Setting up emscripten loop" << std::endl;
    emscripten_set_main_loop(loop, 0, 1);
#else
    while (!WindowShouldClose()) {
        loop();
    }
#endif

    CloseWindow();

    return 0;
}

void loop() {
    context.screenHeight = getWindowHeight();
    context.screenWidth = getWindowWidth();

#if defined(PLATFORM_WEB)
    static int old_w=0,old_h=0;

    int w = canvasGetWidth();
    int h = canvasGetHeight();
    if(w!=old_w || h!=old_h){ SetWindowSize(w,h); }
#else
    if (IsKeyPressed(KEY_F11)) {
        // see what display we are on right now

        if (IsWindowFullscreen())
        {
            SetWindowSize(context.screenWidth, context.screenHeight);
            ToggleFullscreen();
        }
        else
        {

            ToggleFullscreen();
            int monitor = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        }

    }
#endif

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
        footer(context);
    }
