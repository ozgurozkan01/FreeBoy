//
// Created by ozgur on 7/1/2024.
//

#include "../include/LCD.h"
#include "../include/DMA.h"
#include "../include/InterruptHandler.h"
#include "../include/Renderer.h"
#include <cstdlib>
#include <cstdio>

namespace gameboy::graphic
{
    LCD::LCD(DMA* _dma, InterruptHandler* _interruptHandler, PPU* _ppu) :
            dmaPtr(_dma),
            interruptHandlerPtr(_interruptHandler),
            ppuPtr(_ppu),
            lcdc(0x91),
            lcds(0x0),
            scx(0),
            scy(0),
            ly(0),
            lyc(0),
            bgPalette(0xFC),
            spritePalette0(0xFF),
            spritePalette1(0xFF),
            windowX(0),
            windowY(0)
    {
        defaultColorSet = { Color::white, Color::lightGray, Color::darkGray, Color::black};
        bgColors        = { Color::white, Color::lightGray, Color::darkGray, Color::black};
        spriteColor0    = { Color::white, Color::lightGray, Color::darkGray, Color::black};
        spriteColor1    = { Color::white, Color::lightGray, Color::darkGray, Color::black};
    }

    uint8_t LCD::read(const uint16_t _address) const
    {
        switch (_address)
        {
            case 0xFF40: return lcdc.read();
            case 0xFF41: return lcds.read();
            case 0xFF42: return scy.read();
            case 0xFF43: return scx.read();
            case 0xFF44: return ly.read();
            case 0xFF45: return lyc.read();
            case 0xFF46: return dmaPtr->getValue();
            case 0xFF47: return bgPalette.read();
            case 0xFF48: return spritePalette0.read();
            case 0xFF49: return spritePalette1.read();
            case 0xFF4A: return windowY.read();
            case 0xFF4B: return windowX.read();
        }

        return 0xFF;
    }

    void LCD::write(uint16_t _address, uint8_t _value)
    {
        switch (_address)
        {
            case 0xFF40: lcdc = _value;
                printf("%#02x\n", lcdc.read());
                break;
            case 0xFF41: lcds = _value; break;
            case 0xFF42: scy = _value; break;
            case 0xFF43: scx = _value; break;
            case 0xFF44:               break;
            case 0xFF45: lyc = _value; break;
            case 0xFF46: dmaPtr->transferData(_value); break;
            case 0xFF47: bgPalette = _value; setPaletteColors(bgColors, _value); break;
            case 0xFF48: spritePalette0 = _value; setPaletteColors(spriteColor0, _value & 0b11111100); break;
            case 0xFF49: spritePalette1 = _value; setPaletteColors(spriteColor1, _value & 0b11111100); break;
            case 0xFF4A: windowY = _value; break;
            case 0xFF4B: windowX = _value; break;
        }
    }

    void LCD::setPaletteColors(std::array<Color, 4> &_palette, const uint8_t data)
    {
        for (int i = 0; i < 4; ++i)
        {
            uint8_t colorIndex = (data >> (i * 2)) & 0b11;
            _palette[i] = defaultColorSet[colorIndex];
        }
    }

    void LCD::incrementLY()
    {
        if (isWindowVisible() &&
        ly >= windowY &&
        ly < windowY + Renderer::yResolution)
        {
            ppuPtr->visibleWindowCounter++;
        }

        ly++;

        if (ly.read() == lyc.read())
        {
            lcds |= (static_cast<uint8_t>(Status::coincidenceFlag));

            if (isInterruptEnabled(Status::lycEnable))
            {
                interruptHandlerPtr->requestInterrupt(InterruptType::lcd);
            }
        }
        else
        {
            lcds &= ~(static_cast<uint8_t>(Status::coincidenceFlag));
        }
    }

    uint16_t LCD::readLCDC(LCD::LCDCFlag _flag) const
    {
        switch (_flag)
        {
            case LCDCFlag::bgwEnable:
            case LCDCFlag::spriteEnable:
            case LCDCFlag::windowEnable:
            case LCDCFlag::lcdDisplayEnable:
                return (lcdc.read() & static_cast<uint8_t>(_flag)) ? 1 : 0;
            case LCDCFlag::spriteSize:
                return (lcdc.read() & static_cast<uint8_t>(_flag)) ? 16 : 8;
            case LCDCFlag::bgTileMap:
            case LCDCFlag::windowTileMap:
                return (lcdc.read() & static_cast<uint8_t>(_flag)) ? 0x9C00 : 0x9800;
            case LCDCFlag::bgwTileData:
                return (lcdc.read() & static_cast<uint8_t>(_flag)) ? 0x8000 : 0x8800;
            default:
                printf("INVALID LCDC FLAG ACCESS\n");
                exit(-1);
        }
    }

    bool LCD::isWindowVisible() {
        return readLCDC(LCDCFlag::windowEnable) &&
        (windowX >= 0 && windowX < Renderer::xResolution + 7) &&
        (windowY >= 0 && windowY < Renderer::yResolution);
    }
}