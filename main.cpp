#include <SDL.h>
#include <iostream>

int main(int argv, char** args){
    for(int i = 0; i < argv; i++) {
        std::cout << args[i] << '\n';
    }
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Quit();
    return 0;
}