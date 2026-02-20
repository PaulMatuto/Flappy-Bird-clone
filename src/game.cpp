#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Game.h"
#include "GameLogic.h"
#include "Background.h"
#include "Pipe.h"
#include "Bird.h"

#include <iostream>
#include <vector>

Game::Game(SDL_Renderer* renderer)
    :gameState(Start),
     renderer(renderer),
     background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, renderer), 
     foreground(0, SCREEN_HEIGHT - 78, SCREEN_WIDTH, SCREEN_HEIGHT, renderer),
     bird(SCREEN_WIDTH / 2 - BIRD_SIZE, SCREEN_HEIGHT / 2 - BIRD_SIZE, BIRD_SIZE, renderer),
     gameLogic(bird),
     textColor({255, 255, 255, 255})
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

    // Load the Font
    fontSize = 60;
    font = TTF_OpenFont("res/fonts/flappy-font.ttf", fontSize);
    if (!font)
    {
        std::cout << "Error Opening Font: " << TTF_GetError() << std::endl;
        TTF_Quit();
        return;
    }

    score = 0;
}

// Game Loop
void Game::run()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        running = false;
        
        gameState = manageState(gameState, &event);
        
        if(gameState == Play)
            bird.handleInput(&event);
    }

    // State Manager
    switch (gameState)
    {
        case Start:
            foreground.start();
            for (int i = 0; i < 2; i++)
            {
                pipes[i].setX(SCREEN_WIDTH + i * (SCREEN_WIDTH + PIPE_WIDTH) / 2);
                pipes[i].stop();
            }
            bird.reset();
            score = 0;
            SDL_Log("State: Start");
            break;
        case GameOver:
            foreground.stop();
            for (Pipe& pipe : pipes)
                pipe.stop();
            SDL_Log("State: GameOver");
            break;
        case Play:
            foreground.start();
            for (Pipe& pipe : pipes)
                pipe.start();
            bird.start();
            SDL_Log("State: Play");
            break;
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
            gameState = GameOver;
        }
        if (gameLogic.isScoring(pipe) && gameState == Play)
            score++;
    } 
    
    if (bird.onGround())
    {
        gameState = GameOver;
    }

    bird.update();
    bird.render();

    foreground.update();
    foreground.render();
    
    if (gameState == Play || gameState == GameOver)
        RenderScore();

    SDL_RenderPresent(renderer);
}

bool Game::isRunning()
{
    return running;
}

Game::State Game::manageState(Game::State gameState, SDL_Event* event)
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
        SDL_Log("State Changed!");
        switch (gameState)
        {
            case Start:
                return Play;
                break;
            case GameOver:
                return Start;
                break;
            case Play:
                break;
        }
    }
    return gameState;
}

void Game::RenderScore()
{
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), textColor);
    if (!score_surface)
        return;

    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, score_surface);
    if (!scoreTexture)
        return;

    SDL_Rect score_Rect;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &score_Rect.w, &score_Rect.h);

    score_Rect.x = SCREEN_WIDTH / 2 - score_Rect.w / 2;
    score_Rect.y = fontSize;

    SDL_FreeSurface(score_surface);
    SDL_RenderCopy(renderer, scoreTexture, NULL, &score_Rect);

    SDL_DestroyTexture(scoreTexture);
}