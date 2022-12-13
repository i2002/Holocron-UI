#include "Application.h"
#include "DemoWidget.h"
#include <iostream>

// test functions
void test_constructor_implementation()
{
    // static tests
    Window wind1;
    wind1.show();
    Window wind2{"Titlu"};
    DemoWidget wdg1{cen::iarea{20,20}};
    wind2.add_child(&wdg1, cen::ipoint{10, 10});
    wind2.show();
    wind2.render();
    std::cout << wind2;

    // dynamic tests
    std::cout << "1.\n";
    Widget *w1 = new Window{"Hello"};
    std::cout << *w1;

    std::cout << "2.\n";
    wind2.add_child(new DemoWidget{cen::iarea{30, 30}, cen::colors::aqua}, cen::ipoint{50, 50});
    Widget *w2 = wind2.clone();
    if(auto w = dynamic_cast<Window*>(w2)) {
        w->render();
    }
    std::cout << *w2;

    std::cout << "3.\n";
    Window wind3{std::move(*(Window*)w2)}; // now w3 is empty
    std::cout << wind3 << "\n" << *w2 << "\n";
    if(auto w = dynamic_cast<Window*>(w2)) {
        w->render();
        w->hide();
    }
    wind3.hide();
    std::cout << "---\n";

    // window show delay
    SDL_Delay(5000);
}


int main(int, char**)
{
    // initialize SDL library
    const cen::sdl sdl;
    const cen::img img;

    // Application a;
    // std::cout << a;
    // a.run();

    test_constructor_implementation();
    return 0;
}
