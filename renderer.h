#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "mesh.h"
#include "string_utils.h"
#include "triangle.h"
#include "vec2.h"

#include <SDL3/SDL.h>

#include <chrono>
#include <iostream>

constexpr float FPS_30 = 30.0f;
constexpr float FPS_60 = 60.0f;
constexpr float FRAME_TARGET_TIME_NS = (1.0f / FPS_60) * 1.0e+9f;

enum DisplayFlags {
    Vertices        = 0x01,
    Wireframe       = 0x02,
    PolygonFill     = 0x04,
    BackfaceCulling = 0x08,
};

class Renderer {
    using enum DisplayFlags;
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        const SDL_DisplayMode* display_mode;
        int w;
        int h;
        std::vector<uint32_t> c_buf;
        std::vector<Mesh> meshes;
        Camera camera;
        SDL_Event event;
        std::vector<Triangle> triangles;
        uint64_t prev_frame_time;
        uint8_t flags;

        Renderer() = default;

        void initialize(std::string title, std::string mesh_path);
        bool process_input();
        void deinitialize();

        Vec2 project_orthographic(const Vec3& p) noexcept;

        Vec2 project_perspective(const Vec3& p) noexcept;

        void update();
        void render();

        void draw_grid(uint32_t color) noexcept;

        void clear_buffer() noexcept;
        void draw_pixel(int x, int y, uint32_t color) noexcept;
        void draw_line_dda(int x1, int y1, int x2, int y2, uint32_t color) noexcept;
        void draw_triangle(const Triangle& t, uint32_t color) noexcept;
        void draw_rectangle(int x, int y, int width, int height, uint32_t color) noexcept;
};

#endif