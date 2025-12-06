#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Game.h"
#include "Background.h"
#include "Bird.h"

#include <iostream>

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {   
        std::cerr << "SDL has failed to initialize. Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Flappy Bird!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window)
    {   
        std::cerr << "SDL failed to create a window. Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {   
        std::cerr << "SDL failed to create a renderer. Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Game Icon
    SDL_Surface* iconSurf = IMG_Load("res/Icon.png");
    SDL_SetWindowIcon(window, iconSurf);
    SDL_FreeSurface(iconSurf);

    Game game = Game(renderer);

    while (game.isRunning())
    {   
        game.run();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}