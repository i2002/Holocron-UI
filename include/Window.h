#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <centurion.hpp>
#include "Widget.h"

// FIXME: struct?
/*
struct WidgetPlacement
{
    int x;
    int y;
    Widget widget;
}
*/

class Window
{
    std::string title;
    cen::color color;
    std::vector<std::tuple<int, int, Widget>> widgets;

public:
    Window(const Window &w);
    Window(std::string title = "Untitled window", const cen::color &color = cen::colors::white);
    ~Window();
    
    friend std::ostream& operator<<(std::ostream& os, const Window &w);
    Window& operator=(const Window &other);

    void render(cen::renderer &renderer);
    
    // TODO:
    // - render (including rendering child widgets)
    // - listen to events / propagate events
    // - 
};

#endif // WINDOW_H
