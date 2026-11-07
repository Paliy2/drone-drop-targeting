#ifndef CASE02_BUDGET_H
#define CASE02_BUDGET_H

#include <vector>

// Кожна одиниця цінності цілі відповідає такій кількості вагових одиниць
// у зведеному показнику місії.
const int WEIGHT_PER_VALUE = 5000000;

// Сумарний ваговий показник усіх цілей сценарію.
long long total_weight(const std::vector<int>& values);

// Середній ваговий показник на одну ціль.
long long average_weight(const std::vector<int>& values);

#endif  // CASE02_BUDGET_H
