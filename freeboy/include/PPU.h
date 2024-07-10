//
// Created by ozgur on 6/19/2024.
//

#ifndef FREEBOY_PPU_H
#define FREEBOY_PPU_H

#include <cstdint>
#include <array>
#include <queue>

namespace gameboy
{
    class InterruptHandler;
    class MMU;

    /*
     LCD viewport 20x18 tiles (160x144 pixels)

     3 Layers : Background 32x32 Tiles (256x256 pixels)
                Window     32x32 Tiles (256x256 pixels)
                Sprites
     */

    namespace graphic
    {
        class LCD;

        namespace ModeCycles
        {
            // Cycle Amounts of each mode in per scanline
            const uint16_t hBlankCycle = 204;
            const uint16_t vBlankCycle = 456;
            const uint8_t oamCycle = 80;
            const uint16_t transferCycle = 172;
        }

        class PPU {
            friend class LCD;
            friend class Renderer;
            // Fetcher stands for fetching tile data and preparing it for the Pixel FIFO
            enum class FetcherState : uint8_t
            {
                getTile,
                getDataLow,
                getDataHigh,
                sleep,
                pushPixel
            };

            enum class PPUMode : uint8_t
            {
                hBlank = 0, // 87–204 dots
                vBlank = 1, // 4560 dots (10 scanlines = 144-153)
                oamScan = 2, // 80 dots
                transferData = 3 // 172–289 dots
            };

            // Sprite data is stored in the OAM section of memory which can fit up to 40 sprites.
            struct OAM
            {
                uint8_t y;
                uint8_t x;
                uint8_t tileIndex;
                struct Attributes
                {
                    uint8_t  cgbPalette : 3;
                    uint8_t  vramBank : 1;
                    uint8_t  dmgPalette : 1;
                    uint8_t  xFlip : 1;
                    uint8_t  yFlip : 1;
                    uint8_t  priority : 1;
                }attributes;
            };

            struct PixelFIFO
            {
                FetcherState currentState;
                uint8_t scanlineX;
                uint8_t pushedX;
                uint8_t fetcherX;
                uint8_t mapY;
                uint8_t mapX;
                uint8_t tileY;
                uint8_t fifoX;
                std::array<uint8_t, 3> bgwFetchData;
                std::array<uint8_t, 6> spriteFetchData;
                std::queue<uint32_t> fifo;

            }pixelFifo;

        public:
            PPU(InterruptHandler* _interruptHandler, MMU* _mmu);

            void tick();

            void writeVRAM(const uint16_t _address, const uint8_t _value);
            uint8_t readVRAM(const uint16_t _address);

            void writeOAM(const uint16_t _address, const uint8_t _value);
            uint8_t readOAM(const uint16_t _address);

            void connectLCD(LCD* _lcd) { lcdPtr = _lcd; setPPUMode(PPUMode::oamScan);};

            uint32_t getRenderedFrame() const { return renderedFrame;}
            void calculateFPS();

            bool isFifoFull();
        private:
            void setPPUMode(const PPUMode _mode);
            PPUMode getPPUMode();

            void runOAM();
            void runHBlank();
            void runVBlank();
            void runTransferData();

            void resetPixelFIFO();
            void updateVisibleSprites();
            void fetchSpriteTiles();
            void fetchSpriteData(uint8_t _offset);
            uint32_t fetchSpritePixels(uint8_t _bit, uint32_t _color, uint8_t _colorIndex);

            void pipelineProcess();
            void pipelineFetch();
            void pipelinePushPixel();
            void pipelineResetFIFO();
            bool canPushToFIFO();

            void runGetTileState();
            void runGetDataLowState();
            void runGetDataHightState();
            void runSleepState();
            void runPushPixelState();

            void loadWindowTile();

            const uint8_t maxSpritePerLine;
            const uint8_t maxSpriteAmount;
            const uint8_t fifoCapacity;
            // Window
            uint8_t visibleWindowCounter;
            // Renderin Variables
            const uint16_t visibleScanlinePerFrame;
            const uint16_t scanlinePerFrame;
            uint16_t scanlineTCycle;
            // FPS variables...
            const float targetFPS;
            const float targetDeltaTime;
            uint32_t frameDuration;
            uint32_t previousFrame;
            uint32_t calculationStartTime;
            uint32_t frameCount;
            uint32_t renderedFrame;


            // Memory
            std::array<uint8_t, 0x2000> videoRAM;
            std::array<OAM, 0x28> oamRAM;
            std::array<uint32_t, 160 * 144> videoBuffer; // Pixel data buffer for rendering
            std::vector<OAM> lineSprites; // max 10
            std::vector<OAM> fetchedLineSprites; // max 3
            // Components
            InterruptHandler* interruptHandlerPtr;
            LCD* lcdPtr;
            MMU* mmuPtr;
        };
    }
}

#endif //FREEBOY_PPU_H
