//
// Created by ozgur on 10/04/2024.
//

#ifndef FREEBOY_GRAPHIC_H
#define FREEBOY_GRAPHIC_H


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

#endif //FREEBOY_GRAPHIC_H
