#include "mesh.h"

Mesh get_mesh_from_obj_file(std::string file_path) {
    std::ifstream ifile = std::ifstream(file_path);
    if (!ifile.is_open()) {
        throw std::format("failed to open filepath: {}", file_path.data());
    }

    Mesh mesh;
    std::string line;

    while (std::getline(ifile, line)) {
        auto split_string = split(line, " ", 1);
        if (split_string.size() < 2) {
            continue;
        }
        
        if (split_string[0] == "v") {
            split_string = split(split_string[1], " ");
            mesh.vertices.push_back( Vec3(std::stod(split_string[0]), std::stod(split_string[1]), std::stod(split_string[2])) );
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
            mesh.faces.push_back( std::array<int, 3>{ std::stoi(split_string[0]), std::stoi(split_string[3]), std::stoi(split_string[6]) } );
            mesh.textures.push_back( std::array<int, 3>{std::stoi(split_string[1]), std::stoi(split_string[4]), std::stoi(split_string[7]) } );
            mesh.normals.push_back( Vec3(std::stod(split_string[2]), std::stod(split_string[5]), std::stod(split_string[8])) );
        } else if (split_string[0] == "l") {
            // TODO
        } else if (split_string[0] == "s") {
            // TODO
        }
    }

    return mesh;
}