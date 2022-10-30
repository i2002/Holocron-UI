#include "../include/Window.h"

Window::Window(const Window &w) : widgets{w.widgets}
{
    title = w.title;
    color = w.color;
}

Window::Window(std::string title, const cen::color &color)
{
    this->title = title;
    this->color = color;
}

Window::~Window()
{
    for(auto it : widgets) {
        std::get<2>(it).destroy();
    }
}

std::ostream& operator<<(std::ostream &os, const Window &w)
{
    os << "Window with title: \"" << w.title <<"\"\n";
    return os;
}

Window& Window::operator=(const Window &other)
{
    title = other.title;
    color = other.color;
    widgets.clear();
    for(auto it : other.widgets) {
        widgets.push_back(it); // FIXME: shallow copy vs full copy?
    }

    return *this;
}

void Window::render(cen::renderer &renderer)
{
    // window background color
    renderer.clear_with(color);

    // render each widget at it's own position
    for(auto it : widgets) {
        std::get<2>(it).render(renderer, std::get<0>(it), std::get<1>(it));
    }

    // present rendered items
    renderer.present();
}