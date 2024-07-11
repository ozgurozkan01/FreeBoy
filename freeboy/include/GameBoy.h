//
// Created by ozgur on 10/04/2024.
//

#ifndef FREEBOY_GAMEBOY_H
#define FREEBOY_GAMEBOY_H

#include "SDL2/SDL.h"
#include "Renderer.h"
#include "PPU.h"
#include "LCD.h"
#include <cstdint>
#include <string>

namespace gameboy
{
    using namespace graphic;

    class InterruptHandler;
    class Cartridge;
    class Joypad;
    class Timer;
    class CPU;
    class MMU;
    class DMA;
    class IO;


    enum class EmulatorState : uint8_t
    {
        pause,
        running,
        quit
    };

    class GameBoy {
        friend class CPU;
        friend class Joypad;
        friend class MMU;
    public:
        GameBoy(std::string _romPath);
        ~GameBoy();
        bool init();
        void run();
    private:

        EmulatorState currentState;

        std::string romPath;

        InterruptHandler* interruptHandler;
        Cartridge* cartridge;
        Renderer* renderer;
        Joypad* joypad;
        Timer* timer;
        CPU* cpu;
        MMU* mmu;
        PPU* ppu;
        LCD* lcd;
        DMA* dma;
        IO* io;

        uint64_t ticks;
        void processEvent();
        void emulateCycles(uint8_t mCycle);
    };

}

#endif //FREEBOY_GAMEBOY_H
