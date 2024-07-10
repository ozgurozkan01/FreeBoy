//
// Created by ozgur on 6/30/2024.
//

#include "../include/DMA.h"
#include "../include/MMU.h"
#include <chrono>
#include <thread>
#include <cstdio>

namespace gameboy
{
    DMA::DMA(PPU *_ppu, MMU *_mmu) :
        ppuPtr(_ppu),
        mmuPtr(_mmu),
        isActive(false),
        lowerByte(0),
        upperByte(0)
    {}

    void DMA::transferData(uint8_t _address)
    {
        isActive = true;
        lowerByte = 0;
        upperByte = _address;
    }

    void DMA::tick()
    {
        if (!isTransferring()) { return; }

        // Source:      $XX00-$XX9F
        // Destination: $FE00-$FE9F
        uint16_t address = 0xFE00 | lowerByte;
        uint8_t data = mmuPtr->read8((upperByte * 0x100) + lowerByte);
        ppuPtr->writeOAM(address, data);

        // If lowerByte is bigger than 0x9F, that means invalid memory address for transfering
        isActive = ++lowerByte < 0xA0;
    }

    bool DMA::isTransferring() { return isActive; }
}