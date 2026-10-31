#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "include/drop_planner.h"
#include "include/scenario_io.h"
#include "include/trajectory.h"

const Vec2 kForwardDroneVelocity{12.0, 0.0};

struct CliOptions {
    std::string input_file;
    std::string output_file;
    std::string trajectory = "hover";
};

CliOptions parseArgs(int argc, char *argv[]) {
    CliOptions options;
    for (int i = 1; i < argc; i++) {
        const std::string arg = argv[i];
        if (arg == "--input-file" && i + 1 < argc) {
            options.input_file = argv[++i];
        } else if (arg == "--output-file" && i + 1 < argc) {
            options.output_file = argv[++i];
        } else if (arg == "--trajectory" && i + 1 < argc) {
            options.trajectory = argv[++i];
        }
    }
    return options;
}

std::unique_ptr<ITrajectorySource> makeTrajectory(const std::string &name) {
    if (name == "hover") return std::make_unique<HoverTrajectory>();
    if (name == "forward") return std::make_unique<ForwardThrowTrajectory>(kForwardDroneVelocity);
    return nullptr;
}

int main(int argc, char *argv[]) {
    const CliOptions options = parseArgs(argc, argv);

    if (options.input_file.empty() || options.output_file.empty()) {
        std::cerr << "Usage: " << argv[0]
                  << " --input-file <scenario.txt> --output-file <result.txt> [--trajectory hover|forward]\n";
        return 1;
    }

    std::unique_ptr<ITrajectorySource> trajectory = makeTrajectory(options.trajectory);
    if (trajectory == nullptr) {
        std::cerr << "Unknown trajectory: " << options.trajectory << " (expected hover|forward)\n";
        return 1;
    }

    try {
        const DropPlanner planner(std::move(trajectory));
        const Scenario scenario = ScenarioReader(options.input_file).read();

        const auto started = std::chrono::steady_clock::now();
        const DropSolution solution = planner.plan(scenario);
        const auto finished = std::chrono::steady_clock::now();

        std::cout << "elapsed_us "
                  << std::chrono::duration_cast<std::chrono::microseconds>(finished - started).count() << "\n";
        ResultWriter(options.output_file).write(solution);
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
