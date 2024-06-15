//
// Created by ozgur on 6/12/2024.
//

#ifndef FREEBOY_MMU_H
#define FREEBOY_MMU_H

#include <cstdint>
#include <array>

namespace gameboy
{
    class Cartridge;
    class InterruptHandler;

    class MMU{
    public:
        MMU(Cartridge* _cartridge, InterruptHandler* _interruptHandler);

        bool init();

        void write8(const uint16_t _address, const uint8_t _value);
        uint8_t read8(const uint16_t _address);

        void write16(const uint16_t _address, const uint16_t _value);
        uint16_t read16(const uint16_t _address);

    private:
        Cartridge* cartridgePtr;
        InterruptHandler* interruptHandlerPtr;

        /* RAM */
        std::array<uint8_t, 0x2000> workRAM;
        // std::array<uint8_t, 0x2000> videoRAM;
        std::array<uint8_t, 0x80> highRAM;
        // std::array<uint8_t, 0xA0> oamRAM;
    };
}


#endif //FREEBOY_MMU_H
