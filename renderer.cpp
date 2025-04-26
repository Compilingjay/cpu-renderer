#include "renderer.hpp"

#include <array>
#include <cstdint>

void Renderer::initialize(std::string title, std::string mesh_path) {
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

    window = SDL_CreateWindow(title.c_str(), window_width, window_height, 0);
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

    meshes.push_back(get_mesh_from_obj_file(mesh_path));
    camera = Camera { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, 640.0 };
    triangles = {};
    prev_frame_time = SDL_GetTicksNS();
    flags = 0xff;
}

void Renderer::deinitialize() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
}

bool Renderer::process_input() {
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_W:
                        camera.rotation.x -= 0.1;
                        break;
                    case SDLK_S:
                        camera.rotation.x += 0.1;
                        break;
                    case SDLK_A:
                        camera.rotation.y += 0.1;
                        break;
                    case SDLK_D:
                        camera.rotation.y -= 0.1;
                        break;
                    case SDLK_Q:
                        camera.rotation.z -= 0.1;
                        break;
                    case SDLK_E:
                        camera.rotation.z += 0.1;
                        break;
                    case SDLK_1:
                        flags &= BackfaceCulling;
                        flags |= Vertices | Wireframe;
                        break;
                    case SDLK_2:
                        flags &= BackfaceCulling;
                        flags |= Wireframe;
                        break;
                    case SDLK_3:
                        flags &= BackfaceCulling;
                        flags |= PolygonFill;
                        break;
                    case SDLK_4:
                        flags &= BackfaceCulling;
                        flags |= Wireframe | PolygonFill;
                        break;
                    case SDLK_C:
                        flags &= Vertices | Wireframe | PolygonFill;
                        flags |= BackfaceCulling;
                        break;
                    case SDLK_X:
                        flags &= Vertices | Wireframe | PolygonFill;
                        flags |= ~BackfaceCulling;
                        break;
                    case SDLK_ESCAPE:
                        deinitialize();
                        return false;
                    default:
                        return true;
                }
                break;
            case SDL_EVENT_QUIT:
                deinitialize();
                return false;
        }
    }
    return true;
}

void Renderer::update() {
    int window_width_offset = w/2;
    int window_height_offset = h/2;
    Triangle triangle;
    Vec2 projected_point;

    uint64_t time_to_wait = FRAME_TARGET_TIME_NS - (SDL_GetTicksNS() - prev_frame_time);
    if (time_to_wait > 0) {
        SDL_DelayNS(time_to_wait);
        if (time_to_wait > FRAME_TARGET_TIME_NS) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "wait time exceeded frame time: %llu, frame time: %f", time_to_wait, FRAME_TARGET_TIME_NS);
        }
    }
    prev_frame_time = SDL_GetTicksNS();

    for (Mesh& mesh : meshes) {
        int i = 0;
        // mesh.rot = global_rot;
        uint32_t color = 0xccdd33ff;
        for (const std::array<int, 3>& face : mesh.faces) {
            std::array<Vec3, 3> face_vertices = {
                mesh.vertices[face[0] - 1],
                mesh.vertices[face[1] - 1],
                mesh.vertices[face[2] - 1]
            };

            std::array<Vec3, 3> transformed_vertices;
            for (int i = 0; i < 3; ++i) {
                transformed_vertices[i] = rotate_axis_z(rotate_axis_y(rotate_axis_x(face_vertices[i], mesh.rot.x), mesh.rot.y), mesh.rot.z);
                transformed_vertices[i] = rotate_axis_z(rotate_axis_y(rotate_axis_x(face_vertices[i], camera.rotation.x), camera.rotation.y), camera.rotation.z);
                transformed_vertices[i].z += 5;
            }

            triangle.color = color;
            color = 0x000000ff | color+0x132480ff;

            if ((flags & BackfaceCulling) == BackfaceCulling) {
                Vec3 normal = cross(transformed_vertices[1] - transformed_vertices[0], transformed_vertices[2] - transformed_vertices[0]);
                Vec3 camera_ray = camera.position - transformed_vertices[0];
                if (dot(normal, camera_ray) < 0) { continue; }
            }

            for (int i = 0; i < 3; ++i) {
                projected_point = project_perspective(transformed_vertices[i]);
                projected_point.x += window_width_offset;
                projected_point.y += window_height_offset;

                triangle.points[i] = projected_point;
            }
            triangle.avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;
            triangles.push_back(triangle);
        }

        // mesh.rot += 0.01;
    }

    // sort faces by depth (average z)
    uint32_t color = 0xccdd33ff;
    for (Triangle& t1 : triangles) {

        for (Triangle& t2 : triangles) {
            if (t1.avg_depth > t2.avg_depth) { std::swap(t1, t2); }
        }
    }
}

