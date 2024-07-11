//
// Created by ozgur on 6/15/2024.
//

#ifndef FREEBOY_IO_H
#define FREEBOY_IO_H

#include <array>
#include <cstdint>
#include "LCD.h"

namespace gameboy
{
    class Joypad;
    class Timer;
    class InterruptHandler;
    class DMA;

    using namespace graphic;

    class IO {
    public:
        IO(Joypad* _joypad, Timer* _timer, InterruptHandler* _interruptHandler, LCD* _lcd);

        uint8_t read(const uint16_t _address);
        void write(const uint16_t _address, const uint8_t _value);
    private:
        uint8_t sb; // Serial Transfer Data    - 0xFF01
        uint8_t sc; // Serial Transfer Control - 0xFF02

        InterruptHandler* interruptHandlerPtr;
        Joypad* joypadPtr;
        Timer* timerPtr;
        LCD* lcd;
    };
}

#endif //FREEBOY_IO_H
