#include "Widget.h"
#include <iostream> //FIXME: remove debug messages

// ------------- Constructors and destructors -------------
Widget::Widget(cen::iarea size, Widget *p) : 
    parent{p}, size{size}
{

}

Widget::Widget(const Widget &other) :
    parent{other.parent}, size{other.size}
{
    std::cout << "widget cc\n"; //FIXME: remove debug messages
    for (const auto &child : other.children) {
        children.push_back({child.first, child.second->clone()});
    }
}

Widget::~Widget()
{
    std::cout << "widget destructor\n"; //FIXME: remove debug messages
    for (const auto &child : children) {
        delete child.second;
    }
    children.clear();
}


// ---------------- Operator overloading -----------------
std::ostream& operator<<(std::ostream& os, const Widget &w)
{
    // TODO: support for derivate specific info (type name and field information)
    os << "widget with size: (" << w.allocated_size << "), with children: (\n";

    // TODO: better format for children tree
    for (const auto &child : w.children) {
        os << "  - at position: " << child.first << ", "  << child.second;
    }
    os << ")\n";
    return os;
}


// ------------------ Getters and setters -----------------
cen::iarea Widget::get_size()
{
    return size;
}

void Widget::set_size(cen::iarea size)
{
    this->size = size;
}

void Widget::add_child(Widget *w, cen::ipoint pos)
{
    // FIXME: should I clone the widget so that the caller function is responsible for the object created there and here I'm responsile for the copy?
    // TODO: check if widget fits
    children.push_back({pos, w});
}


// ------------------- Internal methods -------------------
void Widget::render(cen::renderer &renderer, cen::ipoint offset)
{
    // render children
    for (const auto& [pos, w] : children) {
        w->render(renderer, offset + pos);
    }

    // render self
    render_self(renderer, offset);
}
