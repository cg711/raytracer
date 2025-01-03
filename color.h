/**
 * Casey Gehling
 * 
 * Defines color type -- inherits vec3 type structure.
 */
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

// Color represented by 3 element 256 valued vector
using color = vec3;

inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        return std::sqrt(linear_component);
    }
    return 0;
}

// Write ray color to output stream written into pixel map. Configured only for ppm image type usage.
void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Gamma correction
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Scale to [0,255]
    static const interval intensity(0.000, 0.999);
    int rb = int(256 * intensity.clamp(r));
    int gb = int(256 * intensity.clamp(g));
    int bb = int(256 * intensity.clamp(b));

    // Write pixel components
    out << rb << ' ' << gb << ' ' << bb << '\n';
}

#endif