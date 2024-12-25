#include "template_concepts.h"
#include "vec2.h"
#include "vec3.h"

#include <array>

template <SizedNumeric T>
class Triangle {
    std::array<Vec2<T>, 3> points;
};