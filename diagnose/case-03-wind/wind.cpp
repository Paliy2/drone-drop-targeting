#include "wind.h"

#include <sstream>

WindProfile* load_profile(const std::string& report) {
    WindProfile* wind = new WindProfile;

    std::istringstream in(report);
    double x = 0.0;
    double y = 0.0;
    in >> x >> y;

    wind->x = x;
    wind->y = y;
    return wind;
}

std::string release_mode(const WindProfile& wind) {
    if (wind.gust_mps > 12.0) {
        return "затримати скид";
    }
    return "скид дозволено";
}
