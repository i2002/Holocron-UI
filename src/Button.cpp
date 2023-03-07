#include "Button.h"
#include "Utilities.h"
#include <utility>

const std::string Button::font_name = "OpenSans-Regular.ttf";

Button::Button(std::string text) :
    Widget({0, 0}, SizingPolicy::FIT_PARENT),
    label{std::move(text)}
{
    size = compute_button_size();
}

Button& Button::operator=(Button other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Widget> Button::clone() const
{
    return std::make_shared<Button>(*this);
}

void swap(Button &first, Button &second)
{
    // enable ADL
    using std::swap;
    swap(static_cast<Widget&>(first), static_cast<Widget&>(second));
    swap(first.label, second.label);
}

std::string Button::display_name() const
{
    return "Button";
}

void Button::display_attributes(std::ostream &os) const
{
    os << "label: " << label << ", ";
    Widget::display_attributes(os);
}

void Button::render(cen::renderer &renderer, cen::ipoint offset) const
{
    Utilities::render_background(renderer, offset, size, get_background_color());

    auto font = cen::font{font_name, font_size};
    auto text_texture = renderer.make_texture(font.render_blended(label.c_str(), cen::colors::black));
    auto text_size = text_texture.size();
    cen::ipoint pos = {(size.width - text_size.width) / 2, (size.height - text_size.height) / 2};
    renderer.render(text_texture, offset + pos);
}

cen::iarea Button::compute_button_size() const
{
    auto font = cen::font{font_name, font_size};
    auto text_size = font.calc_size(label).value_or(cen::iarea{0, 0});
    return {text_size.width + 2 * padding, text_size.height + 2 * padding};
}

cen::color Button::get_background_color() const
{
    if (active) {
        return active_color;
    } else if (hover) {
        return hover_color;
    }
    return background_color;
}
