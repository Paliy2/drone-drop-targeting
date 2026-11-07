#pragma once

#include <cmath>

#include "vec2.h"

constexpr double kGravity = 9.81;

inline double fallTime(double altitude) { return std::sqrt(2 * altitude / kGravity); }

class ITrajectorySource {
public:
    virtual ~ITrajectorySource() = default;
    virtual Vec2 release(const Vec2 &impact, double altitude, const Vec2 &wind) const = 0;
};

class HoverTrajectory : public ITrajectorySource {
public:
    Vec2 release(const Vec2 &impact, double altitude, const Vec2 &wind) const override {
        return impact - wind * fallTime(altitude);
    }
};

class ForwardThrowTrajectory : public ITrajectorySource {
public:
    explicit ForwardThrowTrajectory(const Vec2 &drone_velocity) : drone_velocity_(drone_velocity) {}

    Vec2 release(const Vec2 &impact, double altitude, const Vec2 &wind) const override {
        return impact - (wind + drone_velocity_) * fallTime(altitude);
    }

private:
    Vec2 drone_velocity_;
};
