#ifndef WIDGET_H
#define WIDGET_H

#include <centurion.hpp>
#include <vector>
#include <string>
#include <tuple>

class Widget
{
public:

protected:
    // TODO: sizing based on widget size requirement? (-> scrolling)
    // TODO: margins in relation to parent (px or %)
    Widget *parent;
    std::vector<std::pair<cen::ipoint, Widget*>> children;
    cen::iarea size;

public:
    // TODO: switch to using smart pointers
    explicit Widget(cen::iarea size = {0, 0}, Widget *parent = nullptr);
    virtual ~Widget();
    Widget(Widget &&other) = default;
    Widget &operator=(Widget &&other) = default;
    [[nodiscard]] virtual Widget* clone() const = 0;
    friend void swap(Widget &first, Widget &second);

    // virtual void display();
    [[nodiscard]] virtual std::string display_name() const = 0;
    virtual void display_attributes(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const Widget &w);
    cen::iarea get_size();
    void add_child(Widget *w, cen::ipoint pos);

protected:
    // prevent object slicing when copying
    Widget(const Widget &other);
    // QUESTION: is it ok to not have an assigment operator for abstract class?

    // TODO: surface rendering: every widget has it's own surface that is merged by the parent + surface caching
    void render(cen::renderer &renderer, cen::ipoint offset);
    virtual void render_self(cen::renderer &renderer, cen::ipoint offset) const  = 0;
    virtual bool check_collisions(Widget *w, cen::ipoint pos);
    void set_size(cen::iarea size);
    // TODO: implement resizing request (when internal widget structure changed)
};

#endif // WIDGET_H
