#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Constants.h"

class Pipe{
  public:
    Pipe(SDL_Renderer& renderer, int screenWidth, int screenHeight, int gap);
    ~Pipe();

    void loadTex();

    void update();
    void render();

    SDL_Rect getTopRect();
    SDL_Rect getBotRect();
    
    void setX(int x);
    void start();
    void stop();

  private:
    SDL_Renderer* renderer;
    
    int screenWidth;
    int screenHeight;
    int gap;
    int threshold;
    int speed;

    SDL_Texture* topTex;
    SDL_Texture* botTex;
    SDL_Rect top;
    SDL_Rect bottom;

    bool scored;
};