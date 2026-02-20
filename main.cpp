#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Game.h"
#include "Background.h"
#include "Bird.h"

#include <iostream>

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {   
        SDL_Log("SDL has failed to initialize. Error: %s", SDL_GetError());
        return 1;
    }
    SDL_Log("SDL has initialized successfully");

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("SDL_IMG has failed to initialize. Error: %s", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Log("SDL_IMG has initialized successfully!");

    if (TTF_Init() != 0)
    {
        SDL_Log("SDL_TTF has failed to initialize. Error: %s", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Log("SDL_TTF has initialized successfully!");
    
    SDL_Window* window = SDL_CreateWindow("Flappy Bird!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window)
    {   
        SDL_Log("SDL failed to create a window. Error: %s", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Log("SDL created a window successfully!");
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {   
        SDL_Log("SDL failed to create a renderer. Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Log("SDL created a renderer successfully!");

    // Game Icon
    const char* iconPath = "res/Icon.png";
    SDL_Surface* iconSurf = IMG_Load(iconPath);
    if (!iconSurf)
    {
        SDL_Log("SDL_IMG failed to load %s . Error: %s", iconPath, SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Log("SDL_IMG loaded %s successfully!", iconPath);
    SDL_SetWindowIcon(window, iconSurf);
    SDL_FreeSurface(iconSurf);

    Game game = Game(*renderer);

    while (game.isRunning())
    {   
        game.run();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}