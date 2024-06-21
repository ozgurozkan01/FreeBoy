#include "freeboy/include/GameBoy.h"

int main(int argc, char **argv) {
    gameboy::GameBoy gameBoy("C:/Users/ozgur/GitHub/FreeBoy/ROMs/cpu_instrs.gb");
    if (gameBoy.init())
    {
        gameBoy.run();
    }
    return 0;
}
