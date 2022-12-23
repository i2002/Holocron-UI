#include "DemoWidget.h"
#include "Utilities.h"

DemoWidget::DemoWidget(const cen::iarea &size, const cen::color &color) :
    Widget{size, SizingPolicy::FIT_PARENT},
    color{color}
{}

DemoWidget& DemoWidget::operator=(DemoWidget other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Widget> DemoWidget::clone() const
{
    return std::make_shared<DemoWidget>(*this);
}

void swap(DemoWidget &first, DemoWidget &second)
{
    // enable ADL
    using std::swap;
    swap(static_cast<Widget&>(first), static_cast<Widget&>(second));
    swap(first.color, second.color);
}

std::string DemoWidget::display_name() const
{
    return "DemoWidget";
}

void DemoWidget::display_attributes(std::ostream& os) const
{
    os << "color: " << color << ", ";
    Widget::display_attributes(os);
}

void DemoWidget::render(cen::renderer &renderer, cen::ipoint offset) const
{
    Utilities::render_background(renderer, offset, size, color);
}
