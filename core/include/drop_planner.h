#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "mission.h"
#include "provided_coverage.h"
#include "trajectory.h"

struct DropSolution {
    Vec2 impact;
    Vec2 release;
    Shell shell;
    int destroyed = 0;
    int value = 0;

    int net() const { return value - shell.price(); }
};

bool isWorse(const DropSolution &a, const DropSolution &b);

class DropPlanner {
public:
    explicit DropPlanner(std::unique_ptr<ITrajectorySource> trajectory);

    // Precondition: scenario has at least one target; throws std::invalid_argument otherwise.
    DropSolution plan(const Scenario &scenario) const;

private:
    std::vector<DropSolution> buildCandidates(const std::vector<Target> &targets) const;
    DropSolution evaluate(const Vec2 &impact, std::size_t shell_index, const std::vector<Target> &targets) const;

    std::unique_ptr<ITrajectorySource> trajectory_;
    CoverageChecker coverage_;
};
