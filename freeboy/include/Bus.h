//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_BUS_H
#define FREEBOY_BUS_H

#include <cstdint>

namespace gameboy
{
    class Cartridge;
    namespace ram
    {
        class WRAM;
    }

    namespace instruction
    {
        class InterruptHandler;
    }

    class Bus {
    public:
        Bus(Cartridge* _cartridge, instruction::InterruptHandler* _interruptHandler);
        ~Bus();

        void write8(uint16_t  address, uint8_t value);
        void write16(uint16_t  address, uint16_t value);
        uint8_t read8(uint16_t  address);
        uint16_t read16(uint16_t  address);

    private:
        Cartridge* cartridgeRef;
        instruction::InterruptHandler* interruptHandlerRef;
        ram::WRAM* wram;
    };
}

#endif //FREEBOY_BUS_H
