#include <iostream>
#include "Window.h"

Window::Window(const std::string &t, const cen::iarea &size, const cen::color &color) :
    Container{size, color},
    win{t, size, cen::window::window_flags::resizable},
    renderer{win.make_renderer()}
{
    add_event_handler<cen::mouse_button_event>([this](cen::mouse_button_event, EventActions&) {
        std::cout << "> Dispatch window event\n";
        dispatcher.run_handlers(std::string("message"));
    });

    add_event_handler<cen::mouse_motion_event>([this](cen::mouse_motion_event event, EventActions&) {
        set_hover(cen::ipoint{event.x(), event.y()});
    });
}

Window::~Window()
{
    win.hide();
}

Window::Window(const Window &other) :
    Container{other},
    win{other.win.title(), size},
    renderer{win.make_renderer()}
{
}

Window& Window::operator=(Window other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Widget> Window::clone() const
{
    return std::make_shared<Window>(*this);
}

void swap(Window &first, Window &second)
{
    // enable ADL
    using std::swap;

    // swap attributes
    swap(static_cast<Container&>(first), static_cast<Container&>(second));
    swap(first.win, second.win);
    swap(first.renderer, second.renderer);
}

std::string Window::display_name() const
{
    return "Window";
}

void Window::display_attributes(std::ostream& os) const
{
    os << "title: \"" << win.title() << "\", ";
    Container::display_attributes(os);
}

void Window::render_window()
{
    // window background color
    renderer.clear_with(background_color);

    // parent behaviour (render children)
    Container::render(renderer, {0, 0});

    // present rendered items
    renderer.present();
}

void Window::show()
{
    win.show();
}

void Window::hide()
{
    win.hide();
}

void Window::set_child(const std::shared_ptr<Widget> &w)
{
    if (!children.empty()) {
        children.pop_back();
    }

    Container::add_child(w);
}

void Window::set_focused_widget(Widget *widget) {
    if (focused_widget != nullptr) {
        focused_widget->set_focused(false);
    }

    focused_widget = widget;

    if (focused_widget != nullptr) {
        focused_widget->set_focused(true);
    }
}

cen::ipoint Window::get_child_position(size_t) const
{
    return {0, 0};
}

cen::iarea Window::get_child_allocation(size_t, std::shared_ptr<Widget>) const
{
    return size;
}
