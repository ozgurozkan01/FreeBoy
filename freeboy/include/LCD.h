//
// Created by ozgur on 7/1/2024.
//

#ifndef FREEBOY_LCD_H
#define FREEBOY_LCD_H

#include "Registers.h"
#include <array>

namespace gameboy
{
    class DMA;
    class InterruptHandler;

    using namespace cpu_register;

    namespace graphic
    {
        class PPU;

        class LCD {

            friend class PPU;

            enum class LCDCFlag : uint8_t
            {
                bgwEnable         = 0x01, // (0=Disabled, 1=Enabled)
                spriteEnable      = 0x02, // (0=Disabled, 1=Enabled)
                spriteSize        = 0x04, // (0=8×8, 1=8×16)
                bgTileMap         = 0x08, // (0=9800h-9BFFh, 1=9C00h-9FFFh)
                bgwTileData        = 0x10, // (0=8800h-97FFh, 1=8000h-8FFFh)
                windowEnable      = 0x20, // (0=Disabled, 1=Enabled)
                windowTileMap     = 0x40, // (0=9800h-9BFFh, 1=9C00h-9FFFh)
                lcdDisplayEnable  = 0x80  // (0=Off, 1=On)
            };

            enum class Status : uint8_t
            {
                // 2 LSB stand for PPU mode, it handled in PPU.h
                coincidenceFlag  = 0x04, // 0000 0100 -> LYC == LY
                hBlankEnable     = 0x08, // 0000 1000
                vBlankEnable     = 0x10, // 0001 0000
                oamEnable        = 0x20, // 0010 0000
                lycEnable        = 0x40  // 0100 0000 -> LYC = LY
                // Last bit is unusable
            };

        public:

            enum class Color : uint32_t
            {
                white     = 0xFFFFFFFF,
                lightGray = 0xFFAAAAAA,
                darkGray  = 0xFF555555,
                black     = 0xFF000000
            };

            LCD(DMA* _dma, InterruptHandler* _interruptHandler, PPU* _ppu);

            uint8_t read(const uint16_t _address) const;
            void write(uint16_t _address, uint8_t _value);

            uint16_t readLCDC(LCDCFlag _flag) const;

        private:

            void incrementLY();
            void setPaletteColors(std::array<Color, 4>& _palette, const uint8_t data);

            bool isInterruptEnabled(Status _status) { return lcds.read() & static_cast<uint8_t>(_status); }
            bool isWindowVisible();

            // Each of the 8 bits in this register is a flag which determines which elements are displayed and more.
            Register8 lcdc;             // LCD Control Register - 0xFF40
            // The STAT register contains both information-bits which allow the CPU to determine the status of the PPU,
            // as well as bits which affect the interrupt trigger behavior of the PPU.
            Register8 lcds;             // LCD Sttatus Register - 0xFF41

            Register8 scy;              // Scrolling Y is used to arrange background layer  - Background viewport Y - 0xFF42
            Register8 scx;              // Scrolling X is used to arrange background layer  - Background viewport X - 0xFF43

            Register8 ly;               // LCD Y coordinate - 0xFF44
            Register8 lyc;              // LCD Y coordinate Compare - 0xFF45

            Register8 bgPalette;        // Background Palette - 0xFF47
            Register8 spritePalette0;   // Object Palette 0 - 0xFF48
            Register8 spritePalette1;   // Object Palette 1 - 0xFF49

            Register8 windowY;          // it is used to arrange window layer - 0xFF4A
            Register8 windowX;          // it is used to arrange window layer - 0xFF4B

            std::array<Color, 4> defaultColorSet;

            /*
                Background has 4 color
                7-6 -> ID 3
                5-4 -> ID 2
                3-2 -> ID 1
                1-0 -> ID 0
             */
            std::array<Color, 4> bgColors;
            /*
                Sprites have 3 color, because there is a transparent instead of white
                7-6 -> ID 3
                5-4 -> ID 2
                3-2 -> ID 1
                1-0 -> transparent
             */
            std::array<Color, 4> spriteColor0;
            std::array<Color, 4> spriteColor1;

            InterruptHandler* interruptHandlerPtr;
            DMA* dmaPtr;
            PPU* ppuPtr;
        };
    }
}

#endif //FREEBOY_LCD_H
