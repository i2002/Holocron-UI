#include "Window.h"

Window::Window(const std::string &t, const cen::iarea &size, const cen::color &color) :
    Widget{size},
    win{t, size},
    renderer{win.make_renderer()}
{
    this->color = color;
}

Window::~Window()
{
    win.hide();
}

Window::Window(const Window &w) :
    Widget{w},
    color{w.color},
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
    swap(static_cast<Widget&>(first), static_cast<Widget&>(second));
    swap(first.win, second.win);
    swap(first.renderer, second.renderer);
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
    children[0].second->set_allocated_size(size); // FIXME: window should have it's own allocation

    // parent behaviour (render children)
    Widget::render(renderer, {0, 0});

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
