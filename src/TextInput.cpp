#include <iostream>
#include "TextInput.h"

bool TextInput::blink_on = false;
const std::string TextInput::font_name = "OpenSans-Regular.ttf";

TextInput::TextInput() :
    Widget({0, font_size + 2 * (padding + 1)}, SizingPolicy::FIT_PARENT)
{
    register_handlers();
}

TextInput::TextInput(const TextInput &other)
    : Widget{other}, input{other.input}
{
    register_handlers();
}

TextInput::~TextInput() {
    stop_blink_timer();
}

void TextInput::register_handlers() {
    add_event_handler<cen::keyboard_event>([this] (cen::keyboard_event event, EventActions&) {
        // control state events
        if(event.key() == cen::keycodes::left) {
            this->input.cursor_left();
        }

        if(event.key() == cen::keycodes::right) {
            this->input.cursor_right();
        }

        if(event.key() == cen::keycodes::backspace) {
            this->input.backspace();
        }

        // reset blink
        if (focused) {
            stop_blink_timer();
            set_blink_timer();
        }
    });

    // text input event
    add_event_handler<cen::text_input_event>([this] (cen::text_input_event event, EventActions&) {
        this->input.insert(event.text_utf8());
    });
}

void TextInput::set_blink_timer() {
    blink_on = true;
    blink_timer = SDL_AddTimer(1000, reinterpret_cast<SDL_TimerCallback>(&blink_callback), nullptr);
}

void TextInput::stop_blink_timer() {
    if (blink_timer != 0) {
        SDL_RemoveTimer(blink_timer);
    }
    blink_timer = 0;
}

int TextInput::blink_callback(Uint32, void *) {
    blink_on = !blink_on;
    return 1000;
}

TextInput& TextInput::operator=(TextInput other)
{
    swap(*this, other);
    return *this;
}

std::shared_ptr<Widget> TextInput::clone() const
{
    return std::make_shared<TextInput>(*this);
}

void swap(TextInput &first, TextInput &second)
{
    // enable ADL
    using std::swap;
    swap(static_cast<Widget&>(first), static_cast<Widget&>(second));
    swap(first.input, second.input);
}

std::string TextInput::display_name() const
{
    return "TextInput";
}

void TextInput::display_attributes(std::ostream &os) const
{
    os << "input: " << input.get_input() << ", ";
    Widget::display_attributes(os);
}

bool TextInput::focusable() {
    return true;
}

void TextInput::set_focused(bool state) {
    Widget::set_focused(state);

    if (focused) {
        set_blink_timer();
    } else {
        stop_blink_timer();
    }
}

void TextInput::render(cen::renderer &renderer, cen::ipoint offset) const
{
    // draw input border
    cen::color initial_color = renderer.get_color();
    cen::color ren_border_color = focused ? border_focused_color : border_color;
    renderer.set_color(ren_border_color);
    renderer.draw_rect(cen::irect{offset, allocated_size});
    renderer.set_color(initial_color);

    int cursor_pos = padding + 1;
    if (!input.get_input().empty()) {
        // input texture
        auto font = cen::font{font_name, font_size};
        auto text_texture = renderer.make_texture(font.render_blended_utf8(input.get_input().c_str(), cen::colors::black));

        // render input
        cen::ipoint pos = {padding + 1, padding};
        renderer.render(text_texture, offset + pos);

        // update cursor position
        int text_width = 0;
        TTF_SizeUTF8(font.get(), input.get_input(true).c_str(), &text_width, nullptr);
        cursor_pos += text_width;
    }

    if (focused && blink_on) {
        renderer.draw_line(offset + cen::ipoint{cursor_pos, padding}, offset + cen::ipoint{cursor_pos, size.height - padding - 1});
    }
}
