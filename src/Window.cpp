#include "Window.h"

// Window::Window(const Window &w) :
//     title{w.title},
//     color{w.color},
//     widgets{w.widgets},
//     win{w.title, cen::iarea{1000, 500}},
//     renderer{win.make_renderer()}
// {
// }

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
}

// std::ostream& operator<<(std::ostream &os, const Window &w)
// {
//     os << "Window with title: \"" << w.title <<"\" and widgets: {\n";
//     for(auto it: w.widgets) {
//         os << "- at x: " << std::get<0>(it) << " and y: " << std::get<1>(it) << " " << std::get<2>(it);
//     }
//     os << "}\n";
//     return os;
// }

// Window& Window::operator=(const Window &other)
// {
//     title = other.title;
//     color = other.color;
//     widgets.clear();
//     for(auto it : other.widgets) {
//         widgets.push_back(it); // FIXME: shallow copy vs full copy?
//     }

//     return *this;
// }

void Window::render()
{
    // window background color
    renderer.clear_with(color);

    // parent behaviour (render children)
    Widget::render(renderer, {0, 0});

    // present rendered items
    renderer.present();
}
