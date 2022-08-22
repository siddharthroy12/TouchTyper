#ifndef HELPERS_HPP
#define HELPERS_HPP
#include "../libs/raylib/src/raylib.h"

inline Vector2 getCenter(int width, int height) {
    Vector2 result;
    result.x = width/2.0;
    result.y = height/2.0;
    return result;
}

#endif
