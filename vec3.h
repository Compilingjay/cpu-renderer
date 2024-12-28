#ifndef VEC3_H
#define VEC3_H

#include <cmath>

struct Vec3 {
public:
    double x;
    double y;
    double z;

    Vec3(): x(0), y(0), z(0) {};
    Vec3(double a, double b, double c): x(a), y(b), z(c) {};

    inline int xi() const { return static_cast<int>(x); }
    inline int yi() const { return static_cast<int>(y); }
    inline int zi() const { return static_cast<int>(z); }

    Vec3& operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    };
    Vec3& operator+=(double t) {
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
    Vec3& operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    double len() const { return std::sqrt((x * x) + (y * y) + (z * z)); }

    // none of these functions work properly...
    // void rotate_axis_x(double angle) {
    //     y = y * cos(angle) - z * sin(angle);
    //     z = y * sin(angle) + z * cos(angle);
    // };
    // void rotate_axis_y(double angle) {
    //     x = x * cos(angle) - z * sin(angle);
    //     z = x * sin(angle) + z * cos(angle);
    // }
    // void rotate_axis_z(double angle) {
    //     x = x * cos(angle) - y * sin(angle),
    //     y = x * sin(angle) + y * cos(angle),
    // }
};

inline Vec3 operator+(const Vec3& a, const Vec3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
inline Vec3 operator-(const Vec3& a, const Vec3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
inline Vec3 operator*(const Vec3& a, double b) { return { a.x * b, a.y * b, a.z * b }; };
inline bool operator==(const Vec3&a, const Vec3& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline Vec3 normalized(const Vec3& v) { return v * (1/v.len()); }
inline double dot(const Vec3& a, const Vec3& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return {
        (a.y*b.z) - (a.z*b.y),
        (a.z*b.x) - (a.x*b.z),
        (a.x*b.y) - (a.y*b.x)
    };
}

inline Vec3 rotate_axis_x(const Vec3& v, double angle) {
    return {
        v.x,
        v.y * cos(angle) - v.z * sin(angle),
        v.y * sin(angle) + v.z * cos(angle),
    };
}
inline Vec3 rotate_axis_y(const Vec3& v, double angle) {
    return {
        v.x * cos(angle) - v.z * sin(angle),
        v.y,
        v.x * sin(angle) + v.z * cos(angle),
    };
}
inline Vec3 rotate_axis_z(const Vec3& v, double angle) {
    return {
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle),
        v.z
    };
}

#endif