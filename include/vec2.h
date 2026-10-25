#pragma once

#include <cmath>

class Vec2 {
public:
    double x = 0.0;
    double y = 0.0;

    Vec2() = default;
    Vec2(double x_, double y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2 &other) const { return {x + other.x, y + other.y}; }
    Vec2 operator-(const Vec2 &other) const { return {x - other.x, y - other.y}; }
    Vec2 operator*(double scalar) const { return {x * scalar, y * scalar}; }

    double length() const { return std::sqrt(x * x + y * y); }
    double distanceTo(const Vec2 &other) const { return (*this - other).length(); }
};

inline Vec2 operator*(double scalar, const Vec2 &v) { return v * scalar; }
