#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

// Color represented by 3 element 256 valued vector
using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Scale to [0,255]
    int rb = int(255.999 * r);
    int gb = int(255.999 * g);
    int bb = int(255.999 * b);

    // Write pixel components
    out << rb << ' ' << gb << ' ' << bb << '\n';
}

#endif