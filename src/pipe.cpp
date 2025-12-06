#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Pipe.h"
#include "Constants.h"

#include <iostream>

Pipe::Pipe(SDL_Renderer* renderer, int screenWidth, int screenHeight, int gap)
    :renderer(renderer), 
     screenWidth(screenWidth), 
     screenHeight(screenHeight), 
     gap(gap),
     speed(SCROLL_SPEED),
     topTex(nullptr),
     botTex(nullptr),
     top({0, 0, 0, 0}),
     bottom({0, 0, 0, 0})
{
    threshold = PIPE_MIN_MARGIN;

    scored = false;
}

void Pipe::loadTex()
{
    topTex = IMG_LoadTexture(renderer, "res/Pipe1.png");
    if (!topTex) {
        std::cerr << "Failed to load Pipe1.png: " << IMG_GetError() << std::endl;
        return;
    }

    botTex = IMG_LoadTexture(renderer, "res/Pipe2.png");
    if (!botTex) {
        std::cerr << "Failed to load Pipe2.png: " << IMG_GetError() << std::endl;
        return;
    }

    // Pipe texture and rect initialization
    SDL_QueryTexture(topTex, NULL, NULL, &top.w, &top.h);
    top.x = screenWidth;
    top.y = (rand() % (screenHeight - 3 * threshold - gap)) + threshold - top.h;
    
    SDL_QueryTexture(botTex, NULL, NULL, &bottom.w, &bottom.h);
    bottom.x = top.x;
    bottom.y = top.y + top.h + gap;
}

void Pipe::setX(int x)
{
    top.x = x;
    bottom.x = x;
}

void Pipe::update()
{
    top.x -= speed;
    bottom.x -= speed;

    if (top.x + top.w < 0)
    {
        top.x = screenWidth;
        int randomY = (rand() % (screenHeight - 3 * threshold - gap)) + threshold;
        
        top.y = randomY - top.h;
        bottom.x = top.x;
        bottom.y = top.y + top.h + gap;

        scored = false;
    }
}

void Pipe::render()
{
    SDL_RenderCopy(renderer, topTex, NULL, &top);
    SDL_RenderCopy(renderer, botTex, NULL, &bottom);
}

SDL_Rect Pipe::getTopRect()
{
    return top;
}

SDL_Rect Pipe::getBotRect()
{
    return bottom;
}

void Pipe::stop()
{
    speed = 0;
}

Pipe::~Pipe()
{
    if (topTex)
        SDL_DestroyTexture(topTex);

    topTex = nullptr;
    
    if (botTex)
        SDL_DestroyTexture(botTex);

    botTex = nullptr;
}