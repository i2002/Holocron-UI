#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <centurion.hpp>
#include "Widget.h"


class Window
{
    std::string title;
    cen::color color;
    std::vector<std::tuple<int, int, Widget>> widgets;
    
    cen::window win;
    cen::renderer renderer;

public:
    Window(const Window &w);
    explicit Window(std::string title = "Untitled window", const cen::color &color = cen::colors::white);
    ~Window();
    
    friend std::ostream& operator<<(std::ostream& os, const Window &w);
    Window& operator=(const Window &other);

    void render();
    void add_widget(Widget w, int x, int y);
};

#endif // WINDOW_H
