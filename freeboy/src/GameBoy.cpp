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


        interruptHandler = new InterruptHandler();
        joypad = new Joypad();
        timer = new Timer(interruptHandler);
        ppu = new PPU(interruptHandler, mmu);
        dma = new DMA(ppu, mmu);
        lcd = new LCD(dma, interruptHandler, ppu);
        io = new IO(joypad, timer, interruptHandler,lcd);
        cpu = new CPU(this, mmu, interruptHandler);

        ppu->connectLCD(lcd);

        renderer = new Renderer(cartridge->getTitle(), mmu, ppu);
        if (!renderer->init())
        {
            printf("ERROR : SDL Window could not be created!\n");
            return false;
        }

        return true;
    }

    void GameBoy::run()
    {
        std::thread cpuThread(&CPU::run, cpu);

        uint32_t previousFrame;
        while (currentState != EmulatorState::quit)
        {
            processEvent();
            //printf("JOYPAD : %#02x\n", joypad->read());

            //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if (previousFrame != ppu->getRenderedFrame())
            {
                renderer->render();
            }

            previousFrame = ppu->getRenderedFrame();
        }

        if (cpuThread.joinable()) { cpuThread.join(); }
    }

    void GameBoy::processEvent()
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)  { joypad->handleJoypad(true, event.key.keysym.sym); }
            if (event.type == SDL_KEYUP)    { joypad->handleJoypad(false, event.key.keysym.sym); }
            if ((event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) || event.key.keysym.sym == SDLK_ESCAPE) { currentState = EmulatorState::quit; }
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
                ppu->tick();
            }
            dma->tick();
        }
    }

    GameBoy::~GameBoy()
    {
        delete cpu;
        delete io;
        delete dma;
        delete ppu;
        delete lcd;
        delete timer;
        delete joypad;
        delete interruptHandler;
        delete renderer;
        delete mmu;
        delete cartridge;
    }
}