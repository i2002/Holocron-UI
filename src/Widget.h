#ifndef WIDGET_H
#define WIDGET_H

#include <centurion.hpp>
#include <vector>
#include <tuple>

class Widget
{
public:

protected:
    // FIXME: hide data from derivates and force them to use getters and setters?
    // TODO: sizing based on widget size requirement? (-> scrolling)
    // TODO: margins in relation to parent (px or %)
    cen::iarea allocated_size;
    Widget *parent;
    std::vector<std::pair<cen::ipoint, Widget*>> children;

public:
    explicit Widget(Widget *parent = nullptr);
    virtual ~Widget();
    // TODO: copy constructor
    // TODO: move constructor / virtual clone
    friend std::ostream& operator<<(std::ostream& os, const Widget &w);
    cen::iarea get_allocated_size();
    virtual void add_child(Widget *w, cen::ipoint pos);

protected:
    // TODO: surface rendering: every widget has it's own surface that is merged by the parent + surface caching
    void render(cen::renderer &renderer, cen::ipoint offset);
    virtual void render_self(cen::renderer &renderer, cen::ipoint offset) = 0;
    void set_allocation(cen::iarea allocated_size);
    // TODO: implement resizing request (when internal widget structure changed)
};

#endif // WIDGET_H
