#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec2.h"

#include <array>

struct Triangle {
    public:
        std::array<Vec2, 3> points;
};

#endif