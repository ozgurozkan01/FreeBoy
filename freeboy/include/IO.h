//
// Created by ozgur on 6/15/2024.
//

#ifndef FREEBOY_IO_H
#define FREEBOY_IO_H

#include <array>
#include <cstdint>

namespace gameboy
{
    class Joypad;
    class Timer;
    class InterruptHandler;
    class DMA;

    class IO {
    public:
        IO(Joypad* _joypad, Timer* _timer, InterruptHandler* _interruptHandler, DMA* _dma);

        uint8_t read(const uint16_t _address);
        void write(const uint16_t _address, const uint8_t _value);
    private:
        uint8_t sb; // Serial Transfer Data    - 0xFF01
        uint8_t sc; // Serial Transfer Control - 0xFF02

        InterruptHandler* interruptHandlerPtr;
        Joypad* joypadPtr;
        Timer* timerPtr;
        DMA* dma;

        uint8_t ly{};
    };
}

#endif //FREEBOY_IO_H
