#include "Widget.h"

// ------------- Constructors and destructors -------------
Widget::Widget(cen::iarea size, Widget *p) : 
    parent{p}, size{size}
{}

Widget::~Widget()
{
    for (const auto &child : children) {
        delete child.second;
    }
    children.clear();
}

Widget::Widget(const Widget &other) :
    parent{other.parent}, size{other.size}
{
    for (const auto &child : other.children) {
        children.emplace_back(child.first, child.second->clone());
    }
}

void swap(Widget &first, Widget &second)
{
    // enable ADL
    using std::swap;

    // swap attributes
    swap(first.parent, second.parent);
    swap(first.children, second.children);
    swap(first.size, second.size);
}


// ----------------- Operator overloading -----------------
void Widget::display_attributes(std::ostream& os) const
{
    os << "size: " << size;
}

std::ostream& operator<<(std::ostream& os, const Widget &w)
{
    os << w.display_name() << " (";
    w.display_attributes(os); //FIXME: a way to chain these?
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

void Widget::set_size(cen::iarea size_)
{
    size = size_;
}

void Widget::add_child(Widget *w, cen::ipoint pos)
{
    if (check_collisions(w, pos)) {
        return;
    }

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

bool Widget::check_collisions(Widget *w, cen::ipoint pos) {
    cen::iarea widget_size = w->get_size();

    for (const auto& [child_pos, child] : children) {
        cen::iarea child_size = child->get_size();
        bool outside_x = pos.x() >= child_pos.x() + child_size.width || pos.x() + widget_size.width <= child_pos.x();
        bool outside_y = pos.y() >= child_pos.y() + child_size.height || pos.y() + widget_size.height <= child_pos.y();

        if (!(outside_x || outside_y)) {
            return true;
        }
    }

    return false;
}
