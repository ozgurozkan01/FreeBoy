//
// Created by ozgur on 6/19/2024.
//

#include "../include/PPU.h"

namespace gameboy::graphic
{
    void PPU::writeVRAM(const uint16_t _address, const uint8_t _value)
    {
        videoRAM[_address - 0x8000] = _value;
    }

    uint8_t PPU::readVRAM(const uint16_t _address)
    {
        return videoRAM[_address - 0x8000];
    }

    void PPU::writeOAM(const uint16_t _address, const uint8_t _value)
    {
        auto* p = reinterpret_cast<uint8_t*>(&oamRAM);
        p[_address - 0xFE00] = _value;
    }

    uint8_t PPU::readOAM(const uint16_t _address)
    {
        auto* p = reinterpret_cast<uint8_t*>(&oamRAM);
        return p[_address - 0xFE00];
    }
}