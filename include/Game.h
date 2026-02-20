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

    enum State {
      Start,
      Play,
      GameOver
    };

    void run();
    void RenderScore();

    bool isRunning();
    Game::State manageState(State gameState, SDL_Event* event);

  private:
    bool running;
    Game::State gameState;
  
    SDL_Renderer* renderer;

    Background background;
    Background foreground;
    Bird bird;

    GameLogic gameLogic;
    
    std::vector<Pipe> pipes;

    bool isAlive;

    int fontSize;
    int score;
    SDL_Color textColor;
    TTF_Font* font;
};