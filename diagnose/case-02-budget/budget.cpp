#include "budget.h"

long long total_weight(const std::vector<int>& values) {
    long long total = 0;
    for (int v : values) {
        total += static_cast<long long>(v) * WEIGHT_PER_VALUE;
    }
    return total;
}

long long average_weight(const std::vector<int>& values) {
    if (values.empty()) return 0;
    return total_weight(values) / static_cast<long long>(values.size());
}
