// Випадок 2. Зведений ваговий показник сценарію: цінності цілей
// переводяться у вагові одиниці й підсумовуються.
#include <iostream>
#include <vector>

#include "budget.h"

int main() {
    // Цінності цілей сценарію: 6 × RADAR (90), 4 × TANK (60), 3 × APC (40).
    const std::vector<int> values = {
        90, 90, 90, 90, 90, 90,
        60, 60, 60, 60,
        40, 40, 40,
    };

    int sum_of_values = 0;
    for (int v : values) sum_of_values += v;

    std::cout << "цілей у сценарії:      " << values.size() << "\n";
    std::cout << "сума цінностей:        " << sum_of_values << "\n";
    std::cout << "ваговий коефіцієнт:    " << WEIGHT_PER_VALUE << "\n";
    std::cout << "----\n";
    std::cout << "сумарний показник:     " << total_weight(values) << "\n";
    std::cout << "середній на ціль:      " << average_weight(values) << "\n";
    return 0;
}
