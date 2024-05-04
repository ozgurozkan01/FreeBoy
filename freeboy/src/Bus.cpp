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
        if (isMemoryAddressExist(address))
        {
            cartridgeRef->writeCartridge(address, value);
            return;
        }

        printf("Address does not avaible to write !\n");
        exit(-1);
    }

    uint8_t Bus::busRead(uint16_t address)
    {
        if (isMemoryAddressExist(address))
        {
            return cartridgeRef->readCartridge(address);
        }

        printf("Address does not avaible to read !\n");
        exit(-1);
    }

    bool Bus::isMemoryAddressExist(uint16_t address)
    {
        // 0x7FFF is the last address on the memory
        return address < 0x2000;
    }
}