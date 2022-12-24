#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class holocronui_error : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class invalid_properties : public holocronui_error
{
    using holocronui_error::holocronui_error;
};

class container_error : public holocronui_error
{
    using holocronui_error::holocronui_error;
};

class invalid_grid_position : public container_error
{
    using container_error::container_error;
};

class grid_widget_overlap : public container_error
{
    using container_error::container_error;
};

class builder_parse_error : public holocronui_error
{
    using holocronui_error::holocronui_error;
};

#endif // EXCEPTIONS_H
