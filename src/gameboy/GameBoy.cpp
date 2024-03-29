//
// Created by ozgur on 27/03/2024.
//

#include "GameBoy.h"
#include <cstdio>
#include "../../src/graphics/Graphic.h"

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

/*    if (!cartridge->loadCartridge((char*)""))
    {
        printf("ERROR : Cartridge could not be loaded!");
        return false;
    }*/

    return true;
}

void GameBoy::run()
{
    while (emulatorState != EmulatorState::QUIT)
    {
        //graphic->run();
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
