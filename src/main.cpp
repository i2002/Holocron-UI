#include "Application.h"
#include "DemoWidget.h"
#include <iostream>

int main(int, char**)
{
    // initialize SDL library
    const cen::sdl sdl;
    const cen::img img;

    // Application a;
    // std::cout << a;
    // a.run();

    // static tests
    Window wind1;
    Window wind2{"Titlu"};
    DemoWidget wdg1{cen::iarea{20,20}};
    wind2.add_child(&wdg1, cen::ipoint{10, 10});
    wind2.render();
    std::cout << *((Widget*)&wind2); //QUESTION: any better way to do this?

    // dynamic tests
    std::cout << "1.\n";
    Widget *w1 = new Window{"Hello"};
    std::cout << *w1;

    std::cout << "2.\n";
    Widget *w2 = wind2.clone(); // QUESTION: why window not duplicated?
    if(Window *w = dynamic_cast<Window*>(w2)) {
        w->render();
    }
    std::cout << *w2;

    std::cout << "3.\n";
    Widget *w3{w2};
    std::cout << *w3;

    std::cout << "4.\n";
    Widget *w4 = &wind2;
    std::cout << *w4;
    std::cout << "---\n";

    // window show delay
    SDL_Delay(5000);
    return 0;
}
