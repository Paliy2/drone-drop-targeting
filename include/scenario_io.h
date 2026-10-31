#pragma once

#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "drop_planner.h"
#include "mission.h"

class ScenarioReader {
public:
    explicit ScenarioReader(std::string file_name) : file_name_(std::move(file_name)) {}

    Scenario read() const {
        std::ifstream in(file_name_);
        if (!in.is_open()) {
            throw std::runtime_error("Failed to open " + file_name_);
        }

        double altitude = 0.0;
        Vec2 wind;
        int targets_count = 0;

        in >> altitude;
        in >> wind.x >> wind.y;
        in >> targets_count;

        std::vector<Target> targets;
        targets.reserve(targets_count > 0 ? targets_count : 0);
        for (int i = 0; i < targets_count; i++) {
            Vec2 position;
            int health = 0;
            std::string type;

            in >> position.x >> position.y >> health >> type;
            targets.emplace_back(position, health, parseTargetType(type));
        }

        return Scenario(altitude, wind, std::move(targets));
    }

private:
    std::string file_name_;
};

class ResultWriter {
public:
    explicit ResultWriter(std::string file_name) : file_name_(std::move(file_name)) {}

    void write(const DropSolution &solution) const {
        std::ofstream out(file_name_);
        if (!out.is_open()) {
            throw std::runtime_error("Failed to open output file: " + file_name_);
        }

        out << std::fixed << std::setprecision(2);
        out << solution.impact.x << " " << solution.impact.y << "\n";
        out << solution.release.x << " " << solution.release.y << "\n";
        out << solution.shell.name() << "\n";
        out << solution.destroyed << " " << solution.value << " "
            << solution.shell.price() << " " << solution.net() << "\n";
    }

private:
    std::string file_name_;
};
