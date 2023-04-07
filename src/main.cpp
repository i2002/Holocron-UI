#include "Application.h"
#include "DemoWidget.h"
#include "GridContainer.h"
#include "exceptions.h"
#include "EventDispatcher.h"
#include "TextBox.h"
#include "BoxContainer.h"
#include "Button.h"
#include "TextInput.h"
#include <iostream>

// test functions
void test_constructor_implementation()
{
    std::cout << "------- Test constructors -------\n";
    
    // static tests
    std::cout << "> Static tests\n";
    Window wind1;
    wind1.show();
    Window wind2{"Title"};
    DemoWidget wdg1{cen::iarea{20,20}};
    wind2.set_child(std::make_shared<DemoWidget>(wdg1));
    wind2.show();
    wind2.render_window();
    std::cout << wind2;

    // dynamic tests
    std::cout << "> Dynamic tests\n";

    std::cout << "1.\n";
    Widget *w1 = new Window{"Hello"};
    std::cout << *w1;

    std::cout << "2.\n";
    wind2.set_child(std::make_shared<DemoWidget>(cen::iarea{30, 30}, cen::colors::aqua));
    auto w2 = wind2.clone();
    if(auto w = std::dynamic_pointer_cast<Window>(w2)) {
        w->render_window();
    }
    std::cout << *w2;

    std::cout << "3.\n";
    Window wind3{std::move(*dynamic_cast<Window*>(w2.get()))}; // now w3 is empty
    std::cout << wind3 << "\n" << *w2 << "\n";
    if(auto w = dynamic_cast<Window*>(w2.get())) {
        w->render_window();
        w->hide();
    }
    wind3.hide();
    std::cout << "---\n";

    // window show delay
    SDL_Delay(5000);
    delete w1;
    std::cout << "\n";
}

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
    std::cout << "------- Test containers -------\n";
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
    std::cout << "\n";
}

void test_application()
{
    std::cout << "------- Test application run -------\n";
    Application a;
    std::cout << a;
    a.run();
    std::cout << "\n";
}

void func(int ev, char data)
{
    std::cout << "Event <int>, function handler ";
    std::cout << "(event: " << ev << " with data: " << data << ")\n";
}

void test_event_dispatcher()
{
    std::cout << "------- Test event dispatcher -------\n";
    EventDispatcher<int, double> ev{};

    auto handler = [](int ev, double extra) {
        std::cout << "Event <int>, lambda handler ";
        std::cout << "(event: " << ev << " with data: " << extra << ")\n";
    };
    ev.add_handler<int>([handler](int ev) {handler(ev, 10.3);});
    ev.add_handler<int>([](auto && PH1) { return func(std::forward<decltype(PH1)>(PH1), '3'); });
    ev.add_handler<double>([](double ev) {
        std::cout << "Event <double>, without data (event: " << ev << ")\n";
    });

    std::cout << "> Running <int> event 2\n";
    ev.run_handlers(2);

    std::cout << "> Test remove handlers\n";
    ev.remove_handlers<int>();

    std::cout << "\n> Running <int> event 3\n";
    ev.run_handlers(3);
    std::cout << "\n";
}

