//
// Created by ozgur on 12/04/2024.
//

#include "../include/Bus.h"
#include "../include/Cartridge.h"

namespace gameboy
{
    Bus::Bus(Cartridge* _cartridge) : cartridgeRef(_cartridge) {}
    Bus::~Bus() {}

    void Bus::write8(uint16_t address, uint8_t value)
    {
        if (address < 0x8000)
        {
            cartridgeRef->write(address, value);
            return;
        }

        printf("Address does not avaible to write !\n");
    }

    void Bus::write16(uint16_t address, uint16_t value)
    {
        write8(address + 1, (value >> 8) & 0xFF);
        write8(address, value & 0xFF);
    }

    uint8_t Bus::read8(uint16_t address)
    {
        if (address < 0x8000)
        {
            return cartridgeRef->read(address);
        }

        printf("Address does not avaible to read !\n");
        return -1;
    }

    uint16_t Bus::read16(uint16_t address)
    {
        uint16_t low = read8(address);
        uint16_t high = read8(address + 1);

        return low | (high << 8);
    }
}