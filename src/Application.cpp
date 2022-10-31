#include "Application.h"
#include "Widget.h"

Application::Application() :
    main_window{"Titlu aplicatie", cen::colors::aqua}
{
    main_window.add_widget(Widget{100}, 10, 10);
    main_window.add_widget(Widget{100}, 10, 60);
    main_window.add_widget(Widget{50, Widget::VERTICAL}, 10, 10);
    main_window.add_widget(Widget{50, Widget::VERTICAL}, 110, 10);
    main_window.add_widget(Widget{50, Widget::SLOPE}, 10, 10);
    main_window.add_widget(Widget{50, Widget::SLOPE}, 60, 10);
}

void Application::run()
{
    running = true;
    while(running) {
        while (handler.poll()) {
            process_event(handler);
        }

        main_window.render();
    }
}

void Application::process_event(cen::event_handler &e)
{
    if (e.is<cen::quit_event>()) {
        running = false;
    }
}

std::ostream& operator<<(std::ostream& os, const Application &a)
{
    os << "Application, with window: " << a.main_window;
    return os;
}
