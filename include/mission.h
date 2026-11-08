#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "vec2.h"

enum TargetType { PERSONNEL, LOGISTICS, APC, TANK, RADAR, TARGET_TYPE_COUNT };

constexpr std::array<int, TARGET_TYPE_COUNT> kTargetValues = {10, 20, 40, 60, 90};

inline TargetType parseTargetType(const std::string &value) {
    static const std::unordered_map<std::string, TargetType> kTypes = {
        {"PERSONNEL", PERSONNEL}, {"LOGISTICS", LOGISTICS}, {"APC", APC}, {"TANK", TANK}, {"RADAR", RADAR},
    };
    const auto it = kTypes.find(value);
    return it != kTypes.end() ? it->second : PERSONNEL;
}

class Target {
public:
    Target(const Vec2 &position, int health, TargetType type)
        : position_(position), health_(health), type_(type) {}

    const Vec2 &position() const { return position_; }
    int health() const { return health_; }
    int value() const { return kTargetValues[type_]; }

private:
    Vec2 position_;
    int health_;
    TargetType type_;
};

class Shell {
public:
    Shell() = default;
    Shell(std::string name, double radius, int damage, int price)
        : name_(std::move(name)), radius_(radius), damage_(damage), price_(price) {}

    const std::string &name() const { return name_; }
    int price() const { return price_; }

    bool destroys(const Vec2 &impact, const Target &target) const {
        return impact.distanceTo(target.position()) <= radius_ && target.health() <= damage_;
    }

private:
    std::string name_;
    double radius_ = 0.0;
    int damage_ = 0;
    int price_ = 0;
};

class Scenario {
public:
    Scenario(double altitude, const Vec2 &wind, std::vector<Target> targets)
        : altitude_(altitude), wind_(wind), targets_(std::move(targets)) {}

    double altitude() const { return altitude_; }
    const Vec2 &wind() const { return wind_; }
    const std::vector<Target> &targets() const { return targets_; }

private:
    double altitude_;
    Vec2 wind_;
    std::vector<Target> targets_;
};