void Renderer::render() {
    draw_grid(0x333333ff);

    for (const Triangle& t : triangles) {
        draw_triangle(t, t.color, 0x00aabbff, 0xee4444ff);
    }

    triangles = {};
    SDL_UpdateTexture(texture, NULL, c_buf.data(), sizeof(uint32_t) * w);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    clear_buffer();
}

Vec2 Renderer::project_orthographic(const Vec3& p) noexcept {
    return { camera.fov_factor * p.x, camera.fov_factor * p.y };
}

Vec2 Renderer::project_perspective(const Vec3& p) noexcept {
    return { (camera.fov_factor * p.x) / p.z, (camera.fov_factor * p.y) / p.z };
}

void Renderer::draw_grid(uint32_t color) noexcept {
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

void Renderer::clear_buffer() noexcept {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            c_buf[(w * y) + x] = 0x000000ff;
        }
    }
}

void Renderer::draw_pixel(int x, int y, uint32_t color) noexcept {
    if (x < 0 || x >= w || y < 0 || y >= h) return;
    c_buf[(w * y) + x] = color;
}

void Renderer::draw_line_dda(int x1, int y1, int x2, int y2, uint32_t color) noexcept {
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

void Renderer::draw_triangle(const Triangle& t, uint32_t fill_color, uint32_t wire_color, uint32_t vertex_color) noexcept {
    std::array<Vec2, 3> points = { t.points[0], t.points[1], t.points[2] };
    if ((flags & PolygonFill) == PolygonFill) {
        if (points[0].y > points[1].y) { std::swap(points[0], points[1]); }
        if (points[1].y > points[2].y) { std::swap(points[1], points[2]); }
        if (points[0].y > points[1].y) { std::swap(points[0], points[1]); }

        Vec2 midpoint = {
            (((points[2].x - points[0].x) * (points[1].y - points[0].y)) / (points[2].y - points[0].y)) + points[0].x,
            points[1].y
        };

        bool midpoint_left, vertical_line;
        if (points[1].x < midpoint.x) {
            midpoint_left = false;
        } else {
            midpoint_left = true;
        }

        double dxy_left, dxy_right, x_start, x_end;
        if (points[0].y != points[1].y) {
            if (!midpoint_left) {
                dxy_left = (points[1].x - points[0].x) / (points[1].y - points[0].y);
                dxy_right = (midpoint.x - points[0].x) / (midpoint.y - points[0].y);
                x_start = points[1].x;
                x_end = midpoint.x;
            } else {
                dxy_left = (midpoint.x - points[0].x) / (midpoint.y - points[0].y);
                dxy_right = (points[1].x - points[0].x) / (points[1].y - points[0].y);
                x_start = midpoint.x;
                x_end = points[1].x;
            }

            for(int y = midpoint.y; y >= points[0].y; --y) {
                for (int x = x_start; x <= x_end; ++x) {
                    draw_pixel(x, y, fill_color);
                }
                x_start -= dxy_left;
                x_end -= dxy_right;
            }
        }

        if (points[1].y != points[2].y) {
            if (!midpoint_left) {
                x_start = points[1].x;
                x_end = midpoint.x;
                dxy_left = (points[2].x - points[1].x) / (points[2].y - points[1].y);
                dxy_right = (points[2].x - midpoint.x) / (points[2].y - midpoint.y);
            } else {
                x_start = midpoint.x;
                x_end = points[1].x;
                dxy_left = (points[2].x - midpoint.x) / (points[2].y - midpoint.y);
                dxy_right = (points[2].x - points[1].x) / (points[2].y - points[1].y);
            }

            for(int y = midpoint.y; y <= points[2].y; ++y) {
                for (int x = x_start; x <= x_end; ++x) {
                    draw_pixel(x, y, fill_color);
                }
                x_start += dxy_left;
                x_end += dxy_right;
            }
        }
    }

    if ((flags & Wireframe) == Wireframe) {
        draw_line_dda(t.points[0].x, t.points[0].y, t.points[1].x, t.points[1].y, wire_color);
        draw_line_dda(t.points[0].x, t.points[0].y, t.points[2].x, t.points[2].y, wire_color);
        draw_line_dda(t.points[1].x, t.points[1].y, t.points[2].x, t.points[2].y, wire_color);
    }
    if ((flags & Vertices) == Vertices) {
        draw_rectangle(t.points[0].x-1, t.points[0].y-1, 4, 4, vertex_color);
        draw_rectangle(t.points[1].x-1, t.points[1].y-1, 4, 4, vertex_color);
        draw_rectangle(t.points[2].x-1, t.points[2].y-1, 4, 4, vertex_color);
    }
}

void Renderer::draw_rectangle(int x, int y, int width, int height, uint32_t color) noexcept {
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            draw_pixel(x+i, y+j, color);
        }
    }
}