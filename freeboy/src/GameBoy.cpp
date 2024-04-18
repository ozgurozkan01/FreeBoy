//
// Created by ozgur on 10/04/2024.
//

#include "../include/GameBoy.h"
#include "../include/Graphic.h"
#include "../include/Cartridge.h"
#include <cstdio>

namespace gameboy
{
    GameBoy::GameBoy() :
            graphic(new Graphic(160, 144, "GameBoy-Emulator")),
            emulatorState(EmulatorState::RUNNING),
            cartridge(new Cartridge())
    {
    }

    bool GameBoy::init()
    {
        if (!graphic->init())
        {
            printf("ERROR : Graphics could not be loaded!");
            return false;
        }

        if (!cartridge->loadCartridge("C:/Users/ozgur/GitHub/FreeBoy/ROMs/dmg-acid2.gb"))
        {
            printf("Cartridge could not be created! Error code %s\n", SDL_GetError());
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

    GameBoy::~GameBoy()
    {
        delete cartridge;
        delete graphic;
    }

}