#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

struct Camera {
    public:
        Vec3 position;
        Vec3 rotation;
        double fov_factor;
};

#endif