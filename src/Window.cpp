#include "Window.h"

Window::Window(const Window &w) :
    title{w.title},
    color{w.color},
    widgets{w.widgets},
    win{w.title, cen::iarea{1000, 500}},
    renderer{win.make_renderer()}
{
}

Window::Window(const std::string &t, const cen::color &color) :
    win{t, cen::iarea{1000, 500}},
    renderer{win.make_renderer()}
{
    this->title = t;
    this->color = color;
    win.show();
}

Window::~Window()
{
    win.hide();
    for(auto it : widgets) {
        std::get<2>(it).destroy();
    }
}

std::ostream& operator<<(std::ostream &os, const Window &w)
{
    os << "Window with title: \"" << w.title <<"\" and widgets: {\n";
    for(auto it: w.widgets) {
        os << "- at x: " << std::get<0>(it) << " and y: " << std::get<1>(it) << " " << std::get<2>(it);
    }
    os << "}\n";
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

void Window::render()
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

void Window::add_widget(Widget w, int x, int y)
{
    widgets.push_back({x, y, w});
}
