#include "result.hpp"
#include "constants.hpp"
#include "helpers.hpp"
#include <vector>

void result(Context &context) {
    Vector2 sizeOfCharacter = MeasureTextEx(context.fonts.bigFont.font, "a",
                                            context.fonts.bigFont.size, 1);

    // To make it responsive
    int width = std::min(context.screenWidth-(PADDING*2), MAX_WIDTH);

    // Center of the screen
    Vector2 center = getCenter(context.screenWidth, context.screenHeight);

    std::vector<std::vector<std::string>> scores = {
        {"wpm", TextFormat("%d", context.wpm)},
        {"raw", TextFormat("%d", context.raw)},
        {"acc", TextFormat("%d", context.accuracy)},
        {"time", TextFormat("%ds", (int)((context.testEndTime - context.testStartTime)))},
    };

    float totalWidth = 0;

    for (auto score : scores) {
        totalWidth += score[1].size() * sizeOfCharacter.x;
    }

    totalWidth += (scores.size() - 1) * sizeOfCharacter.x;

    Vector2 position;
    position.x = center.x - ((totalWidth)/2.0);
    position.y = center.y - (sizeOfCharacter.y/2.0);

    for (auto score : scores) {
        drawMonospaceText(context.fonts.bigFont.font, score[1], position, context.fonts.bigFont.size, context.theme.correct);
        Vector2 nPosition = position;
        nPosition.y -= context.fonts.typingTestFont.size/2.0;
        drawMonospaceText(context.fonts.typingTestFont.font, score[0], nPosition, context.fonts.typingTestFont.size, context.theme.text);
        position.x += (score[1].size() * sizeOfCharacter.x) + sizeOfCharacter.x;
    }
}