void test_application_events()
{
    std::cout << "------- Test application events -------\n";
    auto a = std::make_shared<Application>();
    a->add_event_handler<app_startup_event>([] (const app_startup_event& event) {
        std::cout << "> Application startup\n";
        Window& main_window = event.get_app()->get_window();
        auto cont1 = std::make_shared<GridContainer>(2, 2);
        auto cont2 = std::make_shared<GridContainer>(1, 3);
        auto cont3 = std::make_shared<GridContainer>(1, 4);
        auto ch1 = std::make_shared<DemoWidget>(cen::iarea{10, 10}, cen::colors::aqua);
        auto ch2 = std::make_shared<DemoWidget>(cen::iarea{20, 20}, cen::colors::red);
        auto ch3 = std::make_shared<DemoWidget>(cen::iarea{10, 10}, cen::colors::yellow);
        auto ch_text = std::make_shared<TextBox>("Text 1", cen::iarea{100, 20});
        auto ch_text2 = std::make_shared<TextBox>("Text 2", cen::iarea{100, 20}, TextBox::VerticalAlignment::MIDDLE, TextBox::HorizontalAlignment::CENTER);
        auto ch_text3 = std::make_shared<TextBox>("Text 3", cen::iarea{100, 20}, TextBox::VerticalAlignment::BOTTOM, TextBox::HorizontalAlignment::RIGHT);

        cont2->add_child(ch1, 0, 0);
        cont2->add_child(ch2, 0, 1, 1, 2);
        cont3->add_child(ch_text, 0, 0);
        cont3->add_child(ch_text2, 0, 1);
        cont3->add_child(ch_text3, 0, 2);
        cont3->add_event_handler<cen::mouse_button_event>([] (cen::mouse_button_event, EventActions &actions) {
            std::cout << "> Stop event propagation\n";
            actions.stopPropagation();
        });
        cont1->add_child(ch3, 0, 0, 2);
        cont1->add_child(cont2, 1, 1);
        cont1->add_child(cont3, 0, 1);

        main_window.set_child(cont1);
        main_window.add_event_handler<std::string, Window>([](const std::string &event) {
            std::cout << "> Received window custom event: " << event << "\n";
            return false;
        });

        ch1->add_event_handler<cen::mouse_button_event>([cont3] (cen::mouse_button_event event, EventActions&) {
            if (!event.pressed()) {
                return;
            }

            std::cout << "> Insert new element\n";
            auto ch_text = std::make_shared<TextBox>("Text extra", cen::iarea{100, 20}, TextBox::VerticalAlignment::MIDDLE, TextBox::HorizontalAlignment::CENTER);
            cont3->add_child(ch_text, 0, 3);
        });
    });
    a->run();
}

void test_box_container()
{
    std::cout << "------- Test BoxContainer -------\n";
    auto a = std::make_shared<Application>();
    a->add_event_handler<app_startup_event>([] (const app_startup_event& event) {
        Window& main_window = event.get_app()->get_window();
        auto cont1 = std::make_shared<BoxContainer>(BoxDirection::VERTICAL);
        auto ch1 = std::make_shared<DemoWidget>(cen::iarea{10, 10}, cen::colors::aqua);
        auto ch2 = std::make_shared<DemoWidget>(cen::iarea{20, 200}, cen::colors::red);
        cont1->append_child(ch1, true);
        cont1->append_child(ch2);
        main_window.set_child(cont1);
    });
    a->run();
}

void test_button()
{
    std::cout << "------- Test Button -------\n";
    auto a = std::make_shared<Application>();
    a->add_event_handler<app_startup_event>([] (const app_startup_event& event) {
        Window& main_window = event.get_app()->get_window();
        auto cont1 = std::make_shared<BoxContainer>(BoxDirection::VERTICAL);
        auto ch1 = std::make_shared<Button>("Click me");
        cont1->append_child(ch1);
        main_window.set_child(cont1);
    });
    a->run();
}

void test_text_input()
{
    std::cout << "------- Test Text Input -------\n";
    auto a = std::make_shared<Application>();
    a->add_event_handler<app_startup_event>([] (const app_startup_event& event) {
        Window& main_window = event.get_app()->get_window();
        auto cont1 = std::make_shared<BoxContainer>(BoxDirection::VERTICAL);
        auto ch1 = std::make_shared<Button>("Click me");
        auto input = std::make_shared<TextInput>();
        cont1->append_child(ch1);
        cont1->append_child(input);
        main_window.set_child(cont1);
    });
    std::cout << a << "\n";
    a->run();
}

void tests(int test)
{
    switch (test) {
        case 1:
            test_constructor_implementation();
            break;
        /*case 2:
            test_widget_placement();
            break;*/
        case 3:
            test_containers();
            break;
        case 4:
            test_application();
            break;
        case 5:
            test_event_dispatcher();
            break;
        case 6:
            test_application_events();
            break;
        case 7:
            test_box_container();
            break;
        case 8:
            test_button();
            break;
        case 9:
            test_text_input();
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
    tests(9);

    // Launch application
/*    std::cout << "------- Launch application -------\n";
    try {
        Application a;
        std::cout << "> Application info:\n" << a << "\n";
        a.run();
    } catch (container_error &err) {
        std::cout << "! interface building error: " << err.what() << "\n";
    } catch (holocronui_error &err) {
        std::cout << err.what() << "\n";
    }*/

    return 0;
}
