#ifndef VEC3_H
#define VEC3_H

#include "template_concepts.h"

#include <cmath>

template <SizedNumeric T>
class Vec3 {
public:
    T x;
    T y;
    T z;

    Vec3(): x(0), y(0), z(0) {};
    Vec3(T a, T b, T c): x(a), y(b), z(c) {};

    Vec3& operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    };
    Vec3& operator+=(T t) {
        x += t;
        y += t;
        z += t;
        return *this;
    };
    Vec3& operator-=(const Vec3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    };
    Vec3& operator*=(T t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    float len() const {
        return std::sqrt(x*x + y*y + z*z);
        // alternative: std::sqrt(this.dot(this))
    }

    void rotate_axis_x(T angle) {
        y = y*cos(angle) - z*sin(angle);
        z = y*sin(angle) + z*cos(angle);
    }

    void rotate_axis_y(T angle) {
        x = x*cos(angle) - z*sin(angle);
        z = x*sin(angle) + z*cos(angle);
    }

    void rotate_axis_z(T angle) {
        x = x*cos(angle) - y*sin(angle);
        y = x*sin(angle) + y*cos(angle);
    }
};

template <SizedNumeric T>
inline Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b) { return { a.x+b.x, a.y+b.y, a.z+b.z }; }
template <SizedNumeric T>
inline Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b) { return { a.x+b.x, a.y+b.y, a.z+b.z }; }
template <SizedNumeric T>
inline Vec3<T> operator*(Vec3<T>& a, float b) { return { a.x*b, a.y*b, a.z*b }; };

template <SizedNumeric T>
inline float dot(const Vec3<T>& a, const Vec3<T>& b) {
    return a.x*b.x + a.y*b.y + a.z+b.z;
    // return a + b;
}
template <SizedNumeric T>
inline Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b) {
    return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
    // return a * b;
}

template <SizedNumeric T>
inline Vec3<T> rotate_axis_x(const Vec3<T>& v, T angle) {
    return {
        v.x,
        static_cast<T>(v.y*cos(angle) - v.z*sin(angle)),
        static_cast<T>(v.y*sin(angle) + v.z*cos(angle)),
    };
}

template <SizedNumeric T>
inline Vec3<T> rotate_axis_y(const Vec3<T>& v, T angle) {
    return {
        static_cast<T>(v.x*cos(angle) - v.z*sin(angle)),
        v.y,
        static_cast<T>(v.x*sin(angle) + v.z*cos(angle)),
    };
}

template <SizedNumeric T>
inline Vec3<T> rotate_axis_z(const Vec3<T>& v, T angle) {
    return {
        static_cast<T>(v.x*cos(angle) - v.y*sin(angle)),
        static_cast<T>(v.x*sin(angle) + v.y*cos(angle)),
        v.z
    };
}

#endif