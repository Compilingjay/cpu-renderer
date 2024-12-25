#ifndef VERTEX_H
#define VERTEX_H

#include "vec3.h"

#include <vector>

template <SizedNumeric T>
std::vector<Vec3<T>> get_9x9x9_vectors() {
    std::vector<Vec3<T>> vec(9*9*9, Vec3<T>{0.0, 0.0, 0.0});

    int i = 0;
    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= 1; z += 0.25) {
                vec[i++] = Vec3<T>{x, y, z};
            }
        }
    }

    return vec;
}

#endif