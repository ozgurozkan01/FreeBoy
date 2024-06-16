//
// Created by ozgur on 6/15/2024.
//

#include "../include/IO.h"
#include <cstdio>

namespace gameboy
{
    uint8_t IO::read(const uint16_t _address)
    {
        if (_address == 0xFF00)
        {
            // JOYPAD INPUT
        }
        else if (_address < 0xFF03)
        {
            // Serial Transfer
        }
        else if (_address >= 0xFF04 && _address <= 0xFF07)
        {
            // Timer and Divider
        }
        else if (_address == 0xFF0F)
        {
            // Interrupts
        }
        else if (_address <= 0xFF26)
        {
            // AUDIO
        }
        else if (_address >= 0xFF30 && _address <= 0xFF3F)
        {
            // Wave Pattern
        }
        else if (_address >= 0xFF40 && _address <= 0xFF4B)
        {
            // LCD Control, Status, Position, Scrolling and Palettes
        }
        else if (_address == 0xFF50)
        {
            // Set to non-zero to disable boot ROM
        }
        return 0xFF;
    }

    void IO::write(const uint16_t _address, const uint8_t _value)
    {
        if (_address == 0xFF00)
        {
            // JOYPAD INPUT
        }
        else if (_address < 0xFF03)
        {
            // Serial Transfer
        }
        else if (_address >= 0xFF04 && _address <= 0xFF07)
        {
            // Timer and Divider
        }
        else if (_address == 0xFF0F)
        {
            // Interrupts
        }
        else if (_address <= 0xFF26)
        {
            // AUDIO
        }
        else if (_address >= 0xFF30 && _address <= 0xFF3F)
        {
            // Wave Pattern
        }
        else if (_address >= 0xFF40 && _address <= 0xFF4B)
        {
            // LCD Control, Status, Position, Scrolling and Palettes
        }
        else if (_address == 0xFF50)
        {
            // Set to non-zero to disable boot ROM
        }
    }
}