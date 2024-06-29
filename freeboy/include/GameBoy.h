//
// Created by ozgur on 10/04/2024.
//

#ifndef FREEBOY_GAMEBOY_H
#define FREEBOY_GAMEBOY_H

#include "SDL2/SDL.h"
#include <cstdint>
#include <string>

namespace gameboy
{
    class InterruptHandler;
    class Cartridge;
    class Joypad;
    class Timer;
    class CPU;
    class PPU;
    class MMU;
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
        Joypad* joypad;
        Timer* timer;
        CPU* cpu;
        MMU* mmu;
        PPU* ppu;
        IO* io;

        uint64_t ticks;
        void processEvent();
        void emulateCycles(uint8_t mCycle);
    };

}

#endif //FREEBOY_GAMEBOY_H
