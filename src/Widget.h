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
    Widget *parent;
    std::vector<std::pair<cen::ipoint, Widget*>> children;
    cen::iarea size;

public:
    // TODO: switch to using smart pointers
    explicit Widget(cen::iarea size = {0, 0}, Widget *parent = nullptr);
    Widget(Widget &&other) = default;
    virtual ~Widget();

    Widget &operator=(Widget &&other) = default; // QUESTION: is there anything I can do besides copying the object pointer?; why is this needed?
    virtual Widget* clone() const = 0;
    friend std::ostream& operator<<(std::ostream& os, const Widget &w);
    cen::iarea get_size();
    virtual void add_child(Widget *w, cen::ipoint pos);

protected:
    // prevent object slicing when copying
    Widget(const Widget &other);
    Widget &operator=(const Widget &other) = default; // QUESTION: what to do here?

    // TODO: surface rendering: every widget has it's own surface that is merged by the parent + surface caching
    void render(cen::renderer &renderer, cen::ipoint offset);
    virtual void render_self(cen::renderer &renderer, cen::ipoint offset) const  = 0;
    void set_size(cen::iarea size);
    // TODO: implement resizing request (when internal widget structure changed)
};

#endif // WIDGET_H
