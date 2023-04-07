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
    void set_focused_widget(Widget *widget);

protected:
    [[nodiscard]] cen::ipoint get_child_position(size_t index) const override;
    [[nodiscard]] cen::iarea get_child_allocation(size_t index, std::shared_ptr<Widget> child) const override;

    // cppcheck-suppress duplInheritedMember
    EventDispatcher<std::string> dispatcher;

private:
    cen::window win;
    cen::renderer renderer;
    Widget* focused_widget = nullptr;

    template <typename Event>
    void process_focussed_event(Event &event);
};

template<typename Event>
void Window::process_focussed_event(Event &event) {
    if (focused_widget == nullptr) {
        return;
    }

    focused_widget->process_event(event, *this);
}

#endif // WINDOW_H
