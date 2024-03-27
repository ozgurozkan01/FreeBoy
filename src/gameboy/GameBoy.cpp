//
// Created by ozgur on 27/03/2024.
//

#include "GameBoy.h"
#include <cstdio>
#include "../../src/graphics/Graphic.h"

GameBoy::GameBoy() :
    graphic(new Graphic(160, 144, "GameBoy-Emulator")),
    emulatorState(EmulatorState::RUNNING)
{
}

bool GameBoy::init()
{
    if (!graphic->init())
    {
        printf("ERROR : Graphics could not be loaded!");
        return false;
    }

/*    if (!cartridge->loadCartridge((char*)""))
    {
        printf("ERROR : Cartridge could not be loaded!");
        return false;
    }*/

    return true;
}

void GameBoy::run()
{
    graphic->run();
}
