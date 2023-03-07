#ifndef BUTTON_H
#define BUTTON_H


#include <string>
#include "Widget.h"

class Button : public Widget
{
    static const std::string font_name;
    static const int font_size = 15;
    static const int padding = 3;
    constexpr static const cen::color background_color = {169, 194, 175};
    constexpr static const cen::color hover_color = {149, 171, 154};
    constexpr static const cen::color active_color = {126, 145, 131};
public:
    explicit Button(std::string text = "");
    Button(const Button &other) = default;
    Button(Button &&other) = default;

    Button& operator=(Button other);
    Button& operator=(Button &&other) = default;
    [[nodiscard]] std::shared_ptr<Widget> clone() const override;
    friend void swap(Button &first, Button &second);

    [[nodiscard]] std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;

protected:
    void render(cen::renderer &renderer, cen::ipoint offset) const override;

private:
    [[nodiscard]] cen::iarea compute_button_size() const;
    [[nodiscard]] cen::color get_background_color() const;
    std::string label;
};


#endif // BUTTON_H
