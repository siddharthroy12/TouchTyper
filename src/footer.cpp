#include "footer.hpp"
#include "helpers.hpp"
#include "constants.hpp"

void footer(Context &context) {
    Vector2 sizeOfCharacter = MeasureTextEx(context.fonts.tinyFont.font, "a",
            context.fonts.tinyFont.size, 1);

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
            context.theme.text);

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
    drawMonospaceText(context.fonts.tinyFont.font, shortcut.c_str(), position, context.fonts.tinyFont.size, context.theme.text);
    DrawRectangleRounded(rec, 0.2, 5, context.theme.text);
    drawMonospaceText(context.fonts.tinyFont.font, "shift", position, context.fonts.tinyFont.size, context.theme.background);
    position.x += sizeOfCharacter.x * 10;
    rec.x = position.x-4;
    rec.width = (sizeOfCharacter.x * 5) + 8;
    DrawRectangleRounded(rec, 0.2, 5, context.theme.text);
    drawMonospaceText(context.fonts.tinyFont.font, "enter", position, context.fonts.tinyFont.size, context.theme.background);

    shortcut = "enter  -  new test";
    position.x  = getCenter(context.screenWidth, context.screenHeight).x - (sizeOfCharacter.x*shortcut.size())/2.0;
    position.y -= sizeOfCharacter.y + 10;
    drawMonospaceText(context.fonts.tinyFont.font, shortcut.c_str(), position, context.fonts.tinyFont.size, context.theme.text);
    rec.x = position.x-4;
    rec.y = position.y-2;
    rec.width = (sizeOfCharacter.x * 5) + 8;
    DrawRectangleRounded(rec, 0.2, 5, context.theme.text);
    drawMonospaceText(context.fonts.tinyFont.font, "enter", position, context.fonts.tinyFont.size, context.theme.background);
    EndDrawing();
}
