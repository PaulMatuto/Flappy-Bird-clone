#pragma once

#include <SDL2/SDL.h>

#include "Constants.h"

class Background{
  public:
    Background(int x, int y, int w, int h, SDL_Renderer* renderer);
    ~Background();

    void loadTex(const char* path);

    void update();
    void render();

    void start();
    void stop();

  private:
    float x, y;
    int w, h;
    int speed;

    SDL_Renderer* renderer;
    SDL_Texture* bgTexture;
    SDL_Rect dest1;
    SDL_Rect dest2;
};