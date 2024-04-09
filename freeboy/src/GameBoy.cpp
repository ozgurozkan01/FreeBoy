//
// Created by ozgur on 10/04/2024.
//

#include "../include/GameBoy.h"
#include "../include/Graphic.h"

#include <cstdio>


GameBoy::GameBoy() :
        graphic(new Graphic(160, 144, "GameBoy-Emulator")),
        emulatorState(EmulatorState::RUNNING)
{
}

bool GameBoy::init()
{
    if (!graphic->init())
    {
        printf("ERROR : Graphics could not be loaded!");
        return false;
    }

    return true;
}

void GameBoy::run()
{
    while (emulatorState != EmulatorState::QUIT)
    {
        processEvent();
        ticks++;
    }
}

void GameBoy::processEvent()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        switch (event.key.type)
        {
            case SDL_QUIT:
                emulatorState = EmulatorState::QUIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        emulatorState = EmulatorState::PAUSE;
                        break;
                }
        }
    }
}
