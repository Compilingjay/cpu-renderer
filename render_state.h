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

struct RenderState {
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        SDL_Event event;
        const SDL_DisplayMode* display_mode;
        int w;
        int h;
        std::vector<uint32_t> c_buf;

        RenderState() = default;
        RenderState(const char title[]);

        template <SizedNumeric T>
        Vec2<int> project_orthographic(const Vec3<T>& v, const Camera<T>& c) noexcept {
            return { static_cast<int>(c.fov_angle * v.x), static_cast<int>(c.fov_angle * v.y) };
        }
        template <SizedNumeric T>
        Vec2<int> project_perspective(const Vec3<T>& v, const Camera<T>& c) noexcept {
            return { static_cast<int>(c.fov_angle * v.x / (v.z - c.position.z)), static_cast<int>(c.fov_angle * v.y / (v.z - c.position.z)) };
        }

        template <SizedNumeric T>
        void update(const std::vector<Vec3<T>>& vertices, std::vector<Vec3<T>>& faces_buf, const Camera<T>& c, const Vec3<T>& rot, uint32_t color) noexcept { // remove color later on? and refactor vec to be list of objects
            
            draw_grid();

            int x_off = w / 2;
            int y_off = h / 2;
            std::vector<Vec2<int>> transformed_vertices(3);

            for (Vec3 face : faces_buf) {
                std::vector<Vec3<T>> face_vertices = { vertices[face.x], vertices[face.y], vertices[face.z] };
                for (int i = 0; i < 3; ++i) {
                    Vec3 rot_vi = rotate_axis_x(rotate_axis_y(rotate_axis_z(face_vertices[i], rot.z), rot.y), rot.x);
                    Vec2 proj_pt = project_perspective(rot_vi, c);
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

        void render() noexcept;
        void clear_buffer(uint32_t color = 0x000000ff) noexcept;

        void draw_pixel(int x, int y, uint32_t color) noexcept;
        void draw_rectangle(int x, int y, int w, int h, uint32_t color) noexcept;
        void draw_grid(uint32_t color = 0x333333ff) noexcept;
        void draw_line_dda(int x1, int y1, int x2, int y2, uint32_t color) noexcept;
        void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) noexcept;

        bool process_input();

        template <SizedNumeric T>
        void run(std::vector<Vec3<T>>& vertex_buf, std::vector<Vec3<T>>& faces_buf, Camera<T>& camera) {
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
                update(vertex_buf, faces_buf, camera, rotation, 0xDDDD00FF);
                render();
                ++i;
            }
        }
};

#endif