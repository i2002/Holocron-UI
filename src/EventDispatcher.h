#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <tuple>
#include <vector>
#include <functional>

template <typename Event>
using handler_func_t = std::function<void(Event&)>;

template<typename ...Events>
class EventDispatcher
{
    static_assert((!std::is_const_v<Events> && ...));
    static_assert((!std::is_volatile_v<Events> && ...));
    static_assert((!std::is_reference_v<Events> && ...));
    static_assert((!std::is_pointer_v<Events> && ...));

public:
    template <typename Event>
    using event_wrapper = std::vector<handler_func_t<Event>>;

    ~EventDispatcher()
    {
        remove_all_handlers();
    }

    template <typename Event>
    void add_handler(handler_func_t<Event> handler)
    {
        get_handlers<Event>().push_back(handler);
    }

    template <typename Event>
    void run_handlers(Event &event)
    {
        for (auto hand : get_handlers<Event>()) {
            hand(event);
        }
    }

    template <typename Event>
    void run_handlers(const Event &ev)
    {
        Event ev_copy = ev;
        for (auto hand : get_handlers<Event>()) {
            hand(ev_copy);
        }
    }

    template <typename Event>
    void remove_handlers()
    {
        get_handlers<Event>().clear();
    }

    template<size_t I = 0>
    void remove_all_handlers()
    {
        if constexpr (I == sizeof...(Events)) {
            return;
        } else {
            get<I>(handlers).clear();
            remove_all_handlers<I + 1>();
        }
    }

private:
    std::tuple<event_wrapper<Events>...> handlers;

    template <typename Event>
    [[nodiscard]] constexpr auto get_handlers() -> event_wrapper<Event> &
    {
        return std::get<event_wrapper<Event>>(handlers);
    }
};

#endif // EVENT_DISPATCHER_H
