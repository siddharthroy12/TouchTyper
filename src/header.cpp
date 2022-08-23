#include "header.hpp"
#include "constants.hpp"
#include "helpers.hpp"

void header(Context &context) {
    int width = std::min(context.screenWidth-(PADDING*2), MAX_WIDTH);

    // Center of the screen
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);

    Vector2 topLeftPosition;
    topLeftPosition.x = center.x - width/2.0;
    topLeftPosition.y = PADDING;

    Vector2 topRightPosition;
    topRightPosition.x = center.x + width/2.0;
    topRightPosition.y = PADDING;

    DrawTextEx(context.titleFontData.font,
               "TouchTyper", topLeftPosition,
               context.titleFontData.size, 1, WHITE);
}
