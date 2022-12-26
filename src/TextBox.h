//
// Created by tudor on 12/26/2022.
//

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Widget.h"
#include <memory>
#include <string>

class TextBox : public Widget
{
    static const std::string font_name;

public:
    enum class VerticalAlignment {TOP, MIDDLE, BOTTOM};
    enum class HorizontalAlignment {LEFT, CENTER, RIGHT};
    explicit TextBox(const std::string &text = "", const cen::iarea &box_size = {0, 0}, VerticalAlignment vertical_alignment = VerticalAlignment::TOP,
                     HorizontalAlignment horizontal_alignment = HorizontalAlignment::LEFT, const cen::color &text_color = cen::colors::black);
    TextBox(const TextBox &other) = default;
    TextBox(TextBox &&other) = default;

    TextBox& operator=(TextBox other);
    TextBox& operator=(TextBox &&other) = default;
    [[nodiscard]] std::shared_ptr<Widget> clone() const override;
    friend void swap(TextBox &first, TextBox &second);

    [[nodiscard]] std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;

protected:
    void render(cen::renderer &renderer, cen::ipoint offset) const override;
    cen::ipoint text_positioning(const cen::iarea &text_size) const;

private:
    std::string text;
    cen::color text_color;
    VerticalAlignment valign;
    HorizontalAlignment halign;
};

#endif // TEXTBOX_H
