//
// Created by ozgur on 6/12/2024.
//

#include "../include/MMU.h"
#include "../include/Cartridge.h"
#include "../include/InterruptHandler.h"
#include <cstdio>

namespace gameboy
{
    MMU::MMU(Cartridge *_cartridge, InterruptHandler* _interruptHandler) :
            cartridgePtr(_cartridge),
            interruptHandlerPtr(_interruptHandler)
    {}

    bool MMU::init()
    {
        if (cartridgePtr == nullptr)
        {
            printf("ERROR : cartridgePtr could not be initialized!");
            return false;
        }

        if (interruptHandlerPtr == nullptr)
        {
            printf("ERROR : interruptHandlerPtr could not be initialized!");
            return false;
        }
        return true;
    }

    void MMU::write8(const uint16_t _address, const uint8_t _value)
    {
        if (_address < 0x8000)
        {
            cartridgePtr->write(_address, _value);
            return;
        }

        else if (_address < 0xA000)
        {
            printf(" (W-VRAM) ");
        }
        else if (_address < 0xC000)
        {
            printf(" (W-Cartridge External) ");
        }
        else if (_address < 0xE000)
        {
            workRAM[_address - 0xC000] = _value;
            return;
        }
        else if (_address < 0xFE00)
        {
            printf(" (W-Echo RAM) ");
        }
        else if (_address < 0xFEA0)
        {
            printf(" (W-OAM RAM) ");
        }
        else if (_address < 0xFF00)
        {
            printf(" (W-Not Usable Memory) ");
        }
        else if (_address < 0xFF80)
        {
            printf(" (W-I/O Registers) ");
        }
        else if (_address < 0xFFFF)
        {
            highRAM[_address - 0x8000] = _value;
            return;
        }
        else if (_address == 0xFFFF)
        {
            interruptHandlerPtr->setIE(_value);
            return;
        }

        exit(-1);
    }
    uint8_t MMU::read8(const uint16_t _address)
    {
        if (_address < 0x8000)
        {
            return cartridgePtr->read(_address);
        }
        else if (_address < 0xA000)
        {
            printf(" (R-VRAM) ");
        }
        else if (_address < 0xC000)
        {
            printf(" (R-Cartridge External) ");
        }
        else if (_address < 0xE000)
        {
            return workRAM[_address - 0xC000];
        }
        else if (_address < 0xFE00)
        {
            printf(" (R-Echo RAM) ");
        }
        else if (_address < 0xFEA0)
        {
            printf(" (R-OAM RAM) ");
        }
        else if (_address < 0xFF00)
        {
            printf(" (R-Not Usable Memory) ");
        }
        else if (_address < 0xFF80)
        {
            printf(" (R-I/O Registers) ");
        }
        else if (_address < 0xFFFF)
        {
            return highRAM[_address - 0x8000];
        }
        else if (_address == 0xFFFF)
        {
            return interruptHandlerPtr->getIE();
        }

        exit(-1);
    }

    void MMU::write16(const uint16_t _address, const uint16_t _value)
    {
        write8(_address + 1, (_value >> 8) & 0xFF);
        write8(_address, _value & 0xFF);
    }

    uint16_t MMU::read16(const uint16_t _address)

    {
        uint8_t low = cartridgePtr->read(_address);
        uint8_t high = cartridgePtr->read(_address + 1);

        uint16_t n16 = low | (high << 8) ;

        return n16;
    }
}
