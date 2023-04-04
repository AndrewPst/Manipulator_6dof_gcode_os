#pragma once

#define _USE_MATH_DEFINES
#include "math.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

inline constexpr double radToDeg(double v)
{
    return v * 180.0 / M_PI;
}

inline constexpr double degToRad(double v)
{
    return v / 180.0 * M_PI;
}