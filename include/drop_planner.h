#pragma once

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

class DropPlanner {
public:
    explicit DropPlanner(ITrajectorySource *trajectory) : trajectory_(trajectory) {}
    ~DropPlanner() { delete trajectory_; }

    DropPlanner(const DropPlanner &) = delete;
    DropPlanner &operator=(const DropPlanner &) = delete;

    DropSolution plan(const Scenario &scenario) const {
        DropSolution best;
        bool has_best = false;

        for (const Shell &shell: kShells) {
            for (const Target &aim: scenario.targets()) {
                int destroyed = 0;
                int value_sum = 0;

                for (const Target &target: scenario.targets()) {
                    if (shell.destroys(aim.position(), target)) {
                        destroyed++;
                        value_sum += target.value();
                    }
                }

                int net = value_sum - shell.price();
                bool is_better = !has_best || net > best.net() ||
                                 (net == best.net() && shell.price() < best.shell.price());

                if (is_better) {
                    has_best = true;
                    best.destroyed = destroyed;
                    best.value = value_sum;
                    best.shell = shell;
                    best.impact = aim.position();
                }
            }
        }

        best.release = trajectory_->release(best.impact, scenario.altitude(), scenario.wind());
        return best;
    }

private:
    static inline const Shell kShells[] = {
        {"Frag", 60, 25, 35},
        {"HE", 35, 55, 20},
        {"HEAT", 15, 130, 45},
    };

    ITrajectorySource *trajectory_;
};
