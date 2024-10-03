#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

// Color represented by 3 element 256 valued vector
using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Scale to [0,255]
    static const interval intensity(0.000, 0.999);
    int rb = int(256 * intensity.clamp(r));
    int gb = int(256 * intensity.clamp(g));
    int bb = int(256 * intensity.clamp(b));

    // Write pixel components
    out << rb << ' ' << gb << ' ' << bb << '\n';
}

#endif