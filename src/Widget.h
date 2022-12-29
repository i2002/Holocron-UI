#ifndef WIDGET_H
#define WIDGET_H

#include <centurion.hpp>
#include <memory>
#include <string>
#include "EventDispatcher.h"

template<typename ...Events>
using WidgetEventDispatcher = EventDispatcher<cen::mouse_button_event, cen::mouse_motion_event, Events...>;


class Widget
{
    friend class Container;

protected:
    using children_vector = std::vector<std::tuple<std::shared_ptr<Widget>, cen::ipoint, cen::iarea>>;
    enum class SizingPolicy {FIXED_SIZE, FIT_PARENT};

public:
    explicit Widget(cen::iarea size = {0, 0}, SizingPolicy policy = SizingPolicy::FIXED_SIZE);
    virtual ~Widget() = default;
    Widget(Widget &&other) = default;

    Widget &operator=(Widget &&other) = default;
    [[nodiscard]] virtual std::shared_ptr<Widget> clone() const = 0;
    friend void swap(Widget &first, Widget &second);

    [[nodiscard]] virtual std::string display_name() const = 0;
    virtual void display_attributes(std::ostream& os) const;
    virtual void display(std::ostream& os, int nest_level) const;
    friend std::ostream& operator<<(std::ostream& os, const Widget &w);

/*    [[nodiscard]] cen::iarea get_size() const;
    void set_size(cen::iarea size);*/
    /*[[nodiscard]] cen::iarea get_allocated_size() const;*/
    virtual void set_allocated_size(cen::iarea size);

    template <typename Event, class Base = Widget>
    void add_event_handler(EventDispatcher<>::handler_type<Event> hand) {
        dynamic_cast<Base *>(this)->dispatcher.template add_handler<>(hand);
    }

    template <typename Event, class Base = Widget, typename Data>
    void add_event_handler(EventDispatcher<>::handler_type_data<Event, std::type_identity_t<Data>> hand, Data data) {
        dynamic_cast<Base *>(this)->dispatcher.template add_handler<>(hand, data);
    }

protected:
    // prevent object slicing when copying
    Widget(const Widget &other);

    // TODO: surface rendering: every widget has it's own surface that is merged by the parent + surface caching
    virtual void render(cen::renderer &renderer, cen::ipoint offset) const = 0;
    // TODO: implement resizing request (when internal widget structure changed)

    [[nodiscard]] virtual children_vector get_children() const;
    void register_handlers();

    template<typename Event>
    bool process_event(Event);

    template <typename Event>
    bool propagate_event(Event &event, const std::shared_ptr<Widget> &w, cen::ipoint pos, cen::iarea alloc);

    // TODO: sizing based on widget size requirement? (-> scrolling)
    // TODO: margins in relation to parent (px or %)
    cen::iarea size;
    cen::iarea allocated_size;
    SizingPolicy sizing_policy;
    Widget *parent = nullptr;
    WidgetEventDispatcher<> dispatcher;
};

// Template generic implementation
template<typename Event>
bool Widget::process_event(Event event)
{
    bool cancelled = dispatcher.run_handlers(event);
    if (cancelled) {
        return true;
    }

    for (const auto &[w, pos, alloc] : get_children()) {
        if (propagate_event(event, w, pos, alloc)) {
            cancelled = w->process_event(event) || cancelled;
        }
    }

    return cancelled;
}

#endif // WIDGET_H
