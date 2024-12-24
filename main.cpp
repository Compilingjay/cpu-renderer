#include "camera.h"
#include "render_state.h"
#include "3d.h"
#include "vec3.h"

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

    auto vertex_buf = get_9x9x9_vectors<float>();
    auto camera = Camera { Vec3 {0.0f, 0.0f, -5.0f}, Vec3<float>{ 0.0f, 0.0f, 0.0f }, 640.0f };

    rs.run(vertex_buf, camera);
    return 0;
}

void on_exit() {
    SDL_Quit();
    SDL_Log("Successfully exited.");
}