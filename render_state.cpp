#include "render_state.h"

RenderState::RenderState(const char title[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_init: %s", SDL_GetError());
        throw 1;
    }

    display_mode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
    if (display_mode == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Get current display mode: %s", SDL_GetError());
        throw 1;
    }

    int window_width = 2160;
    int window_height = 1440;

    window = SDL_CreateWindow(title, window_width, window_height, 0);
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Create window: %s", SDL_GetError());
        throw 1;
    }

    if (!SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Center window: %s", SDL_GetError());
        throw 1;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Create renderer: %s", SDL_GetError());
        throw 1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    if (texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Create texture: %s", SDL_GetError());
        throw 1;
    }

    w = window_width;
    h = window_height;

    c_buf = std::vector<uint32_t>(w*h, 0x00000000);
}

void RenderState::render() noexcept {
    // SDL_RenderClear(renderer);
    SDL_UpdateTexture(texture, NULL, c_buf.data(), sizeof(uint32_t) * w);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void RenderState::clear_buffer(uint32_t color) noexcept {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            c_buf[(w * y) + x] = color;
        }
    }
}

void RenderState::draw_pixel(int x, int y, uint32_t color) noexcept {
    assert(x >= 0 && x < w);
    assert(y >= 0 && y < h);
    c_buf[(w * y) + x] = color;
}

void RenderState::draw_rectangle(int x, int y, int w, int h, uint32_t color) noexcept {
    assert(w >= 0 && h >= 0);
    assert(x >= 0 && x+w < this->w);
    assert(y >= 0 && y+h < this->h);
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            draw_pixel(x+i, y+j, color);
        }
    }
}

void RenderState::draw_grid(uint32_t color) noexcept {
    for (int y = 0; y < h; y += 10) {
        for (int x = 0; x < w; x += 1) {
            c_buf[(w * y) + x] = color;
        }
    }

    for (int x = 0; x < w; x += 10) {
        for (int y = 0; y < h; y += 1) {
            c_buf[(w * y) + x] = color;
        }
    }
}

bool RenderState::process_input() {
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyTexture(texture);
            return false;
        }
    }
    return true;
}