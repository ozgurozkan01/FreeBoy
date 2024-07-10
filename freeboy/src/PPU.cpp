//
// Created by ozgur on 6/19/2024.
//

#include "../include/PPU.h"
#include "../include/MMU.h"
#include "../include/LCD.h"
#include "../include/Renderer.h"
#include "../include/InterruptHandler.h"
#include <algorithm>

namespace gameboy::graphic
{

    PPU::PPU(InterruptHandler* _interruptHandler, MMU* _mmu) :
            // FPS
            targetFPS(60),
            targetDeltaTime(1000 / targetFPS),
            calculationStartTime(0),
            previousFrame(0),
            frameCount(0),
            renderedFrame(0),
            frameDuration(0),
            // Window
            visibleWindowCounter(0),
            // Informations of One Frame
            visibleScanlinePerFrame(144),
            scanlinePerFrame(154),
            scanlineTCycle(0),
            fifoCapacity(8),
            maxSpritePerLine(10),
            maxSpriteAmount(40),
            // RAM
            videoRAM{0},
            oamRAM{OAM()},
            videoBuffer{0},
            // Components
            interruptHandlerPtr(_interruptHandler),
            mmuPtr(_mmu)
            {
        // Pixel FIFO
        pixelFifo.scanlineX = 0;
        pixelFifo.pushedX = 0;
        pixelFifo.fetcherX = 0;
        pixelFifo.currentState = FetcherState::getTile;
    }

    void PPU::writeVRAM(const uint16_t _address, const uint8_t _value)
    {
        videoRAM[_address - 0x8000] = _value;
    }

    uint8_t PPU::readVRAM(const uint16_t _address)
    {
        return videoRAM[_address - 0x8000];
    }

    void PPU::writeOAM(const uint16_t _address, const uint8_t _value)
    {
        auto* p = reinterpret_cast<uint8_t*>(&oamRAM);
        p[_address - 0xFE00] = _value;
    }

    uint8_t PPU::readOAM(const uint16_t _address)
    {
        auto* p = reinterpret_cast<uint8_t*>(&oamRAM);
        return p[_address - 0xFE00];
    }

    void PPU::runHBlank()
    {
        if (scanlineTCycle >= ModeCycles::hBlankCycle)
        {
            lcdPtr->incrementLY();

            if (lcdPtr->ly.read() < visibleScanlinePerFrame)
            {
                setPPUMode(PPUMode::oamScan);
            }
            else
            {
                setPPUMode(PPUMode::vBlank);
                interruptHandlerPtr->requestInterrupt(InterruptType::vblank);

                if (lcdPtr->isInterruptEnabled(LCD::Status::vBlankEnable))
                {
                    interruptHandlerPtr->requestInterrupt(InterruptType::lcd);
                }

                calculateFPS();
            }

            scanlineTCycle = 0;
        }
    }

    void PPU::runVBlank()
    {
        if (scanlineTCycle >= ModeCycles::vBlankCycle)
        {
            lcdPtr->incrementLY();

            if (lcdPtr->ly.read() >= scanlinePerFrame)
            {
                setPPUMode(PPUMode::oamScan);
                lcdPtr->ly = 0;
                visibleWindowCounter = 0;
            }

            scanlineTCycle = 0;
        }
    }

    void PPU::runOAM()
    {
        if (scanlineTCycle >= ModeCycles::oamCycle)
        {
            setPPUMode(PPUMode::transferData);

            resetPixelFIFO();
        }

        updateVisibleSprites();
    }

    void PPU::runTransferData()
    {
        pipelineProcess();

        if (pixelFifo.pushedX >= Renderer::xResolution)
        {
            pipelineResetFIFO();
            setPPUMode(PPUMode::hBlank);
            if (lcdPtr->isInterruptEnabled(LCD::Status::hBlankEnable))
            {
                interruptHandlerPtr->requestInterrupt(InterruptType::lcd);
            }
        }
    }

    bool PPU::isFifoFull()
    {
        return pixelFifo.fifo.size() >= fifoCapacity;
    }

    void PPU::resetPixelFIFO()
    {
        pixelFifo.currentState = FetcherState::getTile;
        pixelFifo.scanlineX = 0;
        pixelFifo.fetcherX = 0;
        pixelFifo.pushedX = 0;
        pixelFifo.fifoX = 0;
    }

    void PPU::updateVisibleSprites()
    {
        if (scanlineTCycle != 1) { return; }

        lineSprites.clear();

        uint16_t currentLine = lcdPtr->ly.read();
        uint8_t spriteHeight = lcdPtr->readLCDC(LCD::LCDCFlag::spriteSize);

        lineSprites.clear();

        for (int spriteNumber = 0; spriteNumber < maxSpriteAmount; ++spriteNumber)
        {
            if (!oamRAM[spriteNumber].x) { continue; }
            if (lineSprites.size() >= maxSpritePerLine) { break; }

            //printf("If VALUE : %d\n", oamRAM[spriteNumber].y <= currentLine + 16 && oamRAM[spriteNumber].y + spriteHeight > currentLine + 16);

            if (oamRAM[spriteNumber].y <= currentLine + 16 && oamRAM[spriteNumber].y + spriteHeight > currentLine + 16)
            {
                auto it = std::upper_bound(
                        lineSprites.begin(),
                        lineSprites.end(),
                        oamRAM[spriteNumber],
                        [](const OAM& a, const OAM& b) { return a.x < b.x; }
                );

                lineSprites.insert(it, oamRAM[spriteNumber]);
            }
        }
    }

    void PPU::tick()
    {
        scanlineTCycle++;

        switch (getPPUMode())
        {
            case PPUMode::hBlank: runHBlank(); break;
            case PPUMode::vBlank: runVBlank(); break;
            case PPUMode::oamScan: runOAM(); break;
            case PPUMode::transferData: runTransferData(); break;
        }
    }


    PPU::PPUMode PPU::getPPUMode()
    {
        return static_cast<PPUMode>(lcdPtr->lcds.read() & 0b11);
    }

    void PPU::setPPUMode(const PPU::PPUMode _mode)
    {
        lcdPtr->lcds &= ~(0b11); // Reset first 2 bit, if we did not reset, setting might be wrong.
        lcdPtr->lcds |= static_cast<uint8_t>(_mode); // Set first 2 bit
    }

    void PPU::calculateFPS()
    {
        renderedFrame++;

        uint32_t currentFrame = SDL_GetTicks();
        frameDuration = currentFrame - previousFrame;

        if (frameDuration < targetDeltaTime)
        {
            SDL_Delay(targetDeltaTime - frameDuration);
        }

        if (currentFrame - calculationStartTime >= 1000)
        {
            uint32_t fps = frameCount;
            calculationStartTime = currentFrame;
            frameCount = 0;
            printf("FPS: %d\n", fps);
        }

        frameCount++;
        previousFrame = SDL_GetTicks();
    }

    void PPU::pipelineProcess()
    {
        pixelFifo.mapY = lcdPtr->ly.read() + lcdPtr->scy.read();
        pixelFifo.mapX = pixelFifo.fetcherX + lcdPtr->scx.read();
        pixelFifo.tileY = ((lcdPtr->ly.read() + lcdPtr->scy.read()) % 8) * 2;

        if (!(scanlineTCycle & 1))
        {
            pipelineFetch();
        }

        pipelinePushPixel();
    }

    void PPU::pipelinePushPixel()
    {
        if (isFifoFull())
        {
            uint32_t pixelData = pixelFifo.fifo.front();
            pixelFifo.fifo.pop();
            if (pixelFifo.scanlineX >= (lcdPtr->scx.read() % 8))
            {
                videoBuffer[pixelFifo.pushedX + (lcdPtr->ly.read() * Renderer::xResolution)] = pixelData;
                pixelFifo.pushedX++;
            }
            pixelFifo.scanlineX++;
        }
    }

