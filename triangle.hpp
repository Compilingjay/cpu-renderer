#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec.hpp"

#include <array>
#include <cstdint>

struct Triangle {
    public:
        std::array<Vec2, 3> points;
        double avg_depth;
        uint32_t color;
};

#endif