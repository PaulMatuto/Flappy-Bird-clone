#pragma once

#include <SDL2/SDL.h>

#include "Bird.h"
#include "Pipe.h"
#include "Constants.h"

class GameLogic{
  public:
    GameLogic(Bird& bird);

    bool isColliding(Pipe& pipe);
    bool isScoring(Pipe& pipe);
    bool onGround();

  private:
    Bird& bird;

    SDL_Rect birdRect;
    SDL_Rect pipeTopRect;
    SDL_Rect pipeBotRect;
    
    int ground;
};