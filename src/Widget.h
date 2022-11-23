#ifndef WIDGET_H
#define WIDGET_H

#include <centurion.hpp>
#include <vector>

class Widget
{
public:

protected:
    // FIXME: hide data from derivates and force them to use getters and setters?
    cen::iarea requested_size;
    cen::iarea allocated_size;
    cen::ipoint allocated_position;
    Widget *parent;
    std::vector<Widget*> children;

public:
    explicit Widget(Widget *parent = nullptr);
    virtual ~Widget();
    // TODO: copy constructor
    // TODO: move constructor / virtual clone
    friend std::ostream& operator<<(std::ostream& os, const Widget &w);
    cen::iarea get_requested_size();
    cen::iarea get_allocated_size();
    cen::ipoint get_allocated_position();

protected:
    virtual void render(cen::renderer &renderer, int x, int y) = 0;
    void set_allocation(cen::iarea allocated_size, cen::ipoint allocated_position);
    void request_resize();
    // TODO: implement child management + FIXME: how to handle widget references
};

#endif // WIDGET_H