#ifndef CONTAINER_H
#define CONTAINER_H

#include "Widget.h"
#include <memory>
#include <vector>

class Container : public Widget
{
public:
    explicit Container(cen::iarea size = {0, 0}, SizingPolicy policy = SizingPolicy::FIT_PARENT);
    Container(Container &&other) = default;

    Container &operator=(Container &&other) = default;
    friend void swap(Container &first, Container &second);

    void display_attributes(std::ostream& os) const override;
    void display(std::ostream &os, int nest_level) const override;
    void set_allocated_size(cen::iarea size) override;

protected:
    Container(const Container &other);
    void render(cen::renderer &renderer, cen::ipoint offset) const final;
    virtual void render_self(cen::renderer &, cen::ipoint) const {};

    void add_child(const std::shared_ptr<Widget> &w);
    [[nodiscard]] virtual cen::ipoint get_child_position(size_t index) const = 0;
    [[nodiscard]] virtual cen::iarea get_child_allocation(size_t index) const = 0;

    std::vector<std::shared_ptr<Widget>> children;
};

#endif //CONTAINER_H
