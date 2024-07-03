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

        class Renderer {
        public:
            Renderer(const char *_windowName, MMU* _mmu);
            ~Renderer();

            bool init();
            void renderDebugger();
        private:

            void displayTile(SDL_Surface *_surface, uint16_t _tileIndex, uint16_t _x, uint16_t _y);

            const uint8_t windowScale;
            const uint8_t debuggerScale;

            const char* windowName;
            const uint16_t windowHeight;
            const uint16_t windowWidth;

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
        };
    }
}

#endif //FREEBOY_RENDERER_H
