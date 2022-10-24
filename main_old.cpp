#include <SDL.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

int main(int argv, char** args){
    for(int i = 0; i < argv; i++) {
        std::cout << args[i] << '\n';
    }
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create an application window with the following settings:
    SDL_Window *window = SDL_CreateWindow(
            "An SDL2 window",                  // window title
            SDL_WINDOWPOS_UNDEFINED,           // initial x position
            SDL_WINDOWPOS_UNDEFINED,           // initial y position
            640,                               // width, in pixels
            480,                               // height, in pixels
            SDL_WINDOW_SHOWN                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        std::cout << "Could not create window: %s\n" << SDL_GetError();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, 0);


    // The window is open: could enter program loop here (see SDL_PollEvent())
    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example
//
//    while(true) {
//        SDL_Delay(1000);
//    }

    SDL_ShowWindow(window);
    SDL_Event e;
    bool done = false;
   while (!done){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                done = true;
                break;
            }

            SDL_RenderDrawLine(renderer, 0, 0, 100, 100);
        }
   }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}