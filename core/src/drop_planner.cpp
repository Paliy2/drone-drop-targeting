#include "drop_planner.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace {

const std::array<Shell, 3> kShells = {{
    {"Frag", 60, 25, 35},
    {"HE", 35, 55, 20},
    {"HEAT", 15, 130, 45},
}};

std::array<double, kShells.size()> shellRadii() {
    std::array<double, kShells.size()> radii{};
    for (std::size_t i = 0; i < kShells.size(); i++) {
        radii[i] = kShells[i].radius();
    }
    return radii;
}

CoverageChecker makeCoverage() {
    const std::array<double, kShells.size()> radii = shellRadii();
    return CoverageChecker(radii.data(), radii.size());
}

}  // namespace

bool isWorse(const DropSolution &a, const DropSolution &b) {
    if (a.net() != b.net()) return a.net() < b.net();
    return a.shell.price() > b.shell.price();
}

DropPlanner::DropPlanner(std::unique_ptr<ITrajectorySource> trajectory)
    : trajectory_(std::move(trajectory)), coverage_(makeCoverage()) {}

DropSolution DropPlanner::plan(const Scenario &scenario) const {
    if (scenario.targets().empty()) {
        throw std::invalid_argument("empty scenario");
    }

    const std::vector<DropSolution> candidates = buildCandidates(scenario.targets());
    DropSolution best = *std::max_element(candidates.begin(), candidates.end(), isWorse);
    best.release = trajectory_->release(best.impact, scenario.altitude(), scenario.wind());
    return best;
}

std::vector<DropSolution> DropPlanner::buildCandidates(const std::vector<Target> &targets) const {
    std::vector<DropSolution> candidates;
    candidates.reserve(targets.size() * kShells.size());

    for (const Target &aim: targets) {
        for (std::size_t shell_index = 0; shell_index < kShells.size(); shell_index++) {
            candidates.push_back(evaluate(aim.position(), shell_index, targets));
        }
    }
    return candidates;
}

DropSolution DropPlanner::evaluate(const Vec2 &impact, std::size_t shell_index,
                                   const std::vector<Target> &targets) const {
    const Shell &shell = kShells[shell_index];
    DropSolution candidate;
    candidate.impact = impact;
    candidate.shell = shell;
    for (const Target &target: targets) {
        const Vec2 delta = target.position() - impact;
        if (coverage_.covers(shell_index, delta.x, delta.y) && shell.pierces(target)) {
            candidate.destroyed++;
            candidate.value += target.value();
        }
    }
    return candidate;
}
