#include "freeboy/include/GameBoy.h"

int main(int argc, char **argv) {
    gameboy::GameBoy gameBoy;
    if (gameBoy.init())
    {
        gameBoy.run();
    }
    return 0;
}
