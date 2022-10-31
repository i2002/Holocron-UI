#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include <centurion.hpp>

class Application
{
    Window main_window;
    cen::event_handler handler;
    bool running;

    void process_event(cen::event_handler &e);
public:
    Application();
    friend std::ostream& operator<<(std::ostream& os, const Application &a);
    void run();
};

#endif // APPLICATION_H
