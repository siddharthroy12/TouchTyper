#include "footer.hpp"
#include "helpers.hpp"
#include "constants.hpp"

// This will return the slected index
int optionSelectWidth = 200;
int optionPadding = 5;
int optionsContainerPadding = 10;

bool showThemesOptions = false;
bool showWordListOptions = false;

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
    DrawRectangleLinesEx(rect, 1, theme.text);
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
            DrawRectangleRec(optionRect, theme.text);
            color = theme.background;
        }

        startingPosition.y += optionPadding;
        drawMonospaceText(context.fonts.tinyFont.font, option, startingPosition, context.fonts.tinyFont.size, color);
        startingPosition.y += sizeOfCharacter.y + optionPadding;
    }

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
    DrawRectangleRounded(rec, 0.2, 5, theme.text);
    drawMonospaceText(context.fonts.tinyFont.font, "shift", position, context.fonts.tinyFont.size, theme.background);
    position.x += sizeOfCharacter.x * 10;
    rec.x = position.x-4;
    rec.width = (sizeOfCharacter.x * 5) + 8;
    DrawRectangleRounded(rec, 0.2, 5, theme.text);
    drawMonospaceText(context.fonts.tinyFont.font, "enter", position, context.fonts.tinyFont.size, theme.background);

    shortcut = "enter  -  new test";
    position.x  = getCenter(context.screenWidth, context.screenHeight).x - (sizeOfCharacter.x*shortcut.size())/2.0;
    position.y -= sizeOfCharacter.y + 10;
    drawMonospaceText(context.fonts.tinyFont.font, shortcut.c_str(), position, context.fonts.tinyFont.size, theme.text);
    rec.x = position.x-4;
    rec.y = position.y-2;
    rec.width = (sizeOfCharacter.x * 5) + 8;
    DrawRectangleRounded(rec, 0.2, 5, theme.text);
    drawMonospaceText(context.fonts.tinyFont.font, "enter", position, context.fonts.tinyFont.size, theme.background);

    // Draw options
    Rectangle themeOptionRect;
    themeOptionRect.x = bottomLeftPosition.x;
    themeOptionRect.y = bottomLeftPosition.y - sizeOfCharacter.y;
    themeOptionRect.width = 5 * sizeOfCharacter.x;
    themeOptionRect.height = sizeOfCharacter.y;
    position.x = themeOptionRect.x;
    position.y = themeOptionRect.y;
    Color color = theme.text;

    if (CheckCollisionPointRec(GetMousePosition(), themeOptionRect)) {
        context.mouseOnClickable  = true;
        color = theme.correct;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            showThemesOptions = !showThemesOptions;
        }
    }

    drawMonospaceText(context.fonts.tinyFont.font, "theme", position, context.fonts.tinyFont.size, color);

    std::vector<std::string> themeOptions;
    for (auto theme : context.themes) {
        themeOptions.push_back(theme.name);
    }

    if (showThemesOptions)  {
        int selected = optionSelect(context, themeOptions, context.selectedTheme);

        if (selected != -1) {
            context.selectedTheme = selected;
            showThemesOptions = false;
        }

        if (!CheckCollisionPointRec(GetMousePosition(), themeOptionRect)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showThemesOptions = false;
            }
        }
    }

    Rectangle wordListOptionRect;
    wordListOptionRect.x = themeOptionRect.x + themeOptionRect.width + sizeOfCharacter.x;
    wordListOptionRect.y = bottomLeftPosition.y - sizeOfCharacter.y;
    wordListOptionRect.width = 9 * sizeOfCharacter.x;
    wordListOptionRect.height = sizeOfCharacter.y;
    position.x = wordListOptionRect.x;
    position.y = wordListOptionRect.y;
    color = theme.text;

    if (CheckCollisionPointRec(GetMousePosition(), wordListOptionRect)) {
        context.mouseOnClickable  = true;
        color = theme.correct;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            showWordListOptions = !showWordListOptions;
        }
    }

    drawMonospaceText(context.fonts.tinyFont.font, "word list", position, context.fonts.tinyFont.size, color);

    std::vector<std::string> wordListOptions;

    for (auto wordList : context.wordsLists) {
        wordListOptions.push_back(wordList.name);
    }

    if (showWordListOptions)  {
        int selected = optionSelect(context, wordListOptions, context.selectedWordList);

        if (selected != -1) {
            context.selectedWordList = selected;
            showWordListOptions = false;
            restartTest(context, false);
        }

        if (!CheckCollisionPointRec(GetMousePosition(), wordListOptionRect)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showWordListOptions = false;
            }
        }
    }
}
