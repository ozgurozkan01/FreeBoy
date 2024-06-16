//
// Created by ozgur on 10/04/2024.
//

#include "../include/GameBoy.h"
#include "../include/InterruptHandler.h"
#include "../include/Cartridge.h"
#include "../include/CPU.h"
#include "../include/MMU.h"
#include <cstdio>

namespace gameboy
{
    GameBoy::GameBoy(std::string _romPath) : emulatorState(EmulatorState::RUNNING), romPath(_romPath) {}

    bool GameBoy::init()
    {
        interruptHandler = new InterruptHandler();

        cartridge = new Cartridge();
        if (!cartridge->load(romPath))
        {
            printf("ERROR : Cartridge could not be created!\n");
            return false;
        }

        mmu = new MMU(cartridge, interruptHandler);
        if (!mmu->init())
        {
            printf("ERROR : CPU could not be initialized!");
            return false;
        }

        cpu = new CPU(this, mmu, interruptHandler);
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
        if (cpu != nullptr) { delete cpu; }
        if (cartridge != nullptr) { delete cartridge; }
    }

    void GameBoy::emulateCycles(uint8_t cycleCount)
    {
        // TODO : increment cycle
    }
}