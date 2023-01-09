#ifndef CONTAINER_H
#define CONTAINER_H

#include "Widget.h"
#include <memory>
#include <vector>

class Container : public Widget
{
public:
    explicit Container(cen::iarea size = {0, 0}, cen::color background_color = cen::colors::white, SizingPolicy policy = SizingPolicy::FIT_PARENT);
    Container(Container &&other) = default;

    Container &operator=(Container &&other) = default;
    friend void swap(Container &first, Container &second);

    void display_attributes(std::ostream& os) const override;
    void display(std::ostream &os, int nest_level) const override;
    void set_allocated_size(cen::iarea size) override;
    void set_hover(bool state, cen::ipoint pos = {0, 0}) override;

    [[nodiscard]] children_vector get_children() const override;

protected:
    Container(const Container &other);
    void render(cen::renderer &renderer, cen::ipoint offset) const final;

    void add_child(const std::shared_ptr<Widget> &w);
    [[nodiscard]] virtual cen::ipoint get_child_position(size_t index) const = 0;
    [[nodiscard]] virtual cen::iarea get_child_allocation(size_t index) const = 0;

    children_vector children;
    cen::color background_color;
};

#endif //CONTAINER_H
