//
// Created by tudor on 12/18/22.
//

#include "GridContainer.h"

GridContainer::GridContainer(int cols, int rows) :
    Widget{{0, 0}, nullptr, SizingPolicy::FIT_PARENT},
    cols{cols}, rows{rows}
{}

GridContainer& GridContainer::operator=(GridContainer other)
{
    swap(*this, other);
    return *this;
}

Widget* GridContainer::clone() const
{
    return new GridContainer(*this);
}

// FIXME: is it necessary if I only swap all attributes (isn't that done by default?)
void swap(GridContainer &first, GridContainer &second)
{
    // enable ADL
    using std::swap;
    swap(static_cast<Widget&>(first), static_cast<Widget&>(second));
    swap(first.cols, second.cols);
    swap(first.rows, second.rows);
}

std::string GridContainer::display_name() const
{
    return "GridContainer";
}

void GridContainer::display_attributes(std::ostream& os) const
{
    Widget::display_attributes(os);
    os << ", cols: " << cols
       << ", rows: " << rows;
}

void GridContainer::set_allocated_size(cen::iarea size)
{
    Widget::set_allocated_size(size);
    int row_size = size.height / rows;
    int col_size = size.width / cols;

    for (size_t i = 0; i < children.size(); i++) {
        auto& [pos, w] = children[i];
        const auto& [col, row, span_cols, span_rows] = positioning_data[i];
        pos = {col * col_size, row * row_size};
        w->set_allocated_size(cen::iarea {span_cols * col_size, span_rows * row_size});
    }
}

void GridContainer::render_self(cen::renderer &, cen::ipoint ) const
{
    // FIXME: widget background color
/*    cen::color prev_color = renderer.get_color();
    renderer.set_color(cen::colors::dark_gray);
    renderer.fill_rect(cen::irect{offset, size});
    renderer.set_color(prev_color);*/
}

void GridContainer::add_child(Widget *w, int col, int row, int span_cols, int span_rows)
{
    Position p{col, row, span_cols, span_rows}; // TODO: throw if invalid data
    if (check_collisions(p)) {
        return; // TODO: throw if overlapping
    }

    w->set_allocated_size(p.get_child_size(*this));
    positioning_data.emplace_back(p);
    children.emplace_back(p.get_child_pos(*this), w->clone());
}

bool GridContainer::check_collisions(Position pos) const
{
    return std::any_of(positioning_data.cbegin(), positioning_data.cend(), [pos](Position other) {
        bool outside_col = pos.col + pos.span_cols <= other.col || pos.col >= other.col + other.span_cols;
        bool outside_row = pos.row + pos.span_rows <= other.row || pos.row >= other.row + other.span_rows;
        return !(outside_col || outside_row);
    });
}
