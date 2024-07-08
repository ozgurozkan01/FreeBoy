//
// Created by ozgur on 6/30/2024.
//

#ifndef FREEBOY_DMA_H
#define FREEBOY_DMA_H

#include <cstdint>
#include "PPU.h"

namespace gameboy
{
    class MMU;

    using namespace graphic;

    /*
     DMA transfer is used to quickly copy a block of data from a part of memory (ROM or RAM)
     to another part of memory, specifically to the OAM.
     This is important for updating the sprite attributes efficiently.
    */
    class DMA {
    public:
        DMA(PPU* _ppu, MMU* _mmu);

        void transferData(uint8_t _address);
        void tick();
        bool isTransferring();

        uint8_t getValue() const { return upperByte; };

    private:
        bool isActive;
        uint8_t lowerByte;
        uint8_t upperByte;

        PPU* ppuPtr;
        MMU* mmuPtr;
    };
}

#endif //FREEBOY_DMA_H
