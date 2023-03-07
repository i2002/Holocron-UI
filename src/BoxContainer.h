#ifndef BOX_CONTAINER_H
#define BOX_CONTAINER_H

#include "Container.h"

enum class BoxDirection {HORIZONTAL, VERTICAL};

class BoxContainer : public Container
{
public:
    BoxContainer(BoxDirection dir = BoxDirection::VERTICAL, const cen::color &background_color = cen::colors::white);
    BoxContainer(const BoxContainer &other) = default;
    BoxContainer(BoxContainer &&other) = default;

    BoxContainer& operator=(BoxContainer other);
    BoxContainer& operator=(BoxContainer &&other) = default;
    [[nodiscard]] std::shared_ptr<Widget> clone() const override;
    friend void swap(BoxContainer &first, BoxContainer &second);

    [[nodiscard]] std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;

    void append_child(const std::shared_ptr<Widget> &w, bool expand = false);

protected:
    [[nodiscard]] cen::ipoint get_child_position(size_t index) const override;
    [[nodiscard]] cen::iarea get_child_allocation(size_t index, std::shared_ptr<Widget> child) const override;
    [[nodiscard]] int get_expanded_size() const;

private:
    int get_widget_dimension(const Widget &child) const;
    cen::iarea create_allocation(int child_size) const;
    cen::ipoint create_position(int child_position) const;

    std::vector<bool> expand;
    int nr_expanded = 0;
    int total_fixed_size = 0;
    BoxDirection direction;
};


#endif // BOX_CONTAINER_H