    void PPU::pipelineFetch()
    {
        switch (pixelFifo.currentState)
        {
            case FetcherState::getTile:     runGetTileState(); break;
            case FetcherState::getDataLow:  runGetDataLowState(); break;
            case FetcherState::getDataHigh: runGetDataHightState(); break;
            case FetcherState::sleep:       runSleepState(); break;
            case FetcherState::pushPixel:   runPushPixelState(); break;
        }
    }

    void PPU::runGetTileState()
    {
        fetchedLineSprites.clear();

        if (lcdPtr->readLCDC(LCD::LCDCFlag::bgwEnable))
        {
            pixelFifo.bgwFetchData[0] = mmuPtr->read8(
                    lcdPtr->readLCDC(LCD::LCDCFlag::bgTileMap) +
                    ((pixelFifo.mapY / 8) * 32) +
                    (pixelFifo.mapX / 8));


            if (lcdPtr->readLCDC(LCD::LCDCFlag::bgwTileData) == 0x8800)
            {
                pixelFifo.bgwFetchData[0] += 128;
            }

            loadWindowTile();
        }

        if (lcdPtr->readLCDC(LCD::LCDCFlag::spriteEnable))
        {
            fetchSpriteTiles();
        }

        pixelFifo.currentState = FetcherState::getDataLow;
        pixelFifo.fetcherX += 8;
    }
    void PPU::runGetDataLowState()
    {
        pixelFifo.bgwFetchData[1] = mmuPtr->read8(
                lcdPtr->readLCDC(LCD::LCDCFlag::bgwTileData) +
                (pixelFifo.bgwFetchData[0] * 16) +
                pixelFifo.tileY
                );

        fetchSpriteData(0);

        pixelFifo.currentState = FetcherState::getDataHigh;
    }
    void PPU::runGetDataHightState()
    {
        pixelFifo.bgwFetchData[2] = mmuPtr->read8(
                lcdPtr->readLCDC(LCD::LCDCFlag::bgwTileData) +
                (pixelFifo.bgwFetchData[0] * 16) +
                pixelFifo.tileY + 1);

        fetchSpriteData(1);

        pixelFifo.currentState = FetcherState::sleep;
    }
    void PPU::runSleepState()
    {
        pixelFifo.currentState = FetcherState::pushPixel;
    }
    void PPU::runPushPixelState()
    {
        if (canPushToFIFO())
        {
            pixelFifo.currentState = FetcherState::getTile;
        }
    }

    bool PPU::canPushToFIFO()
    {
        if (isFifoFull())
        {
            return false;
        }

        int x = pixelFifo.fetcherX - (8 - (lcdPtr->scx.read() % 8));
        if (x < 0)
        {
            return true;
        }

        for (int bit = 7; bit >= 0; --bit)
        {
            uint8_t colorIndex = (!!(pixelFifo.bgwFetchData[2] & (1 << bit)) << 1) |
                               (!!(pixelFifo.bgwFetchData[1] & (1 << bit)));

            uint32_t color = static_cast<uint32_t>(lcdPtr->bgColors[colorIndex]);

            if (!lcdPtr->readLCDC(LCD::LCDCFlag::bgwEnable))
            {
                colorIndex = static_cast<uint32_t>(lcdPtr->bgColors[0]);
            }

            if (lcdPtr->readLCDC(LCD::LCDCFlag::spriteEnable))
            {
                color = fetchSpritePixels(bit, color, colorIndex);
            }

            pixelFifo.fifo.push(color);
            pixelFifo.fifoX++;
        }

        return true;
    }


    void PPU::pipelineResetFIFO()
    {
        while(!pixelFifo.fifo.empty())
        {
            pixelFifo.fifo.pop();
        }
    }

