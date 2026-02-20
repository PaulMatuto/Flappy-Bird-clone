#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Game.h"

#include <iostream>

int main(int argc, char* argv[])
{
    Game game = Game();

    while (game.isRunning())
    {   
        game.run();
    }

    game.shutdown();

    return 0;
}