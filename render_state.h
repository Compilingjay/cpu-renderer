#ifndef DISPLAY_H
#define DISPLAY_H

#include "camera.h"
#include "mesh.h"
#include "template_concepts.h"
#include "vec2.h"
#include "vec3.h"

#include <SDL3/SDL.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <span>
#include <vector>

constexpr float FPS = 30.0f;
constexpr float FRAME_TARGET_TIME_NS = (1.0f / FPS) * 1.0e+9f;

template <SizedNumeric T>
struct RenderState {
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        const SDL_DisplayMode* display_mode;
        int w;
        int h;
        SDL_Event event;
        Camera<T> camera;
        std::vector<uint32_t> c_buf;
        std::vector<Vec3<T>> vertex_buf;
        std::vector<Vec3<T>> faces_buf;

        RenderState() {};
        RenderState(const char title[]) {
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

            // // auto vertex_buf = get_9x9x9_vectors<float>();
            vertex_buf = get_cube_vertices<T>();
            faces_buf = get_cube_faces<T>();
            camera = Camera<T> { { 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, 640.0f };
        }

        Vec2<int> project_orthographic(const Vec3<T>& v) noexcept {
            return { static_cast<int>(camera.fov_angle * v.x), static_cast<int>(camera.fov_angle * v.y) };
        }

        Vec2<int> project_perspective(const Vec3<T>& v) noexcept {
            return {
                static_cast<int>(camera.fov_angle * v.x / (v.z - camera.position.z)),
                static_cast<int>(camera.fov_angle * v.y / (v.z - camera.position.z))
            };
        }

        void update(const Vec3<T>& rot, uint32_t color) noexcept { // remove color later on? and refactor vec to be list of objects
            
            draw_grid(0x333333ff);

            int x_off = w / 2;
            int y_off = h / 2;
            
            std::vector<Vec3<T>> face_vertices(3);
            std::vector<Vec2<int>> transformed_vertices(3);

            for (Vec3 face : faces_buf) {
                face_vertices = { vertex_buf[face.x], vertex_buf[face.y], vertex_buf[face.z] };
                for (int i = 0; i < 3; ++i) {
                    Vec3 rot_vi = rotate_axis_x(rotate_axis_y(rotate_axis_z(face_vertices[i], rot.z), rot.y), rot.x);
                    // Vec2 proj_pt = project_orthographic(rot_vi*0.25, c); // fov needs to be reduced for this type of projection to function properly, hence *0.25
                    Vec2 proj_pt = project_perspective(rot_vi);
                    int x = proj_pt.x + x_off;
                    int y = proj_pt.y + y_off;
                    draw_rectangle(x, y, 4, 4, color);
                    transformed_vertices[i] = { x, y };
                }

                draw_triangle(
                    transformed_vertices[0].x, transformed_vertices[0].y,
                    transformed_vertices[1].x, transformed_vertices[1].y, 
                    transformed_vertices[2].x, transformed_vertices[2].y,
                    0xcc8800ff);
            }

            // loop over all points and rotate
            // for (int i = 0; i < vertices.size(); ++i) {
            //     // Vec2<int> proj_pt = project_orthographic(v[i], c);
            //     Vec3 rot_vi = rotate_axis_x(rotate_axis_y(rotate_axis_z(vertices[i], rot.z), rot.y), rot.x);
            //     Vec2 proj_pt = project_perspective(rot_vi, c);

            //     assert(proj_pt.x+x_off >= 0 && proj_pt.x+x_off < w);
            //     assert(proj_pt.y+y_off >= 0 && proj_pt.y+y_off < h);
            //     draw_rectangle(proj_pt.x + x_off, proj_pt.y + y_off, 4, 4, color);
            // }
        }

        void render() noexcept {
            // SDL_RenderClear(renderer);
            SDL_UpdateTexture(texture, NULL, c_buf.data(), sizeof(uint32_t) * w);
            SDL_RenderTexture(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }

        void clear_buffer() noexcept {
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    c_buf[(w * y) + x] = 0x000000ff;
                }
            }
        }

        void draw_grid(uint32_t color) noexcept {
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

        void draw_pixel(int x, int y, uint32_t color) noexcept {
            assert(x >= 0 && x < w);
            assert(y >= 0 && y < h);
            c_buf[(w * y) + x] = color;
        }

        void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) noexcept {
            draw_line_dda(x1, y1, x2, y2, color);
            draw_line_dda(x1, y1, x3, y3, color);
            draw_line_dda(x2, y2, x3, y3, color);
        }

        void draw_rectangle(int x, int y, int w, int h, uint32_t color) noexcept {
            assert(w >= 0 && h >= 0);
            assert(x >= 0 && x+w < this->w);
            assert(y >= 0 && y+h < this->h);
            for (int j = 0; j < h; ++j) {
                for (int i = 0; i < w; ++i) {
                    draw_pixel(x+i, y+j, color);
                }
            }
        }

        void draw_line_dda(int x1, int y1, int x2, int y2, uint32_t color) noexcept {
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

        bool process_input() {
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

        void run() {
            auto start_time = std::chrono::high_resolution_clock::now();
            auto current_time = std::chrono::high_resolution_clock::now();
            SDL_Event event;
            Vec3 rotation = Vec3<T>{0.0f, 0.0f, 0.0f};
            int i = 0;
            
            auto prev_frame_time = SDL_GetTicksNS();
            float time_to_wait;

            while (true) {
                // while (SDL_GetTicksNS() < curr_frame_time+FRAME_TARGET_TIME_NS) {}
                // curr_frame_time += FRAME_TARGET_TIME_NS;
                time_to_wait = FRAME_TARGET_TIME_NS - (SDL_GetTicksNS() - prev_frame_time);
                if (time_to_wait > 0) {
                    SDL_DelayNS(time_to_wait);
                    if (time_to_wait > FRAME_TARGET_TIME_NS) {
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "wait time exceeded frame time: %f, frame time: %f", time_to_wait, FRAME_TARGET_TIME_NS);
                    }
                }
                prev_frame_time = SDL_GetTicksNS();

                if (!process_input()) {
                    current_time = std::chrono::high_resolution_clock::now();
                    std::cout << "Program has been running for " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() << " seconds, i = " << i << std::endl;
                    return;
                }
                rotation.z += 0.02f;
                rotation.y += 0.02f;
                rotation.z += 0.02f;
                clear_buffer();
                update(rotation, 0xDDDD00FF);
                render();
                ++i;
            }
        }
};

#endif