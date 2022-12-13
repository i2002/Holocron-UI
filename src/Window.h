#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <centurion.hpp>
#include "Widget.h"

class Window : public Widget
{
public:
    explicit Window(const std::string &t = "Untitled window", const cen::iarea &size = {1000, 500}, const cen::color &color = cen::colors::white);
    ~Window() override;
    Window(Window &&other) = default;
    Window &operator=(Window &&other) = default;
    [[nodiscard]] Widget *clone() const override;
    friend void swap(Window &first, Window &second);

    [[nodiscard]] std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;

    void render();
    void show();
    void hide();

protected:
    Window(const Window &other);
    Window& operator=(Window other);
    void render_self(cen::renderer &, cen::ipoint) const override {};

private:
    cen::color color;
    cen::window win;
    cen::renderer renderer;
};

#endif // WINDOW_H
