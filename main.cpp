#include <centurion/centurion.hpp>

int main()
{
    const cen::sdl sdl;
    const cen::img img;

    /* Create a window and an associated renderer */
    cen::window window{"Demo", cen::iarea{}};
    cen::renderer renderer = window.make_renderer();
    
    /* Make sure our window is visible */
    window.show();
    
    cen::event_handler event;
    bool running = true;
    
    while (running) {
        while (event.poll()) {
            /* Check if the user wants to quit the application */
            if (event.is<cen::quit_event>()) {
                running = false;
                break;
            }
        }
    
        renderer.clear_with(cen::colors::coral);
        renderer.draw_line(cen::ipoint{5, 5}, cen::ipoint{100, 100});
        /* Miscellaneous rendering code goes here... */
    
        renderer.present();
    }
    
    /* Make the window invisible again, this might not be necessary, but it doesn't hurt */
    window.hide();

    return 0;
}