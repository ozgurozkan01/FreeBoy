//
// Created by ozgur on 7/1/2024.
//

#include "../include/Renderer.h"
#include "../include/MMU.h"
#include "../include/PPU.h"
#include <cstdio>

namespace gameboy::graphic
{
    Renderer::Renderer(const char *_windowName, MMU* _mmu, PPU* _ppu):
            windowName(_windowName),
            debuggerName("GameBoy Debugger"),
            debuggerWidth(16 * 8 * scale),
            debuggerHeight(32 * 8 * (scale / 2)),
            tileColumn(24),
            tileRow(16),
            mmuPtr(_mmu),
            ppuPtr(_ppu)
            {}

    bool Renderer::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) { printf("SDL Init is failed!!"); exit(-1); }
        if (TTF_Init() < 0) { printf("TTF Init is failed!!"); exit(-1); }

        SDL_CreateWindowAndRenderer(screenWidth, screenHeight, 0, &mainWindow, &mainRenderer);
        SDL_SetWindowTitle(mainWindow, windowName);


        mainSurface = SDL_CreateRGBSurface(0, screenWidth, screenHeight, 32,
                                      0x00FF0000,
                                      0x0000FF00,
                                      0x000000FF,
                                      0xFF000000);
        mainTexture = SDL_CreateTexture(mainRenderer,
                                       SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                        screenWidth, screenHeight);



        SDL_CreateWindowAndRenderer(debuggerWidth, debuggerHeight, 0, &debugWindow, &debugRenderer);
        SDL_SetWindowTitle(debugWindow, debuggerName);

        debugSurface = SDL_CreateRGBSurface(0,
                                            debuggerWidth + (16 * (scale / 2)),
                                            debuggerHeight + (64 * (scale / 2)),
                                            32,
                                            0x00FF0000,
                                            0x0000FF00,
                                            0x000000FF,
                                            0xFF000000);

        debugTexture = SDL_CreateTexture(debugRenderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         debuggerWidth + (16 * (scale / 2)),
                                         debuggerHeight + (64 * (scale / 2)));

        // Arrange the debugger window position as next of mainWindow
        int x, y;
        SDL_GetWindowPosition(mainWindow, &x, &y);
        SDL_SetWindowPosition(debugWindow, x + screenWidth + 10, y);
        return true;
    }

    Renderer::~Renderer()
    {
        if (debugTexture) { SDL_DestroyTexture(debugTexture); }
        if (debugRenderer) { SDL_DestroyRenderer(debugRenderer); }
        if (debugWindow) { SDL_DestroyWindow(debugWindow); }

        if (mainTexture) { SDL_DestroyTexture(mainTexture); }
        if (mainRenderer) { SDL_DestroyRenderer(mainRenderer); }
        if (mainWindow) { SDL_DestroyWindow(mainWindow); }

        TTF_Quit();
        SDL_Quit();
    }

    void Renderer::displayTile(SDL_Surface *_surface, uint16_t _tileIndex, uint16_t _x, uint16_t _y)
    {
        SDL_Rect tile;
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

                tile.x = _x + ((7 - bit) * (scale / 2));
                tile.y = _y + (y / 2 * (scale / 2));
                tile.w = (scale / 2);
                tile.h = (scale / 2);

                SDL_FillRect(_surface, &tile, tilePalette[colorIndex]);
            }
        }
    }

    void Renderer::render()
    {
        renderMainWindow();
        renderDebugger();
    }

    void Renderer::renderMainWindow()
    {
        SDL_Rect  rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = 3000;
        rect.h = 3000;

        for (int ly = 0; ly < yResolution; ++ly)
        {
            for (int lx = 0; lx < xResolution; ++lx)
            {
                rect.x = lx * scale;
                rect.y = ly * scale;
                rect.w = scale;
                rect.h = scale;

                SDL_FillRect(mainSurface, &rect, ppuPtr->videoBuffer[lx + (ly * xResolution)]);
            }
        }

        SDL_UpdateTexture(mainTexture, nullptr, mainSurface->pixels, mainSurface->pitch);
        SDL_RenderClear(mainRenderer);
        SDL_RenderCopy(mainRenderer, mainTexture, nullptr, nullptr);
        SDL_RenderPresent(mainRenderer);

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
                displayTile(debugSurface, tileIndex, x + (row * (scale / 2)), y + (column * (scale / 2)));
                x += (8 * (scale / 2));
                tileIndex++;
            }
            y += (8 * (scale / 2));
            x = 0;
        }

        SDL_UpdateTexture(debugTexture, nullptr, debugSurface->pixels, debugSurface->pitch);
        SDL_RenderClear(debugRenderer);
        SDL_RenderCopy(debugRenderer, debugTexture, nullptr, nullptr);
        SDL_RenderPresent(debugRenderer);
    }

}