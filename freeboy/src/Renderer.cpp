//
// Created by ozgur on 7/1/2024.
//

#include "../include/Renderer.h"
#include "../include/MMU.h"
#include <cstdio>

namespace gameboy::graphic
{
    Renderer::Renderer(const char *_windowName, MMU* _mmu):
            windowName(_windowName),
            windowWidth(160),
            windowHeight(144),
            debuggerName("GameBoy Debugger"),
            debuggerWidth(16 * 8),
            debuggerHeight(32 * 8),
            windowScale(4),
            debuggerScale(3),
            tileColumn(24),
            tileRow(16),
            mmuPtr(_mmu)
            {}

    bool Renderer::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) { printf("SDL Init is failed!!"); exit(-1); }
        if (TTF_Init() < 0) { printf("TTF Init is failed!!"); exit(-1); }

        SDL_CreateWindowAndRenderer(windowWidth * windowScale, windowHeight * windowScale, 0, &mainWindow, &mainRenderer);
        SDL_SetWindowTitle(mainWindow, windowName);

        SDL_CreateWindowAndRenderer(debuggerWidth * debuggerScale, debuggerHeight * debuggerScale, 0, &debugWindow, &debugRenderer);
        SDL_SetWindowTitle(debugWindow, debuggerName);

        debugSurface = SDL_CreateRGBSurface(0, (16 * 8 * debuggerScale) + (16 * debuggerScale),
                                            (32 * 8 * debuggerScale) + (64 * debuggerScale), 32,
                                            0x00FF0000,
                                            0x0000FF00,
                                            0x000000FF,
                                            0xFF000000);

        debugTexture = SDL_CreateTexture(debugRenderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         (16 * 8 * debuggerScale) + (16 * debuggerScale),
                                         (32 * 8 * debuggerScale) + (64 * debuggerScale));

        int x, y;
        SDL_GetWindowPosition(mainWindow, &x, &y);
        SDL_SetWindowPosition(debugWindow, x + windowWidth * windowScale + 10, y);
        return true;
    }

    Renderer::~Renderer()
    {
        if (debugTexture) SDL_DestroyTexture(debugTexture);
        if (debugRenderer) SDL_DestroyRenderer(debugRenderer);
        if (debugWindow) SDL_DestroyWindow(debugWindow);

        if (mainRenderer) SDL_DestroyRenderer(mainRenderer);
        if (mainWindow) SDL_DestroyWindow(mainWindow);

        SDL_Quit();
        TTF_Quit();
    }

    void Renderer::displayTile(SDL_Surface *_surface, uint16_t _tileIndex, uint16_t _x, uint16_t _y)
    {
        SDL_Rect tile;

        // It is used 8000 method in here.
        // https://github.com/Hacktix/GBEDG/blob/master/ppu/index.md#display-layers
        uint16_t tileAddress = 0x8000 + (_tileIndex * 16);
        for (uint8_t y = 0; y < 16; y += 2)
        {
            uint8_t highByte = mmuPtr->read8(tileAddress + y);
            uint8_t lowByte = mmuPtr->read8(tileAddress + y + 1);

            // to decrease from 7 to 0 was used because to increase from 0 to 7 is drawn everything as reverse
            for (int8_t bit = 7; bit >= 0; --bit)
            {
                uint8_t colorIndex = (!!(highByte & (1 << bit)) << 1) |
                                     (!!(lowByte & (1 << bit)));

                tile.x = _x + ((7 - bit) * debuggerScale);
                tile.y = _y + (y / 2 * debuggerScale);
                tile.w = debuggerScale;
                tile.h = debuggerScale;

                SDL_FillRect(_surface, &tile, tilePalette[colorIndex]);
            }
        }
    }

    void Renderer::renderDebugger()
    {
        int x = 0;
        int y = 0;
        int tileIndex = 0;

        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = debugSurface->w;
        rect.h = debugSurface->h;

        SDL_FillRect(debugSurface, &rect, 0xFF111111);

        // 384 tiles -> 24 x 16
        for (uint8_t column = 0; column < tileColumn; ++column)
        {
            for (uint8_t row = 0; row < tileRow; ++row)
            {
                displayTile(debugSurface, tileIndex, x + (row * debuggerScale), y + (column * debuggerScale));
                x += (8 * debuggerScale);
                tileIndex++;
            }
            y += (8 * debuggerScale);
            x = 0;
        }

        SDL_UpdateTexture(debugTexture, nullptr, debugSurface->pixels, debugSurface->pitch);
        SDL_RenderClear(debugRenderer);
        SDL_RenderCopy(debugRenderer, debugTexture, nullptr, nullptr);
        SDL_RenderPresent(debugRenderer);
    }
}