#include "Widget.h"
#include <iostream>

// ------------- Constructors and destructors -------------
Widget::Widget(cen::iarea size, SizingPolicy policy) :
    size{size}, sizing_policy{policy}
{}

void swap(Widget &first, Widget &second)
{
    // enable ADL
    using std::swap;

    // swap attributes
    swap(first.parent, second.parent);
    swap(first.size, second.size);
    swap(first.sizing_policy, second.sizing_policy);
}


// ----------------- Operator overloading -----------------
void Widget::display_attributes(std::ostream& os) const
{
    os << "size: " << size << ", "
       << "sizing policy: " << static_cast<int>(sizing_policy);
}

void Widget::display(std::ostream &os, int) const
{
    os << display_name() << " (";
    display_attributes(os);
    os << ")\n";
}

std::ostream& operator<<(std::ostream& os, const Widget &w)
{
    w.display(os, 0);
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

// Event handling
bool Widget::process_event(const cen::event_handler &event, cen::ipoint position)
{
    // TODO: capture handlers (+ check for cancellation)
    if (event.is(cen::event_type::mouse_button_down)) {
        const auto& btn_down = event.get<cen::mouse_button_event>();
        // FIXME: remove debug messages
        std::cout << "(capture): mouse click at coordinates x: " << btn_down.x() << " and y: " << btn_down.y() <<  " and relative position: " << position << " on window: " << btn_down.window_id() << ", on widget:" << display_name() << "\n";
    }

    bool bubbling_cancelled = propagate_event(event, position);

    if (bubbling_cancelled) {
        return true;
    }

    // TODO: bubbling handlers (+ check for cancellation)
    if (event.is(cen::event_type::mouse_button_down)) {
        const auto& btn_down = event.get<cen::mouse_button_event>();
        // FIXME: remove debug messages
        std::cout << "(bubbling): mouse click at coordinates x: " << btn_down.x() << " and y: " << btn_down.y() <<  " and relative position: " << position << " on window: " << btn_down.window_id() << ", on widget:" << display_name() << "\n";
    }

    return false;
}
