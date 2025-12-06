#include <SDL2/SDL.h>

#include "Bird.h"
#include "Pipe.h"
#include "Constants.h"
#include "GameLogic.h"

#include <iostream>

GameLogic::GameLogic(Bird& bird)
    :bird(bird)
{
}

bool GameLogic::isColliding(Pipe& pipe)
{
    birdRect = bird.getRect();
    pipeTopRect = pipe.getTopRect();
    pipeBotRect = pipe.getBotRect();

    if ((birdRect.x + birdRect.w > pipeTopRect.x && birdRect.x < pipeTopRect.x + pipeTopRect.w &&
                birdRect.y + birdRect.h > pipeTopRect.y && birdRect.y < pipeTopRect.y + pipeTopRect.h)
             || (birdRect.x + birdRect.w > pipeBotRect.x && birdRect.x < pipeBotRect.x + pipeBotRect.w &&
                birdRect.y + birdRect.h > pipeBotRect.y && birdRect.y < pipeBotRect.y + pipeBotRect.h))
    {
        return true;
    }

    return false;
}

bool GameLogic::isScoring(Pipe& pipe)
{
    if (birdRect.x + SCROLL_SPEED > pipeTopRect.x + pipeTopRect.w && birdRect.x < pipeTopRect.x + pipeTopRect.w)
    {
        SDL_Log("Scoring");
        return true;
    }
    return false;
}