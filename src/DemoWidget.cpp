#include <iostream>
#include "DemoWidget.h"
#include "Utilities.h"

DemoWidget::DemoWidget(const cen::iarea &size, const cen::color &color) :
    Widget{size, SizingPolicy::FIT_PARENT},
    color{color}
{
    #ifdef HOLOCRONUI_DEBUG_MESSAGES
    add_event_handler<cen::mouse_button_event>([] (cen::mouse_button_event event, EventActions&) {
        if (event.clicks() == 2 && event.pressed()) {
            std::cout << "> DemoWidget double click\n";
        }
    });
    #endif // HOLOCRONUI_DEBUG_MESSAGES
}

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
    Utilities::render_background(renderer, offset, size, active ? cen::colors::dark_gray : color);
    
    if (hover) {
        cen::color overlay{0, 0, 0, 50};
        renderer.set_blend_mode(cen::blend_mode::blend);
        Utilities::render_background(renderer, offset, size, overlay);
        renderer.draw_rect(cen::irect{offset, allocated_size});
        renderer.set_blend_mode(cen::blend_mode::none);
    }
}
