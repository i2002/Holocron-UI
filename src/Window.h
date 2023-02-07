#ifndef WINDOW_H
#define WINDOW_H

#include "Container.h"
#include <memory>
#include <string>
#include <centurion.hpp>

class Window : public Container
{
    friend class Application;
    friend class Widget;

public:
    explicit Window(const std::string &t = "Untitled window", const cen::iarea &size = {1000, 500}, const cen::color &color = cen::colors::white);
    ~Window() override;
    Window(const Window &other);
    Window(Window &&other) = default;

    Window& operator=(Window other);
    Window &operator=(Window &&other) = default;
    [[nodiscard]] std::shared_ptr<Widget> clone() const override;
    friend void swap(Window &first, Window &second);

    [[nodiscard]] std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;

    void render_window();
    void show();
    void hide();
    void set_child(const std::shared_ptr<Widget> &w);

protected:
    [[nodiscard]] cen::ipoint get_child_position(size_t index) const override;
    [[nodiscard]] cen::iarea get_child_allocation(size_t index) const override;

    // cppcheck-suppress duplInheritedMember
    EventDispatcher<std::string> dispatcher;

private:
    cen::window win;
    cen::renderer renderer;
};

#endif // WINDOW_H
