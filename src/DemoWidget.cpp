#include "DemoWidget.h"

DemoWidget::DemoWidget(const cen::iarea size, const cen::color &color) :
    Widget{size},
    color{color}
{}

DemoWidget::~DemoWidget()
{

}

Widget* DemoWidget::clone() const 
{
    return new DemoWidget(*this);
}

std::string DemoWidget::display_name() const
{
    return "DemoWidget";
}

void DemoWidget::display_attributes(std::ostream& os) const
{
    Widget::display_attributes(os);
    os << ", "
       << "color: " << color;
}

void DemoWidget::render_self(cen::renderer &renderer, cen::ipoint offset) const
{
    cen::color prev_color = renderer.get_color();
    renderer.set_color(color);
    renderer.fill_rect(cen::irect{offset, size});
    renderer.set_color(prev_color);
}