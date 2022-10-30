#ifndef WIDGET_H
#define WIDGET_H

#include <centurion.hpp>

class Widget
{
    enum Position {VERTICAL, HORIZONTAL, SLOPE};
    int size;
    Widget::Position pos;
public:
    Widget(int size = 10, Widget::Position pos = Widget::HORIZONTAL);
    friend std::ostream& operator<<(std::ostream& os, const Widget &w);
    void render(cen::renderer &renderer, int x, int y);
    void destroy();
};

#endif // WIDGET_H