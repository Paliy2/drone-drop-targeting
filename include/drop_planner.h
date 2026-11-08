#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <utility>
#include <vector>

#include "mission.h"
#include "trajectory.h"

struct DropSolution {
    Vec2 impact;
    Vec2 release;
    Shell shell;
    int destroyed = 0;
    int value = 0;

    int net() const { return value - shell.price(); }
};

inline bool isWorse(const DropSolution &a, const DropSolution &b) {
    if (a.net() != b.net()) return a.net() < b.net();
    return a.shell.price() > b.shell.price();
}

class DropPlanner {
public:
    explicit DropPlanner(std::unique_ptr<ITrajectorySource> trajectory) : trajectory_(std::move(trajectory)) {}

    DropSolution plan(const Scenario &scenario) const {
        const std::vector<DropSolution> candidates = buildCandidates(scenario.targets());
        const auto best_it = std::max_element(candidates.begin(), candidates.end(), isWorse);

        DropSolution best = best_it != candidates.end() ? *best_it : DropSolution{};
        best.release = trajectory_->release(best.impact, scenario.altitude(), scenario.wind());
        return best;
    }

private:
    static std::vector<DropSolution> buildCandidates(const std::vector<Target> &targets) {
        std::vector<DropSolution> candidates;
        candidates.reserve(targets.size() * kShells.size());

        for (const Target &aim: targets) {
            for (const Shell &shell: kShells) {
                candidates.push_back(evaluate(aim.position(), shell, targets));
            }
        }
        return candidates;
    }

    static DropSolution evaluate(const Vec2 &impact, const Shell &shell, const std::vector<Target> &targets) {
        DropSolution candidate;
        candidate.impact = impact;
        candidate.shell = shell;
        for (const Target &target: targets) {
            if (shell.destroys(impact, target)) {
                candidate.destroyed++;
                candidate.value += target.value();
            }
        }
        return candidate;
    }

    static inline const std::array<Shell, 3> kShells = {{
        {"Frag", 60, 25, 35},
        {"HE", 35, 55, 20},
        {"HEAT", 15, 130, 45},
    }};

    std::unique_ptr<ITrajectorySource> trajectory_;
};
