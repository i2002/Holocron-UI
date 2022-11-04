#ifndef WIDGET_H
#define WIDGET_H

#include <centurion.hpp>

class Widget
{
public:
    enum Position {VERTICAL, HORIZONTAL, DIAGONAL};

private:
    int size;
    Widget::Position pos;

public:
    explicit Widget(int size = 10, Widget::Position pos = Widget::HORIZONTAL);
    friend std::ostream& operator<<(std::ostream& os, const Widget &w);
    void render(cen::renderer &renderer, int x, int y);
    void destroy();
};

#endif // WIDGET_H