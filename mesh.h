#ifndef MESH_H
#define MESH_H

#include "string_utils.h"
#include "vec3.h"

#include <stdio.h>
#include <fstream>
#include <format>
#include <string>
#include <vector>

template <SizedNumeric T>
struct Mesh {
    public:
        std::vector<Vec3<T>> vertex_buf;
        std::vector<Vec3<int>> faces_buf;
        std::vector<Vec3<int>> texture_buf;
        std::vector<Vec3<T>> normal_buf;
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

template <SizedNumeric T>
Mesh<T> get_mesh_from_obj_file(std::string_view file_path) {
    std::ifstream ifile = std::ifstream(file_path.data());
    if (!ifile.is_open()) {
        throw std::format("failed to open filepath: {}", file_path.data());
    }

    Mesh<T> mesh;
    Vec3<T> vt;
    Vec3<int> vi;
    std::string line;

    while (std::getline(ifile, line)) {
        auto split_string = split(line, " ", 1);
        if (split_string.size() < 2) {
            continue;
        }
        
        if (split_string[0] == "v") { // note string split removes space and we can check for this without fear of vt, vn and not v
            split_string = split(split_string[1], " ");
            mesh.vertex_buf.push_back( {
                static_cast<T>(std::stod(split_string[0])),
                static_cast<T>(std::stod(split_string[1])),
                static_cast<T>(std::stod(split_string[2])) }
            );
            // mesh.vertex_buf.push_back(v);
        } else if (split_string[0] == "vt") {
            // TODO
        } else if (split_string[0] == "vn") {
            // TODO
        } else if (split_string[0] == "vp") {
            // TODO
        } else if (split_string[0] == "vt") {
            // TODO
        } else if (split_string[0] == "f") {
            split_string = split(split_string[1], { " ", "/" } );
            mesh.faces_buf.push_back({ std::stoi(split_string[0]), std::stoi(split_string[3]), std::stoi(split_string[6]) } );
            mesh.texture_buf.push_back({ std::stoi(split_string[1]), std::stoi(split_string[4]), std::stoi(split_string[7]) } );
            mesh.normal_buf.push_back( {
                static_cast<T>(std::stod(split_string[2])), static_cast<T>(std::stod(split_string[5])), static_cast<T>(std::stod(split_string[8]))
            });
        } else if (split_string[0] == "l") {
            // TODO
        } else if (split_string[0] == "s") {
            // TODO
        }
    }

    return mesh;
}

#endif