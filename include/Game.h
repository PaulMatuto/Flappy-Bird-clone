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
    Game();
    ~Game();

    enum State {
      Start,
      Play,
      GameOver
    };

    void init();
    void run();
    void RenderScore();
    void shutdown();

    bool isRunning();
    Game::State manageState(SDL_Event* event);

  private:
    bool running;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    Background* background;
    Background* foreground;
    Bird* bird;
    
    GameLogic* gameLogic;
    
    std::vector<Pipe> pipes;
    
    int fontSize;
    int score;
    SDL_Color textColor;
    TTF_Font* font;

    Game::State gameState;
};