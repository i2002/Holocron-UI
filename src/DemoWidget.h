#ifndef DEMO_WIDGET_H
#define DEMO_WIDGET_H

#include <string>
#include "Widget.h"

class DemoWidget : public Widget
{
    cen::color color;

public:
    explicit DemoWidget(const cen::iarea size, const cen::color &color = cen::colors::blue);
    DemoWidget(const DemoWidget &other) = default;
    ~DemoWidget();
    Widget *clone() const override;
    std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;

protected:
    void render_self(cen::renderer &renderer, cen::ipoint offset) const override;
};

#endif // DEMO_WIDGET_H
