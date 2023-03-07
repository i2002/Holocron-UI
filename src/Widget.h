#ifndef WIDGET_H
#define WIDGET_H

#include <centurion.hpp>
#include <memory>
#include <string>
#include "EventDispatcher.h"

// Generic widget event handler templates
template <typename Event, typename Base>
struct widget_handler_func final
{
    using type = handler_func_t<Event>;
};

template <typename Event, typename Base>
using widget_handler_func_t = typename widget_handler_func<Event, Base>::type;


// Generic conversion between custom event handler and dispatcher
template <typename Event, typename Base>
struct convert_handler final
{
    using event_type = Event;
    static handler_func_t<event_type> convert(const typename widget_handler_func<Event, Base>::type &hand) {
        return hand;
    }
};

template <typename Event, typename Base>
constexpr auto convert_handler_func = convert_handler<Event, Base>::convert;

template <typename Event, typename Base>
using convert_handler_t = typename convert_handler<Event, Base>::event_type;


// Specific widget actions implementation
class EventActions
{
    bool stop = false;

public:
    void stopPropagation() {
        stop = true;
    }

    [[nodiscard]] bool isPropagationStopped() const {
        return stop;
    }
};

template<typename Event>
class WidgetEvent
{
    Event ev;
    EventActions actions;

public:
    explicit WidgetEvent(const Event &ev) : ev{ev} {}

    const Event& getEvent() {
        return ev;
    }

    EventActions& getEventActions() {
        return actions;
    }
};

// specialize Widget handler and converter templates
class Widget;
template <typename Event>
struct widget_handler_func<Event, Widget> final
{
    using type = std::function<void(Event, EventActions&)>;
};

template <typename Event>
struct convert_handler<Event, Widget> final
{
    using event_type = WidgetEvent<Event>;
    static handler_func_t<event_type> convert(const typename widget_handler_func<Event, Widget>::type &hand) {
        return [hand] (WidgetEvent<Event> &event) {
            hand(event.getEvent(), event.getEventActions());
            return false;
        };
    }
};

template<typename ...Events>
using WidgetEventDispatcher = EventDispatcher<WidgetEvent<Events>...>;


// Widget class implementation
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

    [[nodiscard]] cen::iarea get_size() const;
    /*void set_size(cen::iarea size);*/
    /*[[nodiscard]] cen::iarea get_allocated_size() const;*/
    virtual void set_allocated_size(cen::iarea size);
    virtual void set_hover(std::variant<bool, cen::ipoint> state);

    template <typename Event, class Base = Widget>
    void add_event_handler(widget_handler_func_t<Event, Base> hand);

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
    WidgetEventDispatcher<cen::mouse_button_event, cen::mouse_motion_event> dispatcher;
    bool hover = false;
    bool active = false;
};

// Template generic implementation
template <typename Event, class Base>
void Widget::add_event_handler(widget_handler_func_t<Event, Base> hand) {
    auto handWrapper = convert_handler_func<Event, Base>(hand);
    dynamic_cast<Base *>(this)->dispatcher.template add_handler<convert_handler_t<Event, Base>>(handWrapper);
}

template<typename Event>
bool Widget::process_event(Event event)
{
    auto widget_event = WidgetEvent(event);
    dispatcher.run_handlers(widget_event);
    if (widget_event.getEventActions().isPropagationStopped()) {
        return true;
    }

    bool cancelled = false;
    for (const auto &[w, pos, alloc] : get_children()) {
        if (propagate_event(event, w, pos, alloc)) {
            cancelled = w->process_event(event) || cancelled;
        }
    }

    return cancelled;
}

#endif // WIDGET_H
