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


// ----------------- Operator overloading -----------------
void Widget::display_attributes(std::ostream& os) const
{
    os << "size: " << size;
}

std::ostream& operator<<(std::ostream& os, const Widget &w)
{
    os << w.display_name() << " (";
    w.display_attributes(os); //FIXME: maybe return ostream for chaining?
    os << ")";

    // TODO: better format for children tree
    if (!w.children.empty()) {
        os << ", with children: (\n";
        for (const auto &child : w.children) {
            os << "  - at position " << child.first << " with address " << child.second << ": " << *child.second;
        }
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
    // TODO: check if widget fits
    children.emplace_back(pos, w->clone());
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
