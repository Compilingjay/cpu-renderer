#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

template <SizedNumeric T>
class Camera {
    public:
        Vec3<T> position;
        Vec3<T> rotation;
        T fov_angle;
};

#endif