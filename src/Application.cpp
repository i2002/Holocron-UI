#include "Application.h"
#include "Widget.h"
#include "DemoWidget.h"
#include "GridContainer.h"
#include "TextBox.h"
#include <iostream>

Application::Application() :
    main_window{"Title"}
{
    // FIXME: window building outside of app
    auto cont1 = std::make_shared<GridContainer>(2, 2);
    auto cont2 = std::make_shared<GridContainer>(1, 3);
    auto cont3 = std::make_shared<GridContainer>(1, 4);
    auto ch1 = std::make_shared<DemoWidget>(cen::iarea{10, 10}, cen::colors::aqua);
    auto ch2 = std::make_shared<DemoWidget>(cen::iarea{20, 20}, cen::colors::red);
    auto ch3 = std::make_shared<DemoWidget>(cen::iarea{10, 10}, cen::colors::yellow);
    auto ch_text = std::make_shared<TextBox>("Text 1", cen::iarea{100, 20});
    auto ch_text2 = std::make_shared<TextBox>("Text 2", cen::iarea{100, 20}, TextBox::VerticalAlignment::MIDDLE, TextBox::HorizontalAlignment::CENTER);
    auto ch_text3 = std::make_shared<TextBox>("Text 3", cen::iarea{100, 20}, TextBox::VerticalAlignment::BOTTOM, TextBox::HorizontalAlignment::RIGHT);

    cont2->add_child(ch1, 0, 0);
    cont2->add_child(ch2, 0, 1, 1, 2);
    cont3->add_child(ch_text, 0, 0);
    cont3->add_child(ch_text2, 0, 1);
    cont3->add_child(ch_text3, 0, 2);
    cont3->add_event_handler<cen::mouse_button_event>([] (cen::mouse_button_event) {
        std::cout << "> Stop event propagation\n";
        return true;
    });
    cont1->add_child(ch3, 0, 0, 2);
    cont1->add_child(cont2, 1, 1);
    cont1->add_child(cont3, 0, 1);

    main_window.set_child(cont1);
    main_window.add_event_handler<std::string, Window>([](const std::string &event) {
        std::cout << "> Received window custom event: " << event << "\n";
        return false;
    });

    ch1->add_event_handler<cen::mouse_button_event>([cont3] (cen::mouse_button_event event) {
        if (!event.pressed()) {
            return false;
        }

        std::cout << "> Insert new element\n";
        auto ch_text = std::make_shared<TextBox>("Text extra", cen::iarea{100, 20}, TextBox::VerticalAlignment::MIDDLE, TextBox::HorizontalAlignment::CENTER);
        cont3->add_child(ch_text, 0, 3);
        return false;
    });
}

void Application::run()
{
    main_window.show();
    running = true;
    while(running) {
        while (handler.poll()) {
            process_event(handler);
        }

        main_window.render_window();
    }
}

void Application::process_event(cen::event_handler &e)
{
    // application events
    if (e.is<cen::quit_event>()) {
        running = false;
    }

    // window events
    if(e.is(cen::event_type::window)) {
        const auto& window_event = e.get<cen::window_event>();
        if (window_event.event_id() == cen::window_event_id::resized) {
            main_window.set_allocated_size({window_event.data1(), window_event.data2()});
        }

        switch(window_event.event_id()) {
            case cen::window_event_id::resized:
            case cen::window_event_id::size_changed:
                main_window.set_allocated_size({window_event.data1(), window_event.data2()});
                break;

            case cen::window_event_id::leave:
                main_window.set_hover(false);
                break;

            default:
                break;
        }
    }

    if (e.is(cen::event_type::mouse_button_down) || e.is(cen::event_type::mouse_button_up)) {
        std::cout << "------- Window click event received -------\n";
        main_window.process_event(e.get<cen::mouse_button_event>());
        std::cout << "\n";
    } else if (e.is(cen::event_type::mouse_motion)) {
        main_window.process_event(e.get<cen::mouse_motion_event>());
    }
}

std::ostream& operator<<(std::ostream& os, const Application &a)
{
    os << "Application, with window: " << a.main_window;
    return os;
}
