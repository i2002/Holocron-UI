#include "Window.h"
#include <iostream>

Window::Window(const std::string &t, const cen::iarea size, const cen::color &color) :
    Widget{size},
    win{t, size},
    renderer{win.make_renderer()}
{
    this->title = t;
    this->color = color;
    win.show();
}

Window::Window(const Window &w) :
    Widget{w},
    title{w.title},
    color{w.color},
    win{"AAAA", size}, //FIXME: remove debug messages
    renderer{win.make_renderer()}
{
    std::cout << "window cc\n"; //FIXME: remove debug messages
}

Window::~Window()
{
    win.hide();
    std::cout << "window destrcutor\n"; // FIXME: remove debug messages
}

Widget* Window::clone() const
{
    std::cout << "window clone\n"; // FIXME: remove debug messages
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
       << "title: \"" << title << "\", "
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
