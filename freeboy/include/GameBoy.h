//
// Created by ozgur on 10/04/2024.
//

#ifndef FREEBOY_GAMEBOY_H
#define FREEBOY_GAMEBOY_H


#include <cstdint>

namespace gameboy
{
    class Graphic;
    class Cartridge;
    class CPU;

    enum class EmulatorState : uint8_t
    {
        PAUSE,
        RUNNING,
        QUIT
    };

    class GameBoy {
        friend class CPU;
    public:
        GameBoy();
        ~GameBoy();
        bool init();
        void run();

        Cartridge* getCartridge() const;
    private:
        EmulatorState emulatorState;

        Graphic* graphic;
        Cartridge* cartridge;
        CPU* cpu;

        uint64_t ticks;
        void processEvent();
        void emulateCycles(uint8_t cycleCount);
    };

}

#endif //FREEBOY_GAMEBOY_H
