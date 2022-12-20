//
// Created by tudor on 12/18/22.
//

#include "GridContainer.h"

GridContainer::GridContainer(int cols, int rows) :
    Container{{0, 0}, SizingPolicy::FIT_PARENT},
    cols{cols}, rows{rows}
{}

GridContainer& GridContainer::operator=(GridContainer other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Widget> GridContainer::clone() const
{
    return std::make_shared<GridContainer>(*this);
}

// FIXME: is it necessary if I only swap all attributes (isn't that done by default?)
void swap(GridContainer &first, GridContainer &second)
{
    // enable ADL
    using std::swap;
    swap(static_cast<Container&>(first), static_cast<Container&>(second));
    swap(first.cols, second.cols);
    swap(first.rows, second.rows);
}

std::string GridContainer::display_name() const
{
    return "GridContainer";
}

void GridContainer::display_attributes(std::ostream& os) const
{
    os << "cols: " << cols << ", "
       << "rows: " << rows << ", ";
    Container::display_attributes(os);
}

void GridContainer::add_child(const std::shared_ptr<Widget> &w, int col, int row, int span_cols, int span_rows)
{
    Position p{col, row, span_cols, span_rows}; // TODO: throw if invalid data
    if (check_collisions(p)) {
        return; // TODO: throw if overlapping
    }

    positioning_data.emplace_back(p);
    Container::add_child(w);
}

cen::ipoint GridContainer::get_child_position(size_t index) const
{
    Position p = positioning_data[index];
    int row_size = size.height / rows;
    int col_size = size.width / cols;
    return {p.col * col_size, p.row * row_size};
}

cen::iarea GridContainer::get_child_allocation(size_t index) const
{
    Position p = positioning_data[index];
    int row_size = size.height / rows;
    int col_size = size.width / cols;
    return {p.span_cols * col_size, p.span_rows * row_size};
}

bool GridContainer::check_collisions(Position pos) const
{
    return std::any_of(positioning_data.cbegin(), positioning_data.cend(), [pos](Position other) {
        bool outside_col = pos.col + pos.span_cols <= other.col || pos.col >= other.col + other.span_cols;
        bool outside_row = pos.row + pos.span_rows <= other.row || pos.row >= other.row + other.span_rows;
        return !(outside_col || outside_row);
    });
}
