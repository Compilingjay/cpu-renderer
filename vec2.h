#ifndef VEC2_H
#define VEC2_H

#include "template_concepts.h"

#include <cmath>

template <SizedNumeric T>
class Vec2 {
public:
    T x;
    T y;

    Vec2(): x(0), y(0) {};
    Vec2(T a, T b): x(a), y(b) {};

    Vec2& operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
        return *this;
    };
    Vec2& operator-=(const Vec2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    };
    Vec2& operator*=(float t) {
        x *= t;
        y *= t;
        return *this;
    }

    float len() const {
        return std::sqrt(x*x + y*y);
        // alternative: std::sqrt(this.dot(this))
    }

    void rotate(T angle) {
        x = x*cos(angle) - y*sin(angle);
        y = x*sin(angle) + y*cos(angle);
    }
};

template <SizedNumeric T>
inline Vec2<T> operator-(const Vec2<T>& a, const Vec2<T>& b) { return { a.x+b.x, a.y+b.y }; }
template <SizedNumeric T>
inline Vec2<T> operator+(const Vec2<T>& a, const Vec2<T>& b) { return { a.x+b.x, a.y+b.y }; }
template <SizedNumeric T>
inline Vec2<T> operator*(Vec2<T>& a, T b) { return { a.x*b, a.y*b }; };

template <SizedNumeric T>
inline T dot(const Vec2<T>& a, const Vec2<T>& b) {  return a.x*b.x + a.y*b.y; }
template <SizedNumeric T>
inline T cross(const Vec2<T>& a, const Vec2<T>& b) { return a.x*b.y - a.y*b.x; }

// rotation by -90 degrees? or cross product of input vec and unit vector pointing "up"? at z?
template <SizedNumeric T>
inline Vec2<T> perpendicular(const Vec2<T>& v) { return { v.y, -v.x }; }

template <SizedNumeric T>
inline Vec2<T> rotate(const Vec2<T>& v, T angle) {
    return {
        static_cast<T>(v.x*cos(angle) - v.y*sin(angle)),
        static_cast<T>(v.x*sin(angle) + v.y*cos(angle))
    };
}

#endif