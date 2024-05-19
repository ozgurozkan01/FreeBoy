//
// Created by ozgur on 12/04/2024.
//

#include "../include/Bus.h"
#include "../include/Cartridge.h"

namespace gameboy
{
    Bus::Bus(Cartridge* _cartridge) : cartridgeRef(_cartridge) {}
    Bus::~Bus() {}

    void Bus::busWrite(const uint16_t address, const uint8_t value)
    {
        if (address < 0x8000)
        {
            cartridgeRef->write(address, value);
            return;
        }

        printf("Address does not avaible to write !\n");
        exit(-1);
    }

    uint8_t Bus::busRead(uint16_t address)
    {
        if (address < 0x8000)
        {
            return cartridgeRef->read(address);
        }

        printf("Address does not avaible to read !\n");
        exit(-1);
    }
}