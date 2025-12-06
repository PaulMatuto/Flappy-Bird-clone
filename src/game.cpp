#include <SDL2/SDL.h>

#include "Game.h"
#include "GameLogic.h"
#include "Background.h"
#include "Pipe.h"
#include "Bird.h"

#include <iostream>
#include <vector>

Game::Game(SDL_Renderer* renderer)
    :renderer(renderer),
     background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, renderer), foreground(0, SCREEN_HEIGHT - 78, SCREEN_WIDTH, SCREEN_HEIGHT, renderer),
     bird(SCREEN_WIDTH / 2 - BIRD_SIZE, SCREEN_HEIGHT / 2 - BIRD_SIZE, BIRD_SIZE, renderer),
     gameLogic(bird), isAlive(true)
{
    running = true;

    background.loadTex("res/Background.png");
    foreground.loadTex("res/Ground.png");
    
    // Create pipe objects first to prevent reallocation
    for (int i = 0; i < 2; i++)
    {
        pipes.emplace_back(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, PIPE_GAP);
    }

    // Initialize the pipes
    for (int i = 0; i < 2; i++)
    {
        int x = SCREEN_WIDTH + i * (SCREEN_WIDTH + PIPE_WIDTH) / 2;
        pipes[i].loadTex();
        pipes[i].setX(x);
    }

    bird.loadTex();
}

// Game Loop
void Game::run()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        running = false;
        
        if(isAlive)
            bird.handleInput(&event);
    }
    
    if (!isAlive)
    {
        for (Pipe& pipe : pipes)
            pipe.stop();
        foreground.stop();
    }

    SDL_RenderClear(renderer);
    
    background.update();
    background.render();
    
    for (Pipe& pipe : pipes)
    {
        pipe.update();
        pipe.render();
        if (gameLogic.isColliding(pipe))
        {
            isAlive = false;
        }
        gameLogic.isScoring(pipe);
    } 
    
    if (bird.onGround()) isAlive = false;

    bird.update();
    bird.render();

    foreground.update();
    foreground.render();
    
    SDL_RenderPresent(renderer);
}

bool Game::isRunning()
{
    return running;
}