#include "Container.h"
#include "Utilities.h"

Container::Container(cen::iarea size, cen::color background_color, SizingPolicy policy) :
    Widget{size, policy}, background_color{background_color}
{}

Container::Container(const Container &other)
    : Widget(other), background_color{other.background_color}
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
    Utilities::render_background(renderer, offset, size, background_color);

    for (size_t i = 0; i < children.size(); i++) {
        children[i]->render(renderer, offset + get_child_position(i));
    }
}

bool Container::propagate_event(const cen::event_handler &event, cen::ipoint position)
{
    bool bubbling_cancelled = false;
    if (event.is(cen::event_type::mouse_button_down)) {
        for (size_t i = 0; i < children.size(); i++) {
            auto child_pos = get_child_position(i);
            cen::irect child_rect{child_pos, children[i]->get_allocated_size()};
            if (child_rect.contains(position)) {
                bubbling_cancelled = bubbling_cancelled || children[i]->process_event(event, position - child_pos);
            }
        }
    }

    return bubbling_cancelled;
}

void Container::add_child(const std::shared_ptr<Widget> &w)
{
    w->parent = this;
    w->set_allocated_size(get_child_allocation(children.size() + 1));
    children.emplace_back(w->clone());
}
