#include "render_state.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>

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

void RenderState::draw_line_dda(int x1, int y1, int x2, int y2, uint32_t color) noexcept {
    assert(x1 >= 0 && x1 < w);
    assert(y1 >= 0 && y1 < h);
    assert(x2 >= 0 && x2 < w);
    assert(y2 >= 0 && y2 < h);

    if (x1 > x2) { // unnecessary, except if you want render left to right
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int delta_x = x2 - x1;
    int delta_y = y2 - y1;

    int longest_side_len = delta_x >= abs(delta_y) ? delta_x : abs(delta_y);

    float x_inc = delta_x / static_cast<float>(longest_side_len);
    float y_inc = delta_y / static_cast<float>(longest_side_len);

    float curr_x = x1;
    float curr_y = y1;

    for (int i = 0; i < longest_side_len; ++i) {
        draw_pixel(round(curr_x), round(curr_y), color);
        curr_x += x_inc;
        curr_y += y_inc;
    }
}

void RenderState::draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) noexcept {
    draw_line_dda(x1, y1, x2, y2, color);
    draw_line_dda(x1, y1, x3, y3, color);
    draw_line_dda(x2, y2, x3, y3, color);
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