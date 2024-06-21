//
// Created by ozgur on 6/19/2024.
//

#ifndef FREEBOY_PPU_H
#define FREEBOY_PPU_H

#include <cstdint>
#include <array>

namespace gameboy
{
    class PPU {
    public:
        void writeVRAM(const uint16_t _address, const uint8_t _value);
        uint8_t readVRAM(const uint16_t _address);
    private:
        std::array<uint8_t, 0x2000> videoRAM = {0};
    };
}

#endif //FREEBOY_PPU_H
