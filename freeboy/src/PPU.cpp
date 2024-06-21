//
// Created by ozgur on 6/19/2024.
//

#include "../include/PPU.h"

namespace gameboy
{
    void PPU::writeVRAM(const uint16_t _address, const uint8_t _value)
    {
        videoRAM[_address - 0x8000] = _value;
    }

    uint8_t PPU::readVRAM(const uint16_t _address)
    {
        return videoRAM[_address - 0x8000];
    }
}