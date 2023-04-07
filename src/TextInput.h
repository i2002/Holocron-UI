#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include "Widget.h"
#include <sstream>
#include <iterator>

class TextInput : public Widget
{
    static const std::string font_name;
    static const int font_size = 12;
    static const int padding = 3;
    constexpr static const cen::color border_color = cen::colors::black;
    constexpr static const cen::color border_focused_color = cen::colors::blue;

    class TextInputModel
    {
        std::vector<std::string> input;
        int cursor_pos = 0;
    public:
        [[nodiscard]] std::string get_input(bool sliced = false) const {
            std::stringstream joined;
            auto end = sliced ? input.begin() + cursor_pos : input.end();
            copy(input.begin(), end, std::ostream_iterator<std::string>(joined, ""));
            return joined.str();
        }

        /*[[nodiscard]] size_t get_cursor_pos() const {
            return cursor_pos;
        }*/

        void cursor_left() {
            if (cursor_pos > 0) {
                cursor_pos--;
            }
        }

        void cursor_right() {
            if((size_t)cursor_pos < input.size()) {
                cursor_pos++;
            }
        }

        void backspace() {
            if (cursor_pos == 0) {
                return;
            }
            input.erase(input.begin() + cursor_pos - 1);
            cursor_pos--;
        }

        void insert(std::string_view text) {
            input.insert(input.begin() + cursor_pos, text.data());
            cursor_pos++;
        }
    };

public:
    TextInput();
    TextInput(const TextInput &other);
    TextInput(TextInput &&other) = default;
    ~TextInput();

    TextInput& operator=(TextInput other);
    TextInput& operator=(TextInput &&other) = default;
    [[nodiscard]] std::shared_ptr<Widget> clone() const override;
    friend void swap(TextInput &first, TextInput &second);

    [[nodiscard]] std::string display_name() const override;
    void display_attributes(std::ostream& os) const override;
    bool focusable() override;
    void set_focused(bool state) override;

protected:
    void register_handlers();
    void render(cen::renderer &renderer, cen::ipoint offset) const override;

    void set_blink_timer();
    void stop_blink_timer();
    static int blink_callback(Uint32 interval, void* name);
    static bool blink_on;

private:
    TextInputModel input;
    SDL_TimerID blink_timer = 0;
};


#endif // TEXT_INPUT_H
