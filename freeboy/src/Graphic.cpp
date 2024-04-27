//
// Created by ozgur on 10/04/2024.
//

#include "../include/Graphic.h"
#include <cstdio>
namespace gameboy
{
    Graphic::Graphic(const int windowWidth, const int windowHeigth, const char *windowName) :
            WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeigth), WINDOW_NAME(windowName), WINDOW_SCALE_MULTIPLIER(5)
    {
    }

    bool Graphic::init()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            printf("SDL library could not be loaded! Error code %s\n", SDL_GetError());
            return false;
        }

        mainWindow = SDL_CreateWindow(WINDOW_NAME,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      WINDOW_WIDTH * WINDOW_SCALE_MULTIPLIER ,
                                      WINDOW_HEIGHT * WINDOW_SCALE_MULTIPLIER,
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
                                    WINDOW_WIDTH,
                                    WINDOW_HEIGHT);

        if (texture == nullptr)
        {
            printf("SDL texture could not be created! Error code %s\n", SDL_GetError());
            return false;
        }

        return true;
    }
}
