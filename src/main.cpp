#include "Application.h"
#include <iostream>

int main(int, char**)
{
    // initialize SDL library
    const cen::sdl sdl;
    const cen::img img;

    Application a;
    std::cout << a;
    a.run();
    return 0;
}
