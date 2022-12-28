#include "Container.h"
#include "Utilities.h"

Container::Container(cen::iarea size, cen::color background_color, SizingPolicy policy) :
    Widget{size, policy}, background_color{background_color}
{}

Container::Container(const Container &other)
    : Widget(other), background_color{other.background_color}
{
    for (const auto &[child, pos, alloc] : other.children) {
        auto child_clone = child->clone();
        child_clone->parent = this;
        children.emplace_back(child_clone, pos, alloc);
    }
}

void swap(Container &first, Container &second)
{
    // enable ADL
    using std::swap;

    // swap attributes
    swap(first.children, second.children);
    swap(first.background_color, second.background_color);
}

void Container::display_attributes(std::ostream& os) const
{
    os << "background-color: " << background_color << ", ";
    Widget::display_attributes(os);
}

void Container::display(std::ostream &os, int nest_level) const
{
    Widget::display(os, nest_level);
    for (const auto &[child, pos, alloc] : children) {
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
        auto& [child, pos, alloc] = children[i];
        pos = get_child_position(i);
        alloc = get_child_allocation(i);
        child->set_allocated_size(alloc);
    }
}

void Container::render(cen::renderer &renderer, cen::ipoint offset) const
{
    Utilities::render_background(renderer, offset, size, background_color);

    for (const auto& [child, pos, alloc] : children) {
        child->render(renderer, offset + pos);
    }
}

Widget::children_vector Container::get_children() const
{
    return children;
}

void Container::add_child(const std::shared_ptr<Widget> &w)
{
    auto pos = get_child_position(children.size());
    auto alloc = get_child_allocation(children.size());

    auto child_clone = w->clone();
    child_clone->parent = this;
    child_clone->set_allocated_size(alloc);
    children.emplace_back(child_clone, pos, alloc);
}
