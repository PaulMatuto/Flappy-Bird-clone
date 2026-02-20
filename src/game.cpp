#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Game.h"
#include "GameLogic.h"
#include "Background.h"
#include "Pipe.h"
#include "Bird.h"

#include <iostream>
#include <vector>

Game::Game()
    :textColor({255, 255, 255, 255}),
     gameState(Start)
{
    running = true;

    init();
    
    background = new Background(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, *renderer);
    foreground = new Background(0, SCREEN_HEIGHT - 78, SCREEN_WIDTH, SCREEN_HEIGHT, *renderer);
    bird = new Bird(SCREEN_WIDTH / 2 - BIRD_SIZE, SCREEN_HEIGHT / 2 - BIRD_SIZE, BIRD_SIZE, *renderer);
    gameLogic = new GameLogic(*bird);


    background->loadTex("res/Background.png");
    foreground->loadTex("res/Ground.png");
    
    // Create pipe objects first to prevent reallocation
    for (int i = 0; i < 2; i++)
    {
        pipes.emplace_back(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT, PIPE_GAP);
    }

    // Initialize the pipes
    for (int i = 0; i < 2; i++)
    {
        int x = SCREEN_WIDTH + i * (SCREEN_WIDTH + PIPE_WIDTH) / 2;
        pipes[i].loadTex();
        pipes[i].setX(x);
    }

    bird->loadTex();

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

void Game::init()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {   
        SDL_Log("SDL has failed to initialize. Error: %s", SDL_GetError());
        return;
    }
    SDL_Log("SDL has initialized successfully");

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("SDL_IMG has failed to initialize. Error: %s", IMG_GetError());
        SDL_Quit();
        return;
    }
    SDL_Log("SDL_IMG has initialized successfully!");

    if (TTF_Init() != 0)
    {
        SDL_Log("SDL_TTF has failed to initialize. Error: %s", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return;
    }
    SDL_Log("SDL_TTF has initialized successfully!");
    
    window = SDL_CreateWindow("Flappy Bird!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window)
    {   
        SDL_Log("SDL failed to create a window. Error: %s", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        IMG_Quit();
        SDL_Quit();
        return;
    }
    SDL_Log("SDL created a window successfully!");
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {   
        SDL_Log("SDL failed to create a renderer. Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return;
    }
    SDL_Log("SDL created a renderer successfully!");

    // Game Icon
    const char* iconPath = "res/Icon.png";
    SDL_Surface* iconSurf = IMG_Load(iconPath);
    if (!iconSurf)
    {
        SDL_Log("SDL_IMG failed to load %s . Error: %s", iconPath, SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return;
    }
    SDL_Log("SDL_IMG loaded %s successfully!", iconPath);
    SDL_SetWindowIcon(window, iconSurf);
    SDL_FreeSurface(iconSurf);
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
            bird->handleInput(&event);
    }

    // State Manager
    switch (gameState)
    {
    case Start:
        foreground->start();
        for (int i = 0; i < 2; i++)
        {
            pipes[i].setX(SCREEN_WIDTH + i * (SCREEN_WIDTH + PIPE_WIDTH) / 2);
            pipes[i].stop();
        }
        bird->reset();
        score = 0;
        SDL_Log("State: Start");
        break;
    case GameOver:
        foreground->stop();
        for (Pipe& pipe : pipes)
            pipe.stop();
        SDL_Log("State: GameOver");
        break;
    case Play:
        foreground->start();
        for (Pipe& pipe : pipes)
            pipe.start();
        bird->start();
        SDL_Log("State: Play");
        break;
    }

    SDL_RenderClear(renderer);
    
    background->update();
    background->render();
    
    for (Pipe& pipe : pipes)
    {
        pipe.update();
        pipe.render();
        if (gameLogic->isColliding(pipe))
        {
            gameState = GameOver;
        }
        if (gameLogic->isScoring(pipe) && gameState == Play)
            score++;
    } 
    
    if (bird->onGround())
    {
        gameState = GameOver;
    }

    bird->update();
    bird->render();

    foreground->update();
    foreground->render();
    
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

void Game::shutdown()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Game::RenderScore()
{
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), textColor);
    if (!score_surface)
        return;

    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, score_surface);
    if (!scoreTexture)
    {
        SDL_FreeSurface(score_surface);
        return;
    }

    SDL_Rect score_Rect;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &score_Rect.w, &score_Rect.h);

    score_Rect.x = SCREEN_WIDTH / 2 - score_Rect.w / 2;
    score_Rect.y = fontSize;

    SDL_FreeSurface(score_surface);
    SDL_RenderCopy(renderer, scoreTexture, NULL, &score_Rect);

    SDL_DestroyTexture(scoreTexture);
}

Game::~Game()
{
    if (font)
        TTF_CloseFont(font);

    font = nullptr;
}