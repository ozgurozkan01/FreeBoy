//
// Created by ozgur on 6/29/2024.
//

#ifndef FREEBOY_TIMER_H
#define FREEBOY_TIMER_H

#include "Registers.h"

namespace gameboy
{
    class InterruptHandler;
    class CPU;
    class MMU;

    using namespace cpu_register;

    class Timer {
    public:
        Timer(InterruptHandler* _interruptHandler);

        void write(uint16_t _address, uint8_t _value);
        uint8_t read(uint16_t _address);

        void tick();
    private:
        Register16 div; // Divider Register , MSByte is usable - 0xFF04
        Register8 tima; // Timer Counter    - 0xFF05
        Register8 tma;  // Timer Modulo     - 0xFF06
        Register8 tac;  // Timer Control    - 0xFF07

        const int clockFrequency;
        bool previousTimaResult;

        InterruptHandler* interruptHandlerPtr;
    };
}

#endif //FREEBOY_TIMER_H
