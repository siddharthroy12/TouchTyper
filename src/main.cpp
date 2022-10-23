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
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
#if defined(PLATFORM_WEB)
    InitWindow(browserWindowWidth(), browserWindowHeight(), PROJECT_NAME);
#else
    InitWindow(800, 500, PROJECT_NAME);
#endif
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitAudioDevice();

    context.load();

    restartTest(context, false);

#if defined(PLATFORM_WEB)
    std::cout << "Setting up emscripten loop" << std::endl;
    emscripten_set_main_loop(loop, 0, 1);
#else
    while (!WindowShouldClose()) {
        loop();
    }
#endif
    context.unload();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void loop() {
    context.screenHeight = getWindowHeight();
    context.screenWidth = getWindowWidth();

    Theme theme = context.themes[context.selectedTheme];

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
    ClearBackground(theme.background);

    header(context);


    if (context.currentScreen == Screen::TEST) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (context.soundOn) PlaySoundMulti(context.sounds.clickSound1);

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

            if (key != 0) {
                if (context.soundOn) PlaySoundMulti(context.sounds.clickSound1);
            }

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
    EndDrawing();
}
