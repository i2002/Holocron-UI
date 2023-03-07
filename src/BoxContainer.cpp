#include "BoxContainer.h"

BoxContainer::BoxContainer(BoxDirection dir, const cen::color &background_color) :
    Container({0, 0}, background_color, SizingPolicy::FIT_PARENT),
    direction{dir}
{}

BoxContainer& BoxContainer::operator=(BoxContainer other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Widget> BoxContainer::clone() const
{
    return std::make_shared<BoxContainer>(*this);
}

void swap(BoxContainer &first, BoxContainer &second)
{
    // enable ADL
    using std::swap;
    swap(static_cast<Container&>(first), static_cast<Container&>(second));
    swap(first.direction, second.direction);
    swap(first.expand, second.expand);
    swap(first.nr_expanded, second.nr_expanded);
    swap(first.total_fixed_size, second.total_fixed_size);
}

std::string BoxContainer::display_name() const
{
    return "BoxContainer";
}

void BoxContainer::display_attributes(std::ostream &os) const
{
    os << "direction: " << (direction == BoxDirection::VERTICAL ? "vertical" : "horizontal") << ", ";
    Container::display_attributes(os);
}

void BoxContainer::append_child(const std::shared_ptr<Widget> &w, bool _expand)
{
    expand.emplace_back(_expand);
    nr_expanded += _expand;
    if (!_expand) {
        total_fixed_size += get_widget_dimension(*w);
    }
    Container::add_child(w);
}

cen::ipoint BoxContainer::get_child_position(size_t index) const
{
    int position = 0;
    int expanded_size = get_expanded_size();
    for (size_t i = 0; i < index; i++) {
        auto [child, pos, alloc] = children[i];
        position += expand[i] ? expanded_size : get_widget_dimension(*child);
    }

    return create_position(position);
}

cen::iarea BoxContainer::get_child_allocation(size_t index, std::shared_ptr<Widget> child) const
{
    int child_size = expand[index] ? get_expanded_size() : get_widget_dimension(*child);
    return create_allocation(child_size);
}

int BoxContainer::get_expanded_size() const
{
    return nr_expanded ? (get_widget_dimension(*this) - total_fixed_size) / nr_expanded : 0;
}

int BoxContainer::get_widget_dimension(const Widget &child) const
{
    switch (direction) {
        case BoxDirection::VERTICAL:
            return child.get_size().height;
        case BoxDirection::HORIZONTAL:
            return child.get_size().width;
    }

    return 0;
}

cen::iarea BoxContainer::create_allocation(int child_size) const
{
    switch (direction) {
        case BoxDirection::VERTICAL:
            return {size.width, child_size};

        case BoxDirection::HORIZONTAL:
            return {child_size, size.height};
    }

    return {0, 0};
}

cen::ipoint BoxContainer::create_position(int child_position) const
{
    switch (direction) {
        case BoxDirection::VERTICAL:
            return {0, child_position};
        case BoxDirection::HORIZONTAL:
            return {child_position, 0};
    }

    return {0, 0};
}