    uint32_t PPU::fetchSpritePixels(uint8_t _bit, uint32_t _color, uint8_t _colorIndex)
    {
        for (int sprite = 0; sprite < fetchedLineSprites.size(); ++sprite)
        {
            int spriteX = (fetchedLineSprites.at(sprite).x - 8) + (lcdPtr->scx.read() % 8);
            if (spriteX + 8 < pixelFifo.fifoX) { continue; }

            int offset = pixelFifo.fifoX - spriteX;
            if (offset < 0 || offset > 7) { continue; }

            _bit = (7 - offset);

            if (fetchedLineSprites.at(sprite).attributes.xFlip) { _bit = offset; }

            uint8_t colorIndex = (!!(pixelFifo.spriteFetchData.at((sprite * 2) + 1) & (1 << _bit)) << 1) |
                                 (!!(pixelFifo.spriteFetchData.at((sprite * 2))     & (1 << _bit)));
            if (!colorIndex) { continue; }

            bool bgPriority = fetchedLineSprites[sprite].attributes.priority;
            if (!bgPriority || _colorIndex == 0)
            {
                _color = fetchedLineSprites[sprite].attributes.dmgPalette ?
                        static_cast<uint32_t>(lcdPtr->spriteColor1[colorIndex]) : static_cast<uint32_t >(lcdPtr->spriteColor0[colorIndex]);

                break;
            }
        }

        return _color;
    }

    void PPU::fetchSpriteTiles()
    {
        for (auto &sprite : lineSprites)
        {
            if (fetchedLineSprites.size() >= 3) { break; }
            int spriteX = (sprite.x - 8) + (lcdPtr->scx.read() % 8);

            if ((spriteX >= pixelFifo.fetcherX && spriteX < pixelFifo.fetcherX + 8) ||
                ((spriteX + 8) >= pixelFifo.fetcherX && (spriteX + 8) < pixelFifo.fetcherX + 8))
            {
                fetchedLineSprites.push_back(sprite);
            }
        }
    }

    void PPU::fetchSpriteData(uint8_t _offset)
    {
        uint16_t currentLine = lcdPtr->ly.read();
        uint8_t spriteHeight = lcdPtr->readLCDC(LCD::LCDCFlag::spriteSize);

        for (int sprite = 0; sprite < fetchedLineSprites.size(); ++sprite)
        {
            uint8_t tileY = ((currentLine + 16) - fetchedLineSprites.at(sprite).y) * 2;
            //uint8_t tileY = ((currentLine + 16) - (fetchedLineSprites.at(sprite).y - 16)) * 2;

            if (fetchedLineSprites.at(sprite).attributes.yFlip)
            {
                tileY = ((spriteHeight * 2) - 2) - tileY;
            }

            uint8_t tileIndex = fetchedLineSprites.at(sprite).tileIndex;

            if (spriteHeight == 16)
            {
                tileIndex &= ~(1);
            }

            pixelFifo.spriteFetchData[(sprite * 2) + _offset] = mmuPtr->read8(0x8000 + (tileIndex * 16) + tileY + _offset);
        }
    }

    void PPU::loadWindowTile()
    {
        if (!lcdPtr->isWindowVisible()) { return; }

        if (pixelFifo.fetcherX + 7 >= lcdPtr->windowX.read() &&
                pixelFifo.fetcherX + 7 < lcdPtr->windowX.read() + Renderer::yResolution + 14)
        {
            if (lcdPtr->ly >= lcdPtr->windowY && lcdPtr->ly < lcdPtr->windowY + Renderer::xResolution)
            {
                uint8_t windowTileY = visibleWindowCounter / 8;
                pixelFifo.bgwFetchData[0] = mmuPtr->read8(
                        lcdPtr->readLCDC(LCD::LCDCFlag::windowTileMap) +
                        ((pixelFifo.fetcherX + 7 - lcdPtr->windowX.read()) / 8) +
                        (windowTileY * 32));

                if (lcdPtr->readLCDC(LCD::LCDCFlag::bgwTileData) == 0x8800)
                {
                    pixelFifo.bgwFetchData[0] += 128;
                }
            }
        }
    }
}
