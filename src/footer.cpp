#include "footer.hpp"
#include "Context.hpp"
#include "helpers.hpp"
#include "constants.hpp"

// This will return the slected index
int optionSelectWidth = 200;
int optionPadding = 5;
int optionsContainerPadding = 10;

bool showThemesOptions = false;
bool showWordListOptions = false;
bool showCursorOptions = false;

int optionSelect(Context &context, std::vector<std::string> &options, int selected) {
    Vector2 sizeOfCharacter = MeasureTextEx(context.fonts.tinyFont.font, "a",
            context.fonts.tinyFont.size, 1);

    Theme theme = context.themes[context.selectedTheme];
    int height = options.size() * (sizeOfCharacter.y+(optionPadding * 2));
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);
    Rectangle rect;
    rect.height = height + (optionsContainerPadding*2);
    rect.width = optionSelectWidth;
    rect.x = center.x - (rect.width/2.0);
    rect.y = center.y - (rect.height/2.0);

    DrawRectangle(0, 0, context.screenWidth, context.screenHeight, {0, 0, 0, 100});
    DrawRectangleRec(rect, theme.background);
    Vector2 startingPosition;
    startingPosition.x = rect.x + optionsContainerPadding;
    startingPosition.y = rect.y + optionsContainerPadding;

    for (int i = 0; i < options.size(); i++) {
        auto option = options[i];
        Rectangle optionRect;
        optionRect.x = startingPosition.x - optionsContainerPadding;
        optionRect.y = startingPosition.y;
        optionRect.width = rect.width;
        optionRect.height = sizeOfCharacter.y + (optionPadding*2);
        Color color = theme.text;

        bool mouseOnOption = CheckCollisionPointRec(GetMousePosition(), optionRect);

        if (mouseOnOption) {
            context.mouseOnClickable = true;
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                return i;
            }
        }

        if (i == selected || mouseOnOption) {
            BeginBlendMode(BLEND_SUBTRACT_COLORS);
            DrawRectangleRec(optionRect, theme.cursor);
            EndBlendMode();
            color = theme.background;
        }

        startingPosition.y += optionPadding;
        drawMonospaceText(context.fonts.tinyFont.font, option, startingPosition, context.fonts.tinyFont.size, color);
        startingPosition.y += sizeOfCharacter.y + optionPadding;
    }

    DrawRectangleLinesEx(rect, 1, theme.correct);


    return -1;
}


void footer(Context &context) {
    Vector2 sizeOfCharacter = MeasureTextEx(context.fonts.tinyFont.font, "a",
            context.fonts.tinyFont.size, 1);
    Theme theme = context.themes[context.selectedTheme];

    int width = std::min(context.screenWidth-(PADDING*2), MAX_WIDTH);

    // Center of the screen
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);

    Vector2 bottomLeftPosition;
    bottomLeftPosition.x = center.x - width/2.0;
    bottomLeftPosition.y = context.screenHeight-PADDING;

    Vector2 bottomRightPosition;
    bottomRightPosition.x = center.x + width/2.0;
    bottomRightPosition.y = context.screenHeight-PADDING;

    Vector2 versionPosition = bottomRightPosition;
    versionPosition.y -= sizeOfCharacter.y;
    versionPosition.x -= sizeof(VERSION) * sizeOfCharacter.x;

    // Draw version
    drawMonospaceText(context.fonts.tinyFont.font,
            VERSION,
            versionPosition,
            context.fonts.tinyFont.size,
            theme.text);

    // Draw shortcut
    std::string shortcut = "shift  +  enter  - repeat test";
    Vector2 position = center;
    position.y = context.screenHeight - (PADDING + sizeOfCharacter.y);
    position.x -= (sizeOfCharacter.x*shortcut.size())/2.0;
    Rectangle rec;
    rec.x = position.x-4;
    rec.y = position.y-2;
    rec.height = (sizeOfCharacter.y) + 4;
    rec.width = (sizeOfCharacter.x * 5) + 8;
    drawMonospaceText(context.fonts.tinyFont.font, shortcut.c_str(), position, context.fonts.tinyFont.size, theme.text);
    DrawRectangleRoundedLines(rec, 0.1, 5, 1, theme.text);
    position.x += sizeOfCharacter.x * 10;
    rec.x = position.x-4;
    rec.width = (sizeOfCharacter.x * 5) + 8;
    DrawRectangleRoundedLines(rec, 0.1, 5, 1, theme.text);

    shortcut = "enter  -  new test";
    position.x  = getCenter(context.screenWidth, context.screenHeight).x - (sizeOfCharacter.x*shortcut.size())/2.0;
    position.y -= sizeOfCharacter.y + 10;
    drawMonospaceText(context.fonts.tinyFont.font, shortcut.c_str(), position, context.fonts.tinyFont.size, theme.text);
    rec.x = position.x-4;
    rec.y = position.y-2;
    rec.width = (sizeOfCharacter.x * 5) + 8;
    DrawRectangleRoundedLines(rec, 0.1, 5, 1, theme.text);

    // Draw options
    Vector2 themePosition = {
        bottomLeftPosition.x,
        bottomRightPosition.y - sizeOfCharacter.y
    };

    if (showThemesOptions)  {
        std::vector<std::string> themeOptions;
        for (auto theme : context.themes) {
            themeOptions.push_back(theme.name);
        }

        int selected = optionSelect(context, themeOptions, context.selectedTheme);

        if (selected != -1) {
            context.selectedTheme = selected;
            showThemesOptions = false;
        }
    }

    if (textButton(context, themePosition, "theme")) {
        showThemesOptions = !showThemesOptions;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showThemesOptions = false;
    }

    Vector2 worldlistPosition = {
        themePosition.x + sizeOfCharacter.x * 6,
        themePosition.y
    };

    if (showWordListOptions)  {
        std::vector<std::string> wordListOptions;

        for (auto wordList : context.wordsLists) {
            wordListOptions.push_back(wordList.name);
        }

        int selected = optionSelect(context, wordListOptions, context.selectedWordList);

        if (selected != -1) {
            context.selectedWordList = selected;
            showWordListOptions = false;
            restartTest(context, false);
        }
    }

    if (textButton(context, worldlistPosition, "word list")) {
        showWordListOptions = !showWordListOptions;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        showWordListOptions = false;
    }

    Vector2 cursorPosition = {
        bottomRightPosition.x - (sizeOfCharacter.x * (sizeof(VERSION) + 7)),
        bottomRightPosition.y - sizeOfCharacter.y
    };

    if (showCursorOptions) {
        std::vector<std::string> cursorOptions = {"Block", "Line", "Underline"};
        int selected = optionSelect(context, cursorOptions, (int)context.cursorStyle);

        if (selected != -1) {
            context.cursorStyle = (CursorStyle)selected;
        }
    }

    if (textButton(context, cursorPosition, "cursor")) {
        showCursorOptions = !showCursorOptions;
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showCursorOptions = false;
    }

    Vector2 soundPosition {
        cursorPosition.x - sizeOfCharacter.x * (context.soundOn ? 9 : 10),
            cursorPosition.y
    };

    if (textButton(context, soundPosition, context.soundOn ? "sound on" : "sound off")) {
        context.soundOn = !context.soundOn;
    }
}
