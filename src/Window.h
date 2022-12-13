#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <centurion.hpp>
#include "Widget.h"


class Window : public Widget
{
    std::string title;
    cen::color color;

    cen::window win;
    cen::renderer renderer;

public:
    explicit Window(const std::string &t = "Untitled window", const cen::iarea &size = {1000, 500}, const cen::color &color = cen::colors::white);

    std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;
    void render();
    
    // friend std::ostream& operator<<(std::ostream& os, const Window &w);
    // Window& operator=(const Window &other);

protected:
    Window(const Window &other);
    void render_self(cen::renderer &, cen::ipoint) const override {};
};

#endif // WINDOW_H
