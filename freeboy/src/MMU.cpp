//
// Created by ozgur on 6/12/2024.
//

#include "../include/MMU.h"
#include "../include/Cartridge.h"
#include "../include/InterruptHandler.h"
#include "../include/IO.h"
#include "../include/PPU.h"
#include <cstdio>

namespace gameboy
{
    MMU::MMU(Cartridge *_cartridge, InterruptHandler* _interruptHandler, PPU* _ppu) :
            cartridgePtr(_cartridge),
            interruptHandlerPtr(_interruptHandler),
            ppuPtr(_ppu)
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

        if (ppuPtr == nullptr)
        {
            printf("ERROR : ppuPtr could not be initialized!");
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
            ppuPtr->writeVRAM(_address, _value);
            return;
        }
        else if (_address < 0xC000)
        {
            cartridgePtr->write(_address, _value);
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
            printf(" (W-IO) ");
            return;
        }
        else if (_address < 0xFFFF)
        {
            highRAM[_address - 0xFF80] = _value;
            return;
        }
        else if (_address == 0xFFFF)
        {
            interruptHandlerPtr->setIE(_value);
            return;
        }

        printf("\nUNSUPPORTED MEMORY ADDRESS %02X\n", _address);
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
            return ppuPtr->readVRAM(_address);
        }
        else if (_address < 0xC000)
        {
            return cartridgePtr->read(_address);
        }
        else if (_address < 0xE000)
        {
            return workRAM[_address - 0xC000];
        }
        else if (_address < 0xFE00)
        {
            return 0;
        }
        else if (_address < 0xFEA0)
        {
            return 0;
        }
        else if (_address < 0xFF00)
        {
            return 0;
        }
        else if (_address < 0xFF80)
        {
            return 0;
        }
        else if (_address < 0xFFFF)
        {
            return highRAM[_address - 0xFF80];
        }
        else if (_address == 0xFFFF)
        {
            return interruptHandlerPtr->getIE();
        }

        printf("\nUNSUPPORTED MEMORY ADDRESS %02X\n", _address);
        exit(-1);
    }

    void MMU::write16(const uint16_t _address, const uint16_t _value)
    {
        write8(_address, _value & 0xFF); // LSB
        write8(_address + 1, (_value >> 8) & 0xFF); // MSB
    }

    uint16_t MMU::read16(const uint16_t _address)

    {
        uint8_t low = read8(_address);
        uint8_t high = read8(_address + 1);

        return low | (high << 8);
    }

    void MMU::push(Register16& _sp, const Register16& _srcRegister)
    {
        _sp -= 2;
        write16(_sp.read(), _srcRegister.read());
    }

    uint16_t MMU::pop(Register16& _sp)
    {
        uint16_t popped = read16(_sp.read());
        _sp += 2;
        return popped;
    }
}
