#include "Window.h"

Window::Window(const std::string &t, const cen::iarea &size, const cen::color &color) :
    Container{size},
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
    Container{w},
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
    Widget::display_attributes(os);
    os << ", "
       << "title: \"" << win.title() << "\", "
       << "color: " << color;
}

void Window::render_window()
{
    // window background color
    renderer.clear_with(color);

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
