//
// Created by ozgur on 6/12/2024.
//

#include "../include/MMU.h"
#include "../include/GameBoy.h"
#include "../include/Cartridge.h"
#include "../include/InterruptHandler.h"
#include "../include/IO.h"
#include "../include/PPU.h"
#include "../include/DMA.h"
#include <cstdio>

namespace gameboy
{
    MMU::MMU(GameBoy* _gameBoy) :
        gameBoyPtr(_gameBoy)
    {}

    void MMU::write8(const uint16_t _address, const uint8_t _value)
    {
        if (_address < 0x8000)
        {
            gameBoyPtr->cartridge->write(_address, _value);
            return;
        }
        else if (_address < 0xA000)
        {
            gameBoyPtr->ppu->writeVRAM(_address, _value);
            return;
        }
        else if (_address < 0xC000)
        {
            gameBoyPtr->cartridge->write(_address, _value);
            return;
        }
        else if (_address < 0xE000)
        {
            workRAM[_address - 0xC000] = _value;
            return;
        }
        else if (_address < 0xFE00)
        {
            printf(" (W-Echo RAM) "); return;
        }
        else if (_address < 0xFEA0)
        {
            if (gameBoyPtr->dma->isTransferring()) { return; }
            gameBoyPtr->ppu->writeOAM(_address, _value);
            return;
        }
        else if (_address < 0xFF00)
        {
            printf(" (W-Not Usable Memory) "); return;
        }
        else if (_address < 0xFF80)
        {
            gameBoyPtr->io->write(_address, _value);
            return;
        }
        else if (_address < 0xFFFF)
        {
            highRAM[_address - 0xFF80] = _value;
            return;
        }
        else if (_address == 0xFFFF)
        {
            gameBoyPtr->interruptHandler->setIE(_value);
            return;
        }

        printf("\nUNSUPPORTED MEMORY WRITE %02X\n", _address);
        exit(-1);
    }
    uint8_t MMU::read8(const uint16_t _address)
    {
        if (_address < 0x8000)
        {
            return gameBoyPtr->cartridge->read(_address);
        }
        else if (_address < 0xA000)
        {
            return gameBoyPtr->ppu->readVRAM(_address);
        }
        else if (_address < 0xC000)
        {
            return gameBoyPtr->cartridge->read(_address);
        }
        else if (_address < 0xE000)
        {
            return workRAM[_address - 0xC000];
        }
        else if (_address < 0xFE00)
        {
            printf(" (R-Echo RAM) ");
            return 0x0;
        }
        else if (_address < 0xFEA0)
        {
            if (gameBoyPtr->dma->isTransferring()) { return 0xFF; }

            return gameBoyPtr->ppu->readOAM(_address);
        }
        else if (_address < 0xFF00)
        {
            printf(" (R-Not Usable Memory) ");
            return 0x0;
        }
        else if (_address < 0xFF80)
        {
            return gameBoyPtr->io->read(_address);
        }
        else if (_address < 0xFFFF)
        {
            return highRAM[_address - 0xFF80];
        }
        else if (_address == 0xFFFF)
        {
            return gameBoyPtr->interruptHandler->getIE().read();
        }

        printf("\nUNSUPPORTED MEMORY READ %02X\n", _address);
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

    void MMU::push(Register16& _sp, Register16& _srcRegister)
    {
        _sp--;
        write8(_sp.read(), (_srcRegister >> 8) & 0xFF);
        _sp--;
        write8(_sp.read(), _srcRegister & 0xFF);
    }

    uint16_t MMU::pop(Register16& _sp)
    {
        uint16_t popped = read16(_sp.read());
        _sp += 2;
        return popped;
    }
}
