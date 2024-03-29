//
// Created by ozgur on 27/03/2024.
//

#ifndef GAMEBOY_EMULATOR_SDL2_GRAPHIC_H
#define GAMEBOY_EMULATOR_SDL2_GRAPHIC_H

#include "SDL2/SDL.h"

class Cartridge;

class Graphic {
public:
    Graphic(const int windowWidth, const int windowHeigth, const char* windowName);
    bool init();
private:
    const int windowWidth;
    const int windowHeight;
    const char* windowName;

    SDL_Window* mainWindow;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    Cartridge* cartridge;
};


#endif //GAMEBOY_EMULATOR_SDL2_GRAPHIC_H
