//
// Created by ozgur on 10/04/2024.
//

#ifndef FREEBOY_GAMEBOY_H
#define FREEBOY_GAMEBOY_H


#include <cstdint>

class Graphic;

class GameBoy {
public:
    GameBoy();
    bool init();
    void run();
private:
    Graphic* graphic;
    uint64_t ticks;
    void processEvent();
};



#endif //FREEBOY_GAMEBOY_H
