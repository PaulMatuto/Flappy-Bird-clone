#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Background.h"
#include "Constants.h"

#include <iostream>

Background::Background(int x, int y, int w, int h, SDL_Renderer& renderer)
    :x(x), 
    y(y), 
    w(w), 
    h(h), 
    speed(SCROLL_SPEED), 
    renderer(renderer)
{
}

void Background::loadTex(const char* path)
{
    bgTexture = IMG_LoadTexture(&renderer, path);
    if (!bgTexture) {
        std::cerr << "Failed to load Background.png: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_QueryTexture(bgTexture, NULL, NULL, &dest1.w, &dest1.h);
    SDL_QueryTexture(bgTexture, NULL, NULL, &dest2.w, &dest2.h);

    dest1.x = x;
    dest1.y = y;
    dest2.x = dest1.x + dest1.w;
    dest2.y = y;
}

void Background::update()
{
    dest1.x -= speed;
    dest2.x -= speed;
    
    if (dest1.x + dest1.w <= 0) dest1.x = dest1.w;
    if (dest2.x + dest2.w <= 0) dest2.x = dest2.w;
}

void Background::render()
{
    SDL_RenderCopy(&renderer, bgTexture, NULL, &dest1);
    SDL_RenderCopy(&renderer, bgTexture, NULL, &dest2);
}

void Background::start()
{
    speed = SCROLL_SPEED;
}

void Background::stop()
{
    speed = 0;
}

Background::~Background()
{
    if (bgTexture)
        SDL_DestroyTexture(bgTexture);

    bgTexture = nullptr;
}