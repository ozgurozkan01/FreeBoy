//
// Created by ozgur on 6/29/2024.
//

#include "../include/Timer.h"
#include "../include/InterruptHandler.h"
#include <cstdio>
#include <cstdlib>

namespace gameboy
{
    Timer::Timer(InterruptHandler* _interruptHandler) :
            clockFrequency(4194304),
            previousTimaResult(false),
            interruptHandlerPtr(_interruptHandler),
            div(0xABCC),
            tima(0x00),
            tma(0x00),
            tac(0xF8)
    {}

    void Timer::write(uint16_t _address, uint8_t _value)
    {
        if (_address == 0xFF04)      { div = 0;}
        else if (_address == 0xFF05) { tima = _value; }
        else if (_address == 0xFF06) { tma = _value; }
        else if (_address == 0xFF07) { tac = _value; }
        else
        {
            printf("INVALID MEMORY ADDRESS: %#02x", _address);
            exit(-1);
        }
    }

    uint8_t Timer::read(uint16_t _address)
    {
        if (_address == 0xFF04)      { return div.read() >> 8;}
        else if (_address == 0xFF05) { return tima.read(); }
        else if (_address == 0xFF06) { return tma.read(); }
        else if (_address == 0xFF07) { return tac.read(); }
        else
        {
            printf("INVALID MEMORY ADDRESS: %#02x", _address);
            exit(-1);
        }
    }

    void Timer::tick()
    {
        div++;

        bool newTimaResult;

        switch (tac & 0x03)
        {
            case 0x00:
                newTimaResult = (div.read() & (1 << 9)) && (tac & (1 << 3));
                break;
            case 0x01:
                newTimaResult = (div.read() & (1 << 3)) && (tac & (1 << 3));
                break;
            case 0x10:
                newTimaResult = (div.read() & (1 << 5)) && (tac & (1 << 3));
                break;
            case 0x11:
                newTimaResult = (div.read() & (1 << 7)) && (tac & (1 << 3));
                break;
        }

        if (!newTimaResult && previousTimaResult)
        {
            tima++;

            if (tima == 0x00)
            {
                tima = tma;
                interruptHandlerPtr->setIFBit(InterruptType::timer);
            }
        }

        previousTimaResult = newTimaResult;
    }
}