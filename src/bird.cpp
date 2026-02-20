#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Bird.h"
#include "Constants.h"

#include <iostream>

Bird::Bird(float x_pos, float y_pos, int size, SDL_Renderer& renderer)
    :initialPos(y_pos),
    x_pos(x_pos), 
    y_pos(y_pos), 
    size(size), 
    renderer(renderer),
    dest({(int) x_pos, (int) y_pos, size, size}),
    spriteCounter(0), 
    spriteNum(1)
{
    velocity = 0;
    acceleration = GRAVITY_MULTIPLIER * SDL_STANDARD_GRAVITY / 60.0f;

}

void Bird::loadTex()
{
    birdTex1 = IMG_LoadTexture(&renderer, "res/bird1.png");
    if (!birdTex1) {
        std::cerr << "Failed to load Bird1.png: " << IMG_GetError() << std::endl;
        return;
    }
    
    birdTex2 = IMG_LoadTexture(&renderer, "res/bird2.png");
    if (!birdTex2) {
        std::cerr << "Failed to load Bird2.png: " << IMG_GetError() << std::endl;
        return;
    }

    dest.w = size;
    dest.h = size;
}

void Bird::reset()
{
    y_pos = initialPos;
    stop();
}

void Bird::start()
{
    acceleration = GRAVITY_MULTIPLIER * SDL_STANDARD_GRAVITY / 60.0f;
}

void Bird::stop()
{
    velocity = 0;
    acceleration = 0;
}

void Bird::update()
{
    velocity += acceleration;
    y_pos += velocity;

    int ceiling = 0;
    if (y_pos < ceiling)
    {
        y_pos = ceiling;
        velocity = 0;
    }
    
    dest.x = x_pos;
    dest.y = y_pos;
}

void Bird::render()
{
    // animation
    if(spriteCounter > 5)
    {
        if (spriteNum == 1)
        {
            spriteNum++;
        }
        else if (spriteNum == 2)
        {
            spriteNum--;
        }
        spriteCounter = 0;
    }
    if (velocity <= 5) spriteCounter++;    

    if (spriteNum == 1) SDL_RenderCopy(&renderer, birdTex1, NULL, &dest);
    if (spriteNum == 2) SDL_RenderCopy(&renderer, birdTex2, NULL, &dest);
}

void Bird::handleInput(SDL_Event* event)
{
    bool isPressed = false;
    
    switch (event->type)
    {
    case SDL_KEYDOWN:
        switch (event->key.keysym.scancode)
        {
        case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_SPACE:
            if (!event->key.repeat)
            isPressed = true;
                break;
            default:
                break;
        }
        break;
    
    case SDL_MOUSEBUTTONDOWN:
        if (!event->key.repeat)
                isPressed = true;
        break;
    }

    if (isPressed)
    {
        velocity = -BIRD_JUMP_SPEED;
    }
}

SDL_Rect Bird::getRect()
{
    return dest;
}

bool Bird::onGround()
{
    int ground = SCREEN_HEIGHT - 78 - dest.h;
    if (y_pos >= ground)
    {
        stop();
        y_pos = ground;
        spriteCounter = -1;
        
        return true;
    }   
    return false; 
}

Bird::~Bird()
{
    if (birdTex1)
        SDL_DestroyTexture(birdTex1);

    birdTex1 = nullptr;

    if (birdTex2)
        SDL_DestroyTexture(birdTex2);

    birdTex2 = nullptr;
}