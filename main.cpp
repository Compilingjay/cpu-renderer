#include "SDL3/SDL_log.h"
#include "render_state.h"

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

    RenderState<float>rs;
    try {
        rs = RenderState<float>(argv[1], argv[2]);
    } catch (...) {
        return 1;
    }
    
    rs.run();
    return 0;
}

void on_exit() {
    SDL_Quit();
    SDL_Log("Successfully exited.");
}