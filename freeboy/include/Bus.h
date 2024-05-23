//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_BUS_H
#define FREEBOY_BUS_H

#include <cstdint>

namespace gameboy
{
    class Cartridge;

    class Bus {
    public:
        Bus(Cartridge* _cartridge);
        ~Bus();

        void write(uint16_t  address, uint8_t value);
        uint8_t read(uint16_t  address);
    private:
        Cartridge* cartridgeRef;
    };
}

#endif //FREEBOY_BUS_H
