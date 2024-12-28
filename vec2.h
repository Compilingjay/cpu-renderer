#ifndef VEC2_H
#define VEC2_H

#include <cmath>

struct Vec2 {
public:
    double x;
    double y;

    Vec2(): x(0), y(0) {};
    Vec2(double a, double b): x(a), y(b) {};

    inline int xi() const { return static_cast<int>(x); }
    inline int yi() const { return static_cast<int>(y); }

    Vec2& operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
        return *this;
    };
    Vec2& operator+=(double t) {
        x += t;
        y += t;
        return *this;
    };
    Vec2& operator-=(const Vec2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    };
    Vec2& operator*=(double t) {
        x *= t;
        y *= t;
        return *this;
    }

    double len() const { return std::sqrt((x * x) + (y * y)); }
    // void rotate(double angle) {
    //     x = x * cos(angle) - y * sin(angle);
    //     y = x * sin(angle) + y * cos(angle);
    // }
};

inline Vec2 operator+(const Vec2& a, const Vec2& b) { return { a.x + b.x, a.y + b.y }; }
inline Vec2 operator-(const Vec2& a, const Vec2& b) { return { a.x - b.x, a.y - b.y }; }
inline Vec2 operator*(const Vec2& a, double b) { return { a.x * b, a.y * b }; };
inline bool operator==(const Vec2&a, const Vec2& b) {
    return a.x == b.x && a.y == b.y;
}

inline Vec2 normalized(const Vec2& v) { return v * (1/v.len()); }

inline double dot(const Vec2& a, const Vec2& b) {  return (a.x * b.x) + (a.y * b.y); }
inline double cross(const Vec2& a, const Vec2& b) { return (a.x * b.y) - (a.y * b.x); }

inline Vec2 perpendicular(const Vec2& v) { return { v.y, -v.x }; }

inline Vec2 rotate(const Vec2& v, double angle) {
    return {
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle)
    };
}

#endif