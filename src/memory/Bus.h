//
// Created by ozgur on 27/03/2024.
//

#ifndef GAMEBOY_EMULATOR_SDL2_BUS_H
#define GAMEBOY_EMULATOR_SDL2_BUS_H

#include <cstdint>

class Bus {
public:
    Bus();
    uint8_t busRead(uint16_t address);
    void busWrite(uint16_t address, uint8_t value);
private:
};


#endif //GAMEBOY_EMULATOR_SDL2_BUS_H
