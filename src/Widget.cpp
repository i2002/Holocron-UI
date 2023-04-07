#include "Widget.h"
#include "Window.h"
#include <iostream>

// ------------- Constructors and destructors -------------
Widget::Widget(cen::iarea size, SizingPolicy policy) :
    size{size}, sizing_policy{policy}
{
    register_handlers();
}

Widget::Widget(const Widget &other) :
    size{other.size}, allocated_size{other.allocated_size}, sizing_policy{other.sizing_policy}, parent{other.parent}
{
    register_handlers();
}

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
       << "hover: " << hover << ", "
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

/*void Widget::set_size(cen::iarea size_)
{
    size = size_;
}*/

/*cen::iarea Widget::get_allocated_size() const
{
    return allocated_size;
}*/

void Widget::set_allocated_size(cen::iarea size_)
{
    allocated_size = size_;

    if (sizing_policy == SizingPolicy::FIT_PARENT) {
        size = allocated_size;
    }
}

void Widget::set_hover(std::variant<bool, cen::ipoint> state)
{
    if (auto val = std::get_if<bool>(&state)) {
        hover = *val;
        if (!hover) {
            active = false;
        }
    } else {
        hover = true;
    }
}

bool Widget::focusable() {
    return false;
}

/*bool Widget::get_focused() {
    return focused;
}*/

void Widget::set_focused(bool state)
{
    focused = state;
}

Widget::children_vector Widget::get_children() const
{
    return {};
}

// ------------------ Event handling -----------------
// Default event handlers
void Widget::register_handlers()
{
    add_event_handler<cen::mouse_button_event>([this](cen::mouse_button_event event, EventActions&) {
        #ifdef HOLOCRONUI_DEBUG_MESSAGES
        std::cout << "mouse " << (event.pressed() ? "pressed" : "released") << " at position: " << event.position() << " on window " << event.window_id() <<
                  " and widget: " << display_name() << " (";
        display_attributes(std::cout);
        std::cout << ")\n";
        #endif // HOLOCRONUI_DEBUG_MESSAGES

        active = event.pressed();
    });
}

// Event propagation
template<>
bool Widget::propagate_event(cen::mouse_button_event &event, const std::shared_ptr<Widget> &, cen::ipoint pos, cen::iarea alloc)
{
    cen::irect child_rect{pos, alloc};
    if (child_rect.contains(event.position())) {
        event.set_x(event.x() - pos.x());
        event.set_y(event.y() - pos.y());
        return true;
    }
    return false;
}

template<>
bool Widget::propagate_event(cen::mouse_motion_event &event, const std::shared_ptr<Widget> &, cen::ipoint pos, cen::iarea alloc)
{
    cen::irect child_rect{pos, alloc};
    if (child_rect.contains({event.x(), event.y()})) {
        event.set_x(event.x() - pos.x());
        event.set_y(event.y() - pos.y());
        return true;
    }
    return false;
}

// Widget focus
template<>
void Widget::process_focus(cen::mouse_button_event &event, Window &root) {
    if (event.released() && event.clicks() == 1) {
        if (focusable()) {
            root.set_focused_widget(this);
        } else {
            root.set_focused_widget(nullptr);
        }
    }
}
