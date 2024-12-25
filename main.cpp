#include "camera.h"
#include "mesh.h"
#include "render_state.h"
#include "vertex.h"

void on_exit();

int main(int argc, const char* argv[]) {
    int success = atexit(on_exit);
    if (success != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "atexit: %s", SDL_GetError());
        return 1;
    }

    RenderState rs;
    try {
        rs = RenderState(argv[1]);
    } catch (...) {
        return 1;
    }

    // auto vertex_buf = get_9x9x9_vectors<float>();
    auto vertex_buf = get_cube_vertices<float>();
    auto faces_buf = get_cube_faces<float>();
    auto camera = Camera { { 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, 640.0f };

    rs.run(vertex_buf, faces_buf, camera);
    return 0;
}

void on_exit() {
    SDL_Quit();
    SDL_Log("Successfully exited.");
}