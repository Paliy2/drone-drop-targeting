// Випадок 1. Згладжування показів висотоміра: програма тримає останні
// чотири зразки й друкує їхнє середнє.
#include <iomanip>
#include <iostream>

#include "sample_buffer.h"

int main() {
    const double altitudes[] = {
        118.4, 119.0, 120.2, 121.1, 120.7, 119.8, 118.9, 118.2, 117.6, 117.1,
    };

    SampleBuffer window(4);

    std::cout << std::fixed << std::setprecision(2);
    for (double a : altitudes) {
        window.push(a);
        std::cout << "новий зразок " << a
                  << " | у вікні " << window.size()
                  << " | середнє " << window.average() << "\n";
    }

    std::cout << "----\n";
    std::cout << "останній зразок: " << window.newest() << "\n";
    std::cout << "згладжена висота: " << window.average() << " м\n";
    return 0;
}
