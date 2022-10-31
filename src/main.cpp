#include "Application.h"
#include <iostream>

int main()
{
    // initialize SDL library
    const cen::sdl sdl;
    const cen::img img;

    Application a;
    std::cout << a;
    a.run();
    return 0;
}
