#ifndef MESH_H
#define MESH_H

#include "string_utils.hpp"
#include "vec.hpp"

#include <SDL3/SDL.h>

#include <array>
#include <fstream>
#include <format>

struct Mesh {
    public:
        std::vector<Vec3> vertices;
        std::vector<std::array<int, 3>> faces;
        std::vector<std::array<int, 3>> textures;
        std::vector<Vec3> normals;
        Vec3 rot;
};

Mesh get_mesh_from_obj_file(std::string file_path);

#endif