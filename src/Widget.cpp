#include "Widget.h"

// ------------- Constructors and destructors -------------
Widget::Widget(cen::iarea size, SizingPolicy policy) :
    size{size}, sizing_policy{policy}
{}

Widget::Widget(const Widget &other) :
    parent{other.parent}, size{other.size}, sizing_policy{other.sizing_policy}
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
    swap(first.sizing_policy, second.sizing_policy);
}


// ----------------- Operator overloading -----------------
void Widget::display_attributes(std::ostream& os) const
{
    os << "size: " << size
       << ", sizing policy: " << static_cast<int>(sizing_policy);
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
cen::iarea Widget::get_size() const
{
    return size;
}

void Widget::set_size(cen::iarea size_)
{
    size = size_;
}

cen::iarea Widget::get_allocated_size() const
{
    return allocated_size;
}

void Widget::set_allocated_size(cen::iarea size_)
{
    allocated_size = size_;

    if (sizing_policy == SizingPolicy::FIT_PARENT) {
        size = allocated_size;
    }
}

// FIXME: where to put generic child management?
// (context: usually container widgets have children and they are responsible for the way they are added and sized)
// (maybe create a container derived class for all containers)
void Widget::add_child(const std::shared_ptr<Widget> &w, cen::ipoint pos)
{
    if (check_collisions(w, pos)) {
        return;
    }

    w->set_allocated_size(w->get_size());
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

bool Widget::check_collisions(const std::shared_ptr<Widget> &w, cen::ipoint pos) const
{
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
