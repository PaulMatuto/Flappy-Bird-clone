#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Constants.h"

class Bird{
  public:
    Bird(float x_pos, float y_pos, int size, SDL_Renderer* renderer);
    ~Bird();

    void loadTex();
    void handleInput(SDL_Event* event);

    void update();
    void render();

    bool onGround();
    
    SDL_Rect getRect();
    void setVel(int vel);

  private:
    float x_pos, y_pos;
    float velocity;
    float acceleration;
    int size;
    
    SDL_Renderer* renderer;
    SDL_Texture* birdTex1;
    SDL_Texture* birdTex2;
    SDL_Rect dest;

    int spriteCounter;
    int spriteNum;
};