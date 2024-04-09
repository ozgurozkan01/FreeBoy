//
// Created by ozgur on 10/04/2024.
//

#include "../include/Graphic.h"
#include "../../freeboy/include/Cartridge.h"


Graphic::Graphic(const int windowWidth, const int windowHeigth, const char *windowName) :
        windowWidth(windowWidth), windowHeight(windowHeigth), windowName(windowName),
        cartridge(new Cartridge())
{
}

bool Graphic::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL library could not be loaded! Error code %s\n", SDL_GetError());
        return false;
    }

    mainWindow = SDL_CreateWindow(windowName,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  windowWidth, windowHeight,
                                  0);

    if (mainWindow == nullptr)
    {
        printf("SDL window could not be created! Error code %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(mainWindow,
                                  -1,
                                  SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr)
    {
        printf("SDL renderer could not be created! Error code %s\n", SDL_GetError());
        return false;
    }

    texture = SDL_CreateTexture(renderer,
                                SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888,
                                SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET,
                                windowWidth,
                                windowHeight);

    if (texture == nullptr)
    {
        printf("SDL texture could not be created! Error code %s\n", SDL_GetError());
        return false;
    }

    if (!cartridge->loadCartridge("C:/Users/ozgur/GitHub/FreeBoy/ROMs/dmg-acid2.gb"))
    {
        printf("Cartridge could not be created! Error code %s\n", SDL_GetError());
        return false;
    }

    return true;
}
