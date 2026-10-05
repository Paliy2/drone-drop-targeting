#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

constexpr double G = 9.81;

struct Coords {
    double x = 0.0;
    double y = 0.0;
};

double getFallingTime(double height) { return std::sqrt(2 * height / G); }

double getDistance(const Coords &a, const Coords &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

Coords getRelease(double height, const Coords &target, const Coords &wind) {
    double falling_time = getFallingTime(height);
    return {target.x - wind.x * falling_time, target.y - wind.y * falling_time};
}

enum TargetType { PERSONNEL, LOGISTICS, APC, TANK, RADAR, TARGET_TYPE_COUNT };

constexpr int kTargetValues[TARGET_TYPE_COUNT] = {10, 20, 40, 60, 90};

TargetType parseTargetType(const std::string &value) {
    if (value == "PERSONNEL") return PERSONNEL;
    if (value == "LOGISTICS") return LOGISTICS;
    if (value == "APC") return APC;
    if (value == "TANK") return TANK;
    if (value == "RADAR") return RADAR;

    return PERSONNEL;
}

struct Target {
    Coords coords;
    int health = 0;
    TargetType target_type = PERSONNEL;

    int getValue() const { return kTargetValues[target_type]; }
};

struct ScenarioParams {
    double height = 0.0;
    Coords wind;
    std::vector<Target> targets;
};

struct Shell {
    std::string name;
    int radius = 0;
    int damage = 0;
    int price = 0;
};

struct HitStatistics {
    int destroyed = 0;
    int value = 0;

    Shell shell;
    Coords impact;
    Coords release;

    int getNet() const { return value - shell.price; }
};

struct DroneManager {
    Shell available_shells[3] = {
        {"Frag", 60, 25, 35},
        {"HE", 35, 55, 20},
        {"HEAT", 15, 130, 45}
    };

    HitStatistics getImpact(const ScenarioParams &scenario) {
        HitStatistics best;
        bool has_best = false;

        for (const Shell &shell: available_shells) {
            for (std::size_t i = 0; i < scenario.targets.size(); i++) {
                int destroyed = 0;
                int value_sum = 0;

                for (std::size_t j = 0; j < scenario.targets.size(); j++) {
                    double distance = getDistance(scenario.targets[i].coords, scenario.targets[j].coords);

                    if (distance <= shell.radius &&
                        scenario.targets[j].health <= shell.damage) {
                        destroyed++;
                        value_sum += scenario.targets[j].getValue();
                    }
                }

                int net = value_sum - shell.price;
                bool is_better = !has_best || net > best.getNet() || (
                                     net == best.getNet() && shell.price < best.shell.price);

                if (is_better) {
                    has_best = true;
                    best.destroyed = destroyed;
                    best.value = value_sum;
                    best.shell = shell;
                    best.impact = scenario.targets[i].coords;
                }
            }
        }

        best.release = getRelease(scenario.height, best.impact, scenario.wind);
        return best;
    }
};

struct FileManager {
    ScenarioParams readScenario(const std::string &file_name) {
        ScenarioParams scenario;

        std::ifstream in(file_name);

        if (!in.is_open()) {
            std::cerr << "Failed to open " << file_name << "\n";
            std::exit(1);
        }

        int targets_count = 0;

        in >> scenario.height;
        in >> scenario.wind.x >> scenario.wind.y;
        in >> targets_count;

        for (int i = 0; i < targets_count; i++) {
            Target target;
            std::string target_type_string;

            in >> target.coords.x;
            in >> target.coords.y;
            in >> target.health;
            in >> target_type_string;

            target.target_type = parseTargetType(target_type_string);
            scenario.targets.push_back(target);
        }

        return scenario;
    }

    void writeStatistics(const std::string &file_name, const HitStatistics &statistics) {
        std::ofstream out(file_name);

        if (!out.is_open()) {
            std::cerr << "Failed to open output file: " << file_name << "\n";
            std::exit(1);
        }

        out << std::fixed << std::setprecision(2);
        out << statistics.impact.x << " " << statistics.impact.y << "\n";
        out << statistics.release.x << " " << statistics.release.y << "\n";
        out << statistics.shell.name << "\n";
        out << statistics.destroyed << " " << statistics.value << " "
                << statistics.shell.price << " " << statistics.getNet() << "\n";
    }
};

int main() {
    FileManager file_manager;
    DroneManager drone_manager;

    const ScenarioParams scenario = file_manager.readScenario("scenario.txt");
    const HitStatistics statistics = drone_manager.getImpact(scenario);
    file_manager.writeStatistics("result.txt", statistics);
    return 0;
}
