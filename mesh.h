#ifndef MESH_H
#define MESH_H

#include "vec3.h"

#include <vector>

template <SizedNumeric T>
struct Mesh {
    public:
        std::vector<Vec3<T>> vertex_buf;
        std::vector<Vec3<T>> faces_buf;
        Vec3<T> rot;
};

template <SizedNumeric T>
std::vector<Vec3<T>> get_cube_vertices() {
    return {
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, -1.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f},
        {-1.0f, -1.0f, 1.0f}
    };
}

template <SizedNumeric T>
std::vector<Vec3<T>> get_cube_faces() {
    return {
        {0, 1, 2}, {0, 2, 3}, // front
        {3, 2, 4}, {3, 4, 5}, // right
        {5, 4, 6}, {5, 6, 7}, // back
        {7, 6, 1}, {7, 1, 0}, // left
        {1, 6, 4}, {1, 4, 2}, // top
        {5, 7, 0}, {5, 0, 3}  // bottom
    };
}

#endif