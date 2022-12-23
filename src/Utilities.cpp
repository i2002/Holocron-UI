#include "Utilities.h"

void Utilities::render_background(cen::renderer &renderer, const cen::ipoint &offset, const cen::iarea &size,
                                  const cen::color &color)
{
    cen::color prev_color = renderer.get_color();
    renderer.set_color(color);
    renderer.fill_rect(cen::irect{offset, size});
    renderer.set_color(prev_color);
}
