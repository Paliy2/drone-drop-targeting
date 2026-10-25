#pragma once

#include <cmath>

#include "vec2.h"

constexpr double kGravity = 9.81;

inline double fallTime(double altitude) { return std::sqrt(2 * altitude / kGravity); }

// Contract: given the impact point, release altitude and wind, return where to release.
class ITrajectorySource {
public:
    virtual ~ITrajectorySource() = default;
    virtual Vec2 release(const Vec2 &impact, double altitude, const Vec2 &wind) const = 0;
};

// Drone hovers; the shell free-falls and drifts with the wind only.
class HoverTrajectory : public ITrajectorySource {
public:
    Vec2 release(const Vec2 &impact, double altitude, const Vec2 &wind) const override {
        return impact - wind * fallTime(altitude);
    }
};

// Drone drops on the move; the shell inherits the drone's horizontal velocity.
class ForwardThrowTrajectory : public ITrajectorySource {
public:
    explicit ForwardThrowTrajectory(const Vec2 &drone_velocity) : drone_velocity_(drone_velocity) {}

    Vec2 release(const Vec2 &impact, double altitude, const Vec2 &wind) const override {
        return impact - (wind + drone_velocity_) * fallTime(altitude);
    }

private:
    Vec2 drone_velocity_;
};
