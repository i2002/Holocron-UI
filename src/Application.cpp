#include "Application.h"
#include <iostream>

Application::Application() :
    main_window{"Title"}
{}

void Application::run()
{
    dispatcher.run_handlers(app_startup_event(this));
    main_window.show();
    running = true;
    while(running) {
        while (handler.poll()) {
            process_event(handler);
        }

        main_window.render_window();
    }
    dispatcher.run_handlers(app_shutdown_event(this));
}

void Application::process_event(cen::event_handler &e)
{
    // application events
    if (e.is<cen::quit_event>()) {
        #ifdef HOLOCRONUI_DEBUG_MESSAGES
        std::cout << "--- Application quit event received\n";
        #endif // HOLOCRONUI_DEBUG_MESSAGES
        running = false;
    }

    // window events
    if(e.is(cen::event_type::window)) {
        const auto& window_event = e.get<cen::window_event>();
        switch(window_event.event_id()) {
            case cen::window_event_id::resized:
            case cen::window_event_id::size_changed:
                    #ifdef HOLOCRONUI_DEBUG_MESSAGES
                    std::cout << "--- Window resize event received\n";
                    std::cout << "new window size: " << window_event.data1() << "x" << window_event.data2() << "\n";
                    #endif // HOLOCRONUI_DEBUG_MESSAGES
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
        #ifdef HOLOCRONUI_DEBUG_MESSAGES
        std::cout << "--- Window click event received\n";
        #endif // HOLOCRONUI_DEBUG_MESSAGES
        main_window.process_event(e.get<cen::mouse_button_event>());

        #ifdef HOLOCRONUI_DEBUG_MESSAGES
        std::cout << "\n";
        #endif // HOLOCRONUI_DEBUG_MESSAGES
    } else if (e.is(cen::event_type::mouse_motion)) {
        main_window.process_event(e.get<cen::mouse_motion_event>());
    }
}

Window& Application::get_window() {
    return main_window;
}

std::ostream& operator<<(std::ostream& os, const Application &a)
{
    os << "Application, with window: " << a.main_window;
    return os;
}
