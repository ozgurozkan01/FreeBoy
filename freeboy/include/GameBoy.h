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

    enum class EmulatorState : uint8_t
    {
        PAUSE,
        RUNNING,
        QUIT
    };

    class GameBoy {
    public:
        GameBoy();
        ~GameBoy();
        bool init();
        void run();
    private:
        EmulatorState emulatorState;

        Graphic* graphic;
        Cartridge* cartridge;

        uint64_t ticks;
        void processEvent();
    };

}

#endif //FREEBOY_GAMEBOY_H
