#include "Widget.h"

Widget::Widget(Widget *p) : parent{p}
{

}

Widget::~Widget()
{
    for(auto &child : children) {
        delete child;
    }
    children.clear();
}

std::ostream& operator<<(std::ostream& os, const Widget &w)
{
    os << "Widget with size: (" << w.allocated_size << ") at position: (" << w.allocated_position << "), with children: (\n";
    for (const auto &child : w.children) {
        os << "  - " << child;
    }
    os << ")\n";
    return os;
}

cen::iarea Widget::get_requested_size()
{
    return requested_size;
}

cen::iarea Widget::get_allocated_size()
{
    return allocated_size;
}

cen::ipoint Widget::get_allocated_position()
{
    return allocated_position;
}

void Widget::set_allocation(cen::iarea allocated_size, cen::ipoint allocated_position)
{
    this->allocated_size = allocated_size;
    this->allocated_position = allocated_position;
}

void Widget::request_resize()
{
    parent->request_resize();
}
