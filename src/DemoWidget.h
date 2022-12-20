#ifndef DEMO_WIDGET_H
#define DEMO_WIDGET_H

#include <memory>
#include <string>
#include "Widget.h"

class DemoWidget : public Widget
{
public:
    explicit DemoWidget(const cen::iarea &size, const cen::color &color = cen::colors::blue);
    DemoWidget(const DemoWidget &other) = default;
    DemoWidget(DemoWidget &&other) = default;

    DemoWidget& operator=(DemoWidget other);
    DemoWidget &operator=(DemoWidget &&other) = default;
    [[nodiscard]] std::shared_ptr<Widget> clone() const override;
    friend void swap(DemoWidget &first, DemoWidget &second);

    [[nodiscard]] std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;

protected:
    void render(cen::renderer &renderer, cen::ipoint offset) const override;

private:
    cen::color color;
};

#endif // DEMO_WIDGET_H
