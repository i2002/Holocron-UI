#include "../include/Widget.h"

Widget::Widget(int size, Widget::Position pos)
{
    this->size = size;
    this->pos = pos;
}

std::ostream& operator<<(std::ostream& os, const Widget &w)
{
    os << "Widget with size: " << w.size << " and position: " << w.pos << "\n";
    return os;
}

void Widget::render(cen::renderer &renderer, int x, int y)
{
    int delta_x = pos != Widget::VERTICAL ? size : 0;
    int delta_y = pos != Widget::HORIZONTAL ? size : 0;
    renderer.draw_line(cen::ipoint(x, y), cen::ipoint{x + delta_x, y + delta_y});
}

void Widget::destroy()
{
    // FIXME: implementation
}
