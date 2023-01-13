#include <iostream>
#include "Window.h"

Window::Window(const std::string &t, const cen::iarea &size, const cen::color &color) :
    Container{size, color},
    win{t, size, cen::window::window_flags::resizable},
    renderer{win.make_renderer()}
{
    add_event_handler<cen::mouse_button_event>([this](cen::mouse_button_event) {
        std::cout << "> Dispatch window event\n";
        dispatcher.run_handlers(std::string("message"));
        return false;
    });

    add_event_handler<cen::mouse_motion_event>([this](cen::mouse_motion_event event) {
        set_hover(true, {event.x(), event.y()});
        return false;
    });
}

Window::~Window()
{
    win.hide();
}

Window::Window(const Window &w) :
    Container{w},
    win{w.win.title(), size},
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

cen::ipoint Window::get_child_position(size_t) const
{
    return {0, 0};
}

cen::iarea Window::get_child_allocation(size_t) const
{
    return size;
}
