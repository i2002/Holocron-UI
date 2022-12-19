#ifndef WIDGET_H
#define WIDGET_H

#include <centurion.hpp>
#include <memory>
#include <vector>
#include <string>
#include <tuple>

class Widget
{
protected:
    enum class SizingPolicy {FIXED_SIZE, FIT_PARENT};

public:
    // TODO: switch to using smart pointers
    // FIXME: parent ref management
    explicit Widget(cen::iarea size = {0, 0}, SizingPolicy policy = SizingPolicy::FIXED_SIZE);
    virtual ~Widget() = default;
    Widget(Widget &&other) = default;

    Widget &operator=(Widget &&other) = default;
    [[nodiscard]] virtual std::shared_ptr<Widget> clone() const = 0;
    friend void swap(Widget &first, Widget &second);

    // virtual void display();
    [[nodiscard]] virtual std::string display_name() const = 0;
    virtual void display_attributes(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const Widget &w);
    cen::iarea get_size() const;
    void set_size(cen::iarea size);
    cen::iarea get_allocated_size() const;
    virtual void set_allocated_size(cen::iarea size);
    void add_child(const std::shared_ptr<Widget> &w, cen::ipoint pos);

protected:
    // prevent object slicing when copying
    Widget(const Widget &other);
    // QUESTION: is it ok to not have an assigment operator for abstract class?

    // TODO: surface rendering: every widget has it's own surface that is merged by the parent + surface caching
    void render(cen::renderer &renderer, cen::ipoint offset);
    virtual void render_self(cen::renderer &renderer, cen::ipoint offset) const  = 0;
    bool check_collisions(const std::shared_ptr<Widget> &w, cen::ipoint pos) const;
    // TODO: implement resizing request (when internal widget structure changed)

    // TODO: sizing based on widget size requirement? (-> scrolling)
    // TODO: margins in relation to parent (px or %)
    std::weak_ptr<Widget> parent;
    std::vector<std::pair<cen::ipoint, std::shared_ptr<Widget>>> children;
    cen::iarea size;
    cen::iarea allocated_size;
    SizingPolicy sizing_policy;
};

#endif // WIDGET_H
