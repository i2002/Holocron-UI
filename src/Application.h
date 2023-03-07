#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include <centurion.hpp>
#include <utility>

class Application;

class app_event
{
    Application* app;
public:
    explicit app_event(Application* app) {
        this->app = app;
    }

    [[nodiscard]] Application* get_app() const {
        return app;
    }
};

class app_startup_event : public app_event
{
public:
    explicit app_startup_event(Application* app) : app_event(app) {}
};

class app_shutdown_event : public app_event
{
public:
    explicit app_shutdown_event(Application* app) : app_event(app) {}
};

class Application
{
    Window main_window;
    cen::event_handler handler;
    bool running = false;

    void process_event(cen::event_handler &e);
public:
    Application();
    friend std::ostream& operator<<(std::ostream& os, const Application &a);
    void run();

    Window& get_window();

    template <typename Event>
    void add_event_handler(handler_func_t<Event> hand);

protected:
    EventDispatcher<app_startup_event, app_shutdown_event> dispatcher;
};

template <typename Event>
void Application::add_event_handler(handler_func_t<Event> hand) {
    dispatcher.add_handler<Event>(hand);
}


#endif // APPLICATION_H
