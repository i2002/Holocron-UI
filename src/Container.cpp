#include "Container.h"

Container::Container(cen::iarea size, SizingPolicy policy) :
    Widget{size, policy}
{}

Container::Container(const Container &other)
    : Widget(other)
{
    for (const auto &child : other.children) {
        children.emplace_back(child->clone());
    }
}

void swap(Container &first, Container &second)
{
    // enable ADL
    using std::swap;

    // swap attributes
    swap(first.children, second.children);
}

void Container::display_attributes(std::ostream& os) const
{
    Widget::display_attributes(os);
}

void Container::display(std::ostream &os, int nest_level) const
{
    Widget::display(os, nest_level);
    for (const auto &child : children) {
        for (int i = 0; i < nest_level; i++) {
            os << "  ";
        }
        os << "- ";
        child->display(os, nest_level + 1);
    }
}

void Container::set_allocated_size(cen::iarea size_)
{
    Widget::set_allocated_size(size_);

    for (size_t i = 0; i < children.size(); i++) {
        children[i]->set_allocated_size(get_child_allocation(i));
    }
}

void Container::render(cen::renderer &renderer, cen::ipoint offset) const
{
    // FIXME: render background

    // FIXME: do I need to render container specific info?
    render_self(renderer, offset);

    for (size_t i = 0; i < children.size(); i++) {
        children[i]->render(renderer, offset + get_child_position(i));
    }
}

void Container::add_child(const std::shared_ptr<Widget> &w)
{
    w->parent = this;
    w->set_allocated_size(get_child_allocation(children.size() + 1));
    children.emplace_back(w->clone());
}
