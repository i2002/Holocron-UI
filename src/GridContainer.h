#ifndef GRIDCONTAINER_H
#define GRIDCONTAINER_H

#include "Widget.h"
#include <vector>
#include <tuple>

class GridContainer : public Widget
{
protected:
    struct Position {
        int col, row, span_cols, span_rows;

        cen::iarea get_child_size(const GridContainer &container) const {
            int row_size = container.size.height / container.rows;
            int col_size = container.size.width / container.cols;
            return {span_cols * col_size, span_rows * row_size};
        }

        cen::ipoint get_child_pos(const GridContainer &container) const {
            int row_size = container.size.height / container.rows;
            int col_size = container.size.width / container.cols;
            return {col * col_size, row * row_size};
        }
    };

public:
    GridContainer(int cols, int rows);
    GridContainer(GridContainer &&other) = default;
    GridContainer &operator=(GridContainer &&other) = default;
    [[nodiscard]] Widget *clone() const override;
    friend void swap(GridContainer &first, GridContainer &second);

    [[nodiscard]] std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;

    void set_allocated_size(cen::iarea size) override;
    void add_child(Widget *w, int col, int row, int span_cols = 1, int span_rows = 1);

protected:
    GridContainer(const GridContainer &other) = default;
    GridContainer& operator=(GridContainer other);
    void render_self(cen::renderer &renderer, cen::ipoint offset) const override;
    bool check_collisions(Position pos) const;

private:
    int cols;
    int rows;
    std::vector<Position> positioning_data;
};


#endif // GRIDCONTAINER_H
