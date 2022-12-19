#include "DemoWidget.h"

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

// FIXME: is it useful given that assignment operator is protected?
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
    Widget::display_attributes(os);
    os << ", color: " << color;
}

void DemoWidget::render_self(cen::renderer &renderer, cen::ipoint offset) const
{
    cen::color prev_color = renderer.get_color();
    renderer.set_color(color);
    renderer.fill_rect(cen::irect{offset, size});
    renderer.set_color(prev_color);
}
