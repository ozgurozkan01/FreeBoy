//
// Created by ozgur on 10/04/2024.
//

#include "../include/GameBoy.h"
#include "../include/InterruptHandler.h"
#include "../include/Cartridge.h"
#include "../include/Joypad.h"
#include "../include/Timer.h"
#include "../include/CPU.h"
#include "../include/MMU.h"
#include "../include/PPU.h"
#include "../include/IO.h"
#include <cstdio>

namespace gameboy
{
    GameBoy::GameBoy(std::string _romPath) :
            romPath(_romPath),
            currentState(EmulatorState::running),
            ticks(0)
            {}

    bool GameBoy::init()
    {
        cartridge = new Cartridge();
        if (!cartridge->load(romPath))
        {
            printf("ERROR : Cartridge could not be created!\n");
            return false;
        }

        interruptHandler = new InterruptHandler();
        joypad = new Joypad(this);
        timer = new Timer(interruptHandler);
        ppu = new PPU(cartridge->getTitle());
        io = new IO(joypad, timer, interruptHandler);
        mmu = new MMU(cartridge, interruptHandler, ppu, io);
        cpu = new CPU(this, mmu, interruptHandler);

        return true;
    }

    GameBoy::~GameBoy()
    {
        if (cpu != nullptr) { delete cpu; }
        if (mmu != nullptr) { delete mmu; }
        if (io != nullptr) { delete io; }
        if (ppu != nullptr) { delete ppu; }
        if (interruptHandler != nullptr) { delete interruptHandler; }
        if (joypad != nullptr) { delete joypad; }
        if (timer != nullptr) { delete timer; }
        if (cartridge != nullptr) { delete cartridge; }
    }

    void GameBoy::run()
    {
        while (currentState != EmulatorState::quit)
        {
            ppu->render();
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
            joypad->buttonEvent(event);
        }
    }

    void GameBoy::emulateCycles(uint8_t mCycle)
    {
        for (int mCounter = 0; mCounter < mCycle; ++mCounter)
        {
            for (int tCounter = 0; tCounter < 4; ++tCounter)
            {
                ticks++;
                timer->tick();
            }
        }
    }
}