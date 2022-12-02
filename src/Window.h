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
    // Window(const Window &w);
    explicit Window(const std::string &t = "Untitled window", const cen::color &color = cen::colors::white);
    ~Window();

    void render();
    
    // friend std::ostream& operator<<(std::ostream& os, const Window &w);
    // Window& operator=(const Window &other);

protected:
    void render_self(cen::renderer &, cen::ipoint) {};
};

#endif // WINDOW_H
