// Випадок 3. Профіль вітру з метеозведення та рішення про скид.
#include <iomanip>
#include <iostream>
#include <string>

#include "wind.h"

int main() {
    const std::string report = "4.0 -3.0";

    WindProfile* wind = load_profile(report);

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "метеозведення:  " << report << "\n";
    std::cout << "вітер X:        " << wind->x << " м/с\n";
    std::cout << "вітер Y:        " << wind->y << " м/с\n";
    std::cout << "поривчастість:  " << wind->gust_mps << " м/с\n";
    std::cout << "----\n";
    std::cout << "рішення:        " << release_mode(*wind) << "\n";

    delete wind;
    return 0;
}
