#ifndef WIDGET_H
#define WIDGET_H

#include <centurion.hpp>
#include <memory>
#include <string>

class Widget
{
    friend class Container;

protected:
    using children_vector = std::vector<std::tuple<std::shared_ptr<Widget>, cen::ipoint, cen::iarea>>;
    enum class SizingPolicy {FIXED_SIZE, FIT_PARENT};

public:
    // FIXME: parent ref management
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
    [[nodiscard]] cen::iarea get_allocated_size() const;
    virtual void set_allocated_size(cen::iarea size);

protected:
    // prevent object slicing when copying
    Widget(const Widget &other) = default;
    // QUESTION: is it ok to not have an assigment operator for abstract class? OR clone() and swap on the clone

    // TODO: surface rendering: every widget has it's own surface that is merged by the parent + surface caching
    virtual void render(cen::renderer &renderer, cen::ipoint offset) const = 0;
    // TODO: implement resizing request (when internal widget structure changed)

    // TODO: support for widget specific events (like window resize, etc)
    // TODO: maybe use template function to prevent if/else if in every recursion call
    // TODO: and also remove position and in turn change the event to use relative position
    [[nodiscard]] virtual children_vector get_children() const;


    template<typename Event>
    bool process_event(Event);

    template <typename Event>
    void default_event_handler(Event /* event */) {};

    template <typename Event>
    bool propagate_event(Event &event, const std::shared_ptr<Widget> &w, cen::ipoint pos, cen::iarea alloc);

    // TODO: sizing based on widget size requirement? (-> scrolling)
    // TODO: margins in relation to parent (px or %)
    cen::iarea size;
    cen::iarea allocated_size;
    SizingPolicy sizing_policy;
    Widget *parent = nullptr;
};

#endif // WIDGET_H
