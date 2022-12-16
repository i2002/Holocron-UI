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

void test_widget_placement()
{
    Window w{"Titlu"};
    Widget *ch1 = new DemoWidget{cen::iarea{30, 30}, cen::colors::aqua};
    Widget *ch2 = new DemoWidget{cen::iarea{30, 30}, cen::colors::red};
    Widget *ch3 = new DemoWidget{cen::iarea{30, 30}, cen::colors::yellow};
    Widget *ch4 = new DemoWidget{cen::iarea{30, 30}, cen::colors::dark_gray};
    Widget *ch5 = new DemoWidget{cen::iarea{60, 60}, cen::colors::green};

    w.add_child(ch1, cen::ipoint{50, 50});
    w.add_child(ch2, cen::ipoint{80, 65});
    w.add_child(ch3, cen::ipoint{50, 80});
    w.add_child(ch4, cen::ipoint{20, 20});
    w.add_child(ch5, cen::ipoint{20, 20});
    w.show();
    w.render();

    SDL_Delay(5000);
    delete ch1;
    delete ch2;
    delete ch3;
    delete ch4;
    delete ch5;
}

void tests(int test)
{
    switch (test) {
        case 1:
            test_constructor_implementation();
            break;
        case 2:
            test_widget_placement();
            break;
        default:
            std::cout << "Invalid test\n";
            break;
    }
}

int main(int, char**)
{
    // initialize SDL library
    const cen::sdl sdl;
    const cen::img img;

    // Application a;
    // std::cout << a;
    // a.run();

    tests(2);
    return 0;
}
