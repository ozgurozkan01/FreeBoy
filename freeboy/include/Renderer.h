//
// Created by ozgur on 7/1/2024.
//

#ifndef FREEBOY_RENDERER_H
#define FREEBOY_RENDERER_H

#include "SDL2/SDL.h"
#include "SDL_TTF/SDL_ttf.h"
#include <cstdint>
#include <array>

namespace gameboy
{
    class MMU;

    namespace graphic
    {
        class PPU;

        class Renderer {
        public:
            Renderer(const char *_windowName, MMU* _mmu, PPU* _ppu);
            ~Renderer();

            bool init();
            void render();
            void renderDebugger();
            void renderMainWindow();

            static const uint8_t scale = 4;

            static const uint16_t yResolution = 144;
            static const uint16_t xResolution = 160;

            static const uint16_t screenWidth = xResolution * scale;
            static const uint16_t screenHeight = yResolution * scale;
        private:

            void displayTile(SDL_Surface *_surface, uint16_t _tileIndex, uint16_t _x, uint16_t _y);


            const char* windowName;

            const char* debuggerName;
            const uint16_t debuggerHeight;
            const uint16_t debuggerWidth;

            SDL_Window* mainWindow;
            SDL_Renderer* mainRenderer;
            SDL_Texture* mainTexture;
            SDL_Surface* mainSurface;

            SDL_Window* debugWindow;
            SDL_Renderer* debugRenderer;
            SDL_Texture* debugTexture;
            SDL_Surface* debugSurface;

            const uint8_t tileColumn;
            const uint8_t tileRow;

            std::array<uint32_t, 4> tilePalette = { 0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000 };
            MMU* mmuPtr;
            PPU* ppuPtr;
        };
    }
}

#endif //FREEBOY_RENDERER_H
