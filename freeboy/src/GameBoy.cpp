//
// Created by ozgur on 10/04/2024.
//

#include "../include/GameBoy.h"
#include "../include/Graphic.h"
#include "../include/Cartridge.h"
#include "../include/Bus.h"
#include "../include/CPU.h"
#include <cstdio>

namespace gameboy
{
    GameBoy::GameBoy() : emulatorState(EmulatorState::RUNNING) {}

    bool GameBoy::init()
    {
        graphic = new Graphic(160, 144, "GameBoy-Emulator");
        if (!graphic->init())
        {
            printf("ERROR : Graphics could not be loaded!");
            return false;
        }

        cartridge = new Cartridge();
        if (!cartridge->load("C:/Users/ozgur/GitHub/FreeBoy/ROMs/cpu_instrs.gb"))
        {
            printf("Cartridge could not be created! Error code %s\n", SDL_GetError());
            return false;
        }

        cpu = new CPU(this);
        if (!cpu->init())
        {
            printf("ERROR : CPU could not be initialized!");
            return false;
        }

        return true;
    }

    void GameBoy::run()
    {
        while (emulatorState != EmulatorState::QUIT)
        {
            cpu->step();
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
        delete cpu;
        delete cartridge;
        delete graphic;
    }

    void GameBoy::emulateCycles(uint8_t cycleCount)
    {
        // TODO : increment cycle
    }

    Cartridge *GameBoy::getCartridge() const { return cartridge; }
}