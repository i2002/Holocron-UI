#include "Application.h"
#include "Widget.h"
#include "DemoWidget.h"
#include "GridContainer.h"
#include <iostream>

Application::Application() :
    main_window{"Title"}
{
    // FIXME: window building outside of app
    auto cont1 = std::make_shared<GridContainer>(2, 2);
    auto cont2 = std::make_shared<GridContainer>(1, 3);
    auto ch1 = std::make_shared<DemoWidget>(cen::iarea{10, 10}, cen::colors::aqua);
    auto ch2 = std::make_shared<DemoWidget>(cen::iarea{20, 20}, cen::colors::red);
    auto ch3 = std::make_shared<DemoWidget>(cen::iarea{10, 10}, cen::colors::yellow);
    cont2->add_child(ch1, 0, 0);
    cont2->add_child(ch2, 0, 1, 1, 2);
    cont1->add_child(ch3, 0, 0, 2);
    cont1->add_child(cont2, 1, 1);

    main_window.set_child(cont1);
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
    if (e.is<cen::quit_event>()) {
        running = false;
    }

    // TODO: event processing?
    if (e.is(cen::event_type::mouse_button_down)) {
        std::cout << "-------------\n";
        const auto& btn_down = e.get<cen::mouse_button_event>();
        main_window.process_event(e, {btn_down.x(), btn_down.y()});
    }
}

std::ostream& operator<<(std::ostream& os, const Application &a)
{
    os << "Application, with window: " << a.main_window;
    return os;
}
