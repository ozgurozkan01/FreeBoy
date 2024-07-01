//
// Created by ozgur on 10/04/2024.
//

#include "../include/GameBoy.h"
#include "../include/InterruptHandler.h"
#include "../include/Cartridge.h"
#include "../include/Renderer.h"
#include "../include/Joypad.h"
#include "../include/Timer.h"
#include "../include/CPU.h"
#include "../include/MMU.h"
#include "../include/PPU.h"
#include "../include/DMA.h"
#include "../include/IO.h"
#include <cstdio>
#include <thread>

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

        mmu = new MMU(this);
        ppu = new PPU();
        renderer = new Renderer(cartridge->getTitle(), mmu);
        if (!renderer->init())
        {
            printf("ERROR : SDL Window could not be created!\n");
            return false;
        }

        interruptHandler = new InterruptHandler();
        joypad = new Joypad(this);
        timer = new Timer(interruptHandler);
        dma = new DMA(ppu, mmu);
        io = new IO(joypad, timer, interruptHandler, dma);
        cpu = new CPU(this, mmu, interruptHandler);

        return true;
    }

    void GameBoy::run()
    {
        std::thread cpuThread(&CPU::run, cpu);

        while (currentState != EmulatorState::quit)
        {
            processEvent();
            renderer->renderDebugger();
        }

        if (cpuThread.joinable()) { cpuThread.join(); }
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
            dma->tick();
        }
    }

    GameBoy::~GameBoy()
    {
        if (cpu) { delete cpu; }
        if (mmu) { delete mmu; }
        if (io)  { delete io; }
        if (ppu) { delete ppu; }
        if (interruptHandler) { delete interruptHandler; }
        if (joypad) { delete joypad; }
        if (timer)  { delete timer; }
        if (cartridge) { delete cartridge; }
    }
}