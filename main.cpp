#include "renderer.h"

void on_exit();

int main(int argc, const char* argv[]) {
    if (argc < 3) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "usage: ./renderer_3d <window_name> <path_to_obj_file>");
        return 1;
    }
    int success = atexit(on_exit);
    if (success != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "atexit: %s", SDL_GetError());
        return 1;
    }

    Renderer rs;
    std::string window_name { argv[1] };
    std::string mesh_path { argv[2] };
    try {
        rs.initialize(window_name, mesh_path);
    } catch (...) {
        return 1;
    }

    int debug_i;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();
    
    while (true) {
        if (!rs.process_input()) {
            current_time = std::chrono::high_resolution_clock::now();
            std::cout << "Program has been running for " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() << " seconds, i = " << debug_i << std::endl;
            return 0;
        }

        rs.update();
        rs.render();
        ++debug_i;
    }
}

void on_exit() {
    SDL_Quit();
    SDL_Log("Successfully exited.");
}