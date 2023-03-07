#ifndef GRID_CONTAINER_H
#define GRID_CONTAINER_H

#include "Container.h"
#include <memory>
#include <vector>

class GridContainer : public Container
{
    static const int MAX_COLS = 60;
    static const int MAX_ROWS = 30;

protected:
    struct Position {
        int col, row, span_cols, span_rows;
    };

public:
    GridContainer(int cols, int rows, const cen::color &background_color = cen::colors::white);
    GridContainer(const GridContainer &other) = default;
    GridContainer(GridContainer &&other) = default;

    GridContainer& operator=(GridContainer other);
    GridContainer &operator=(GridContainer &&other) = default;
    [[nodiscard]] std::shared_ptr<Widget> clone() const override;
    friend void swap(GridContainer &first, GridContainer &second);

    [[nodiscard]] std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;

    void add_child(const std::shared_ptr<Widget> &w, int col, int row, int span_cols = 1, int span_rows = 1);

protected:
    [[nodiscard]] cen::ipoint get_child_position(size_t index) const override;
    [[nodiscard]] cen::iarea get_child_allocation(size_t index, std::shared_ptr<Widget> child) const override;
    [[nodiscard]] bool check_collisions(Position pos) const;

private:
    int cols;
    int rows;
    std::vector<Position> positioning_data;
};

#endif // GRID_CONTAINER_H
