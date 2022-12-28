#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <tuple>
#include <vector>
#include <functional>

template<typename ...Events>
class EventDispatcher
{
    static_assert((!std::is_const_v<Events> && ...));
    static_assert((!std::is_volatile_v<Events> && ...));
    static_assert((!std::is_reference_v<Events> && ...));
    static_assert((!std::is_pointer_v<Events> && ...));

    template <typename Event>
    using event_wrapper = std::vector<std::function<bool (Event)>>;

    template <typename Event, typename Data>
    using handler_type = std::function<bool (Event, Data)>;

public:
    /**
     * Disable template deduction for function params:
     * Source: https://stackoverflow.com/questions/37737487/better-way-to-disable-argument-based-template-parameter-deduction-for-functions
     * Further reading:
     * - https://artificial-mind.net/blog/2020/09/26/dont-deduce
     * - https://en.cppreference.com/w/cpp/language/template_argument_deduction#Non-deduced_contexts
     * - https://en.cppreference.com/w/cpp/types/type_identity
     */

    ~EventDispatcher()
    {
        remove_all_handlers();
    }

    template <typename Event, typename Data>
    void add_handler(handler_type<Event, std::common_type_t<Data>> handler, Data data)
    {
        using namespace std::placeholders;
        auto data_handler = std::bind(handler, _1, data);
        get_handlers<Event>().push_back(data_handler);
    }

protected:
    template <typename Event>
    bool run_handlers(Event ev)
    {
        bool cancelled = false;
        for (auto hand : get_handlers<Event>()) {
            cancelled = hand(ev) || cancelled;
        }

        return cancelled;
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
