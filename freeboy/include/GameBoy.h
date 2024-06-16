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
    class CPU;
    class MMU;

    enum class EmulatorState : uint8_t
    {
        PAUSE,
        RUNNING,
        QUIT
    };

    class GameBoy {
        friend class CPU;
    public:
        GameBoy(std::string _romPath);
        ~GameBoy();
        bool init();
        void run();
    private:
        std::string romPath;

        EmulatorState emulatorState;

        InterruptHandler* interruptHandler;
        Cartridge* cartridge;
        CPU* cpu;
        MMU* mmu;

        uint64_t ticks;
        void processEvent();
        void emulateCycles(uint8_t cycleCount);
    };

}

#endif //FREEBOY_GAMEBOY_H
