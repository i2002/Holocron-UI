//
// Created by tudor on 12/18/22.
//

#include "GridContainer.h"

GridContainer::GridContainer(int cols, int rows) :
    Widget{{0, 0}, SizingPolicy::FIT_PARENT},
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

void GridContainer::set_allocated_size(cen::iarea size_)
{
    Widget::set_allocated_size(size_);

    for (size_t i = 0; i < children.size(); i++) {
        const auto& grid_pos= positioning_data[i];
        children[i].first = grid_pos.get_child_pos(*this);
        children[i].second->set_allocated_size(grid_pos.get_child_size(*this));
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

void GridContainer::add_child(const std::shared_ptr<Widget> &w, int col, int row, int span_cols, int span_rows)
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
