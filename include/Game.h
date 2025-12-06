#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameLogic.h"
#include "Background.h"
#include "Pipe.h"
#include "Bird.h"

#include <vector>

class Game{
  public:
    Game(SDL_Renderer* renderer);

    void run();

    bool isRunning();

  private:
    bool running;
  
    SDL_Renderer* renderer;

    Background background;
    Background foreground;
    Bird bird;

    GameLogic gameLogic;
    
    std::vector<Pipe> pipes;

    bool isAlive;

};