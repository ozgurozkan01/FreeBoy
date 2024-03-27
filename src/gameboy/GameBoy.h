//
// Created by ozgur on 27/03/2024.
//

#ifndef GAMEBOY_EMULATOR_SDL2_GAMEBOY_H
#define GAMEBOY_EMULATOR_SDL2_GAMEBOY_H

#include <cstdint>

enum class EmulatorState : uint8_t
{
    PAUSE,
    RUNNING
};

class Graphic;

class GameBoy {
public:
    GameBoy();
    bool init();
    void run();
private:
    EmulatorState emulatorState;

    Graphic* graphic;
};


#endif //GAMEBOY_EMULATOR_SDL2_GAMEBOY_H
