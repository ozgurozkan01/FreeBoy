//
// Created by ozgur on 5/31/2024.
//

#include <cstdio>
#include <cstdlib>
#include "../include/RAM.h"

namespace gameboy::ram
{
    uint8_t WRAM::read(uint16_t address)
    {
        address -= MEMORY_OFFSET;
        if (address >= MEMORY_SIZE)
        {
            printf("INVALID WRAM ADDR %08X\n", address + 0xC000);
            exit(-1);
        }
        return memory[address];
    }
    void WRAM::write(uint16_t address, uint8_t value)
    {
        address -= MEMORY_OFFSET;
        if (address >= MEMORY_SIZE)
        {
            printf("INVALID WRAM ADDR %08X\n", address + 0xC000);
            exit(-1);
        }
        memory[address] = value;
    }

    uint8_t HRAM::read(uint16_t address)
    {
        address -= MEMORY_OFFSET;
        if (address >= MEMORY_SIZE) {
            printf("INVALID WRAM ADDR %08X\n", address + MEMORY_OFFSET);
            exit(-1);
        }
        return memory[address];
    }
    void HRAM::write(uint16_t address, uint8_t value)
    {
        address -= MEMORY_OFFSET;
        if (address >= MEMORY_SIZE) {
            printf("INVALID WRAM ADDR %08X\n", address + MEMORY_OFFSET);
            exit(-1);
        }
        memory[address] = value;
    }

    uint8_t VRAM::read(uint16_t address) { return memory[address - 0x8000];; }
    void VRAM::write(uint16_t address, uint8_t value) { memory[address - 0x8000] = value; }
}