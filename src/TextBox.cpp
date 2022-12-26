#include "TextBox.h"

const std::string TextBox::font_name = "OpenSans-Regular.ttf";

TextBox::TextBox(const std::string &text, const cen::iarea &box_size, VerticalAlignment vertical_alignment,
                 HorizontalAlignment horizontal_alignment, const cen::color &text_color) :
                    Widget(box_size, SizingPolicy::FIT_PARENT),
                    text{text}, text_color{text_color}, valign{vertical_alignment}, halign{horizontal_alignment}
{}

TextBox& TextBox::operator=(TextBox other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Widget> TextBox::clone() const
{
    return std::make_shared<TextBox>(*this);
}

void swap(TextBox &first, TextBox &second)
{
    // enable ADL
    using std::swap;
    swap(static_cast<Widget&>(first), static_cast<Widget&>(second));
    swap(first.text, second.text);
    swap(first.text_color, second.text_color);
    swap(first.valign, second.valign);
    swap(first.halign, second.halign);
}

std::string TextBox::display_name() const
{
    return "TextBox";
}

void TextBox::display_attributes(std::ostream &os) const
{
    os << "text: " << text << ", "
        << "text color: " << text_color << ", ";
    Widget::display_attributes(os);
}

void TextBox::render(cen::renderer &renderer, cen::ipoint offset) const
{
    auto font = cen::font{font_name, 12};
    auto text_texture = renderer.make_texture(font.render_blended(text.c_str(), cen::colors::black));
    auto text_size = text_texture.size();
    renderer.render(text_texture, offset + text_positioning(text_size));
}

cen::ipoint TextBox::text_positioning(const cen::iarea &text_size) const
{
    cen::ipoint pos = {0, 0};
    switch (valign) {
        case VerticalAlignment::TOP:
            pos.set_y(0);
            break;
        case VerticalAlignment::MIDDLE:
            pos.set_y((size.height - text_size.height) / 2);
            break;
        case VerticalAlignment::BOTTOM:
            pos.set_y(size.height - text_size.height);
            break;
    }

    switch (halign) {
        case HorizontalAlignment::LEFT:
            pos.set_x(0);
            break;
        case HorizontalAlignment::CENTER:
            pos.set_x((size.width - text_size.width) / 2);
            break;
        case HorizontalAlignment::RIGHT:
            pos.set_x(size.width - text_size.width);
            break;
    }

    return pos;
}