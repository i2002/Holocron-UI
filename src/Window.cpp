#include "Window.h"

Window::Window(const std::string &t, const cen::iarea &size, const cen::color &color) :
    Widget{size},
    win{t, size},
    renderer{win.make_renderer()}
{
    this->color = color;
}

Window::Window(const Window &w) :
    Widget{w},
    color{w.color},
    win{w.win.title(), size},
    renderer{win.make_renderer()}
{
}

Window::~Window()
{
    win.hide();
    std::cout << "window destrcutor\n"; // FIXME: remove debug messages
}

Widget* Window::clone() const
{
    return new Window(*this);
}

std::string Window::display_name() const
{
    return "Window";
}

void Window::display_attributes(std::ostream& os) const
{
    Widget::display_attributes(os);
    os << ", "
       << "title: \"" << win.title() << "\", "
       << "color: " << color;
}

void Window::render()
{
    // window background color
    renderer.clear_with(color);

    // parent behaviour (render children)
    Widget::render(renderer, {0, 0});

    // present rendered items
    renderer.present();
}
