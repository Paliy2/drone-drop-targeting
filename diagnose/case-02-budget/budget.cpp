#include "budget.h"

int total_weight(const std::vector<int>& values) {
    int total = 0;
    for (int v : values) {
        total += v * WEIGHT_PER_VALUE;
    }
    return total;
}

int average_weight(const std::vector<int>& values) {
    if (values.empty()) return 0;
    return total_weight(values) / static_cast<int>(values.size());
}
