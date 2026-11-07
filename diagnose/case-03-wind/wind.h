#ifndef CASE03_WIND_H
#define CASE03_WIND_H

#include <string>

// Профіль вітру в районі цілі.
struct WindProfile {
    double x = 0.0;         // складова вітру по X, м/с
    double y = 0.0;         // складова вітру по Y, м/с
    double gust_mps = 0.0;  // поривчастість, м/с (у метеозведенні немає — штиль)
};

// Готує профіль вітру за даними метеозведення.
// Володіння результатом переходить до викликача.
WindProfile* load_profile(const std::string& report);

// Рішення про скид за поривчастістю.
std::string release_mode(const WindProfile& wind);

#endif  // CASE03_WIND_H
