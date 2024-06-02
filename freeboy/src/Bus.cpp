//
// Created by ozgur on 12/04/2024.
//

#include "../include/Bus.h"
#include "../include/Cartridge.h"
#include "../include/InterruptHandler.h"
#include "../include/RAM.h"

/*
 * 0x0000 - 0x3FFF : ROM Bank 0 (ROM DATA)
 * 0x4000 - 0x7FFF : ROM Bank 1 - Switchable (ROM DATA)
 * ------------------------------------------
 * 0x8000 - 0x97FF : CHR RAM (VRAM)
 * 0x9800 - 0x9BFF : BG Map 1 (VRAM)
 * 0x9C00 - 0x9FFF : BG Map 2 (VRAM)
 * ------------------------------------------
 * 0xA000 - 0xBFFF : Cartridge RAM (Cartridge External)
 * ------------------------------------------
 * 0xC000 - 0xCFFF : RAM Bank 0 (WRAM)
 * 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only (WRAM)
 * ------------------------------------------
 * 0xE000 - 0xFDFF : Reserved - Echo RAM
 * ------------------------------------------
 * 0xFE00 - 0xFE9F : Object Attribute Memory
 * ------------------------------------------
 * 0xFEA0 - 0xFEFF : Reserved - Unusable
 * ------------------------------------------
 * 0xFF00 - 0xFF7F : I/O Registers
 * ------------------------------------------
 * 0xFF80 - 0xFFFE : Zero Page (HRAM)
 * ------------------------------------------
 * 0xFFFF - 0xFFFF : Enable Interrupt Register
 */

namespace gameboy
{
    Bus::Bus(Cartridge* _cartridge, InterruptHandler* _interruptHandler) :
    cartridgeRef(_cartridge),
    interruptHandlerRef(_interruptHandler),
    wram(new ram::WRAM)
    {}

    Bus::~Bus() {}

    void Bus::write8(uint16_t address, uint8_t value)
    {
        if (address < 0x8000) // ROM DATA
        {
            cartridgeRef->write(address, value);
            return;
        }
        else if (address < 0xA000) // VRAM (Video RAM)
        {
            return;
        }
        else if (address < 0xC000) // Cartridge External RAM
        {
            return;
        }
        else if (address < 0xE000) // WRAM
        {
            return;
        }
        else if (address < 0xFE00) // ECHO RAM
        {
            return;
        }
        else if(address < 0xFEA0) // Object Attribute Memory (OAM)
        {
            return;
        }
        else if(address < 0xFF00) // NOT USABLE
        {
            return;
        }
        else if(address < 0xFF80) // I/O Registers
        {
            return;
        }
        else if(address < 0xFFFF) // HRAM (High RAM)
        {
            return;
        }
        else if(address == 0xFFFF) // Enable Interrupt Register
        {
            interruptHandlerRef->setIE(value);
            return;
        }

        printf("UNSUPPORTED bus_write(%04X)\n", address);
        exit(-1);
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
        else if (address < 0xA000) // VRAM (Video RAM)
        {
            return -1;
        }
        else if (address < 0xC000) // Cartridge External RAM
        {
            return cartridgeRef->read(address);
        }
        else if (address < 0xE000) // WRAM (Working RAM)
        {
            return wram->read(address);
        }
        else if (address < 0xFE00) // ECHO RAM
        {
            return -1;
        }
        else if(address < 0xFEA0) // Object Attribute Memory (OAM)
        {
            return -1;
        }
        else if(address < 0xFF00) // NOT USABLE
        {
            return -1;
        }
        else if(address < 0xFF80) // I/O Registers
        {
            return -1;
        }
        else if(address < 0xFFFF) // HRAM (High RAM)
        {
            return -1;
        }
        else if(address == 0xFFFF) // Enable Interrupt Register
        {
            return interruptHandlerRef->getIE();
        }

        printf("UNSUPPORTED bus_read(%04X)\n", address);
        exit(-1);
    }

    uint16_t Bus::read16(uint16_t address)
    {
        uint16_t low = read8(address);
        uint16_t high = read8(address + 1);

        return low | (high << 8);
    }
}