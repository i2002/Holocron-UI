#ifndef UTILITIES_H
#define UTILITIES_H

#include <centurion.hpp>

class Utilities
{
    Utilities() = delete;
public:
    static void render_background(cen::renderer &renderer, const cen::ipoint &offset, const cen::iarea &size, const cen::color &color);
};

#endif // UTILITIES_H
