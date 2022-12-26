#include "Application.h"
#include "DemoWidget.h"
#include "GridContainer.h"
#include "exceptions.h"
#include <iostream>

// test functions
/*
void test_constructor_implementation()
{
    // static tests
    Window wind1;
    wind1.show();
    Window wind2{"Title"};
    DemoWidget wdg1{cen::iarea{20,20}};
    wind2.add_child(std::make_shared<DemoWidget>(wdg1), cen::ipoint{10, 10});
    wind2.show();
    wind2.render();
    std::cout << wind2;

    // dynamic tests
    std::cout << "1.\n";
    Widget *w1 = new Window{"Hello"};
    std::cout << *w1;

    std::cout << "2.\n";
    wind2.add_child(std::make_shared<DemoWidget>(cen::iarea{30, 30}, cen::colors::aqua), cen::ipoint{50, 50});
    auto w2 = wind2.clone();
    if(auto w = std::dynamic_pointer_cast<Window>(w2)) {
        w->render();
    }
    std::cout << *w2;

    std::cout << "3.\n";
    Window wind3{std::move(*dynamic_cast<Window*>(w2.get()))}; // now w3 is empty
    std::cout << wind3 << "\n" << *w2 << "\n";
    if(auto w = dynamic_cast<Window*>(w2.get())) {
        w->render();
        w->hide();
    }
    wind3.hide();
    std::cout << "---\n";

    // window show delay
    SDL_Delay(5000);
}
*/

/*void test_widget_placement()
{
    Window w{"Title"};
    w.add_child(std::make_shared<DemoWidget>(cen::iarea{30, 30}, cen::colors::aqua), cen::ipoint{50, 50});
    w.add_child(std::make_shared<DemoWidget>(cen::iarea{30, 30}, cen::colors::red), cen::ipoint{80, 65});
    w.add_child(std::make_shared<DemoWidget>(cen::iarea{30, 30}, cen::colors::yellow), cen::ipoint{50, 80});
    w.add_child(std::make_shared<DemoWidget>(cen::iarea{30, 30}, cen::colors::dark_gray), cen::ipoint{20, 20});
    w.add_child(std::make_shared<DemoWidget>(cen::iarea{60, 60}, cen::colors::green), cen::ipoint{20, 20});
    w.show();
    w.render();

    SDL_Delay(5000);
}*/

void test_containers()
{
    Window w("Title");
    auto cont1 = std::make_shared<GridContainer>(2, 2);
    auto cont2 = std::make_shared<GridContainer>(1, 3);
    auto ch1 = std::make_shared<DemoWidget>(cen::iarea{10, 10}, cen::colors::aqua);
    auto ch2 = std::make_shared<DemoWidget>(cen::iarea{20, 20}, cen::colors::red);
    auto ch3 = std::make_shared<DemoWidget>(cen::iarea{10, 10}, cen::colors::yellow);

    try {
        cont2->add_child(ch1, 0, 0);
        cont2->add_child(ch2, 0, 1, 1, 2);
        cont1->add_child(ch3, 0, 0, 2, 2);
        cont1->add_child(cont2, 1, 1);
    } catch (container_error &err) {
        std::cout << "interface building error: " << err.what() << "\n";
    } catch (holocronui_error &err) {
        std::cout << err.what() << "\n";
    }

    w.set_child(cont1);
    w.show();
    w.render_window();
    std::cout << w;
    SDL_Delay(5000);

    auto w2 = w.clone();
    if(auto wind = dynamic_cast<Window*>(w2.get())) {
        wind->show();
        wind->render_window();
    }
    std::cout << *w2;
    SDL_Delay(5000);
}

void test_application()
{
    Application a;
    std::cout << a;
    a.run();
}

void tests(int test)
{
    switch (test) {
/*        case 1:
            test_constructor_implementation();
            break;
        case 2:
            test_widget_placement();
            break;*/
        case 3:
            test_containers();
            break;
        case 4:
            test_application();
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
    const cen::ttf ttf;

    // Tests
    tests(3);

    // Launch application
    try {
        Application a;
        std::cout << a;
        a.run();
    } catch (container_error &err) {
        std::cout << "interface building error: " << err.what() << "\n";
    } catch (holocronui_error &err) {
        std::cout << err.what() << "\n";
    }

    return 0;
}
