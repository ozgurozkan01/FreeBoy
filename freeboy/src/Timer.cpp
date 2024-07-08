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
            interruptHandlerPtr(_interruptHandler),
            div(0xABCC),
            tac(0xF8)
    {}

    void Timer::write(uint16_t _address, uint8_t _value)
    {
        switch (_address) {
            case 0xFF04: div = 0; break;
            case 0xFF05: tima = _value; break;
            case 0xFF06: tma = _value; break;
            case 0xFF07: tac = _value; break;
            default: printf("Invalid Timer Address : %#02x", _address); exit(-1);
        }
    }

    uint8_t Timer::read(uint16_t _address)
    {
        switch (_address) {
            case 0xFF04: return div.read() >> 8;
            case 0xFF05: return tima.read();
            case 0xFF06: return tma.read();
            case 0xFF07: return tac.read();
            default: printf("Invalid Timer Address : %#02x", _address); exit(-1);
        }
    }

    void Timer::tick()
    {
        updateTimer();
    }

    void Timer::updateTimer()
    {
        uint16_t prevDiv = div.read();

        div++;

        bool isFallingEdge;

        switch (tac & 0x03)
        {
            case 0x00:
            {
                uint16_t prevResult = prevDiv & (1 << 9);
                uint16_t currentResult = !(div & (1 << 9));
                isFallingEdge = prevResult && currentResult;
                break;
            }
            case 0x01:
            {
                uint16_t prevResult = prevDiv & (1 << 3);
                uint16_t currentResult = !(div & (1 << 3));
                isFallingEdge = prevResult && currentResult;
                break;
            }

            case 0x10:
            {
                uint16_t prevResult = prevDiv & (1 << 5);
                uint16_t currentResult = !(div & (1 << 5));
                isFallingEdge = prevResult && currentResult;
                break;
            }

            case 0x11:
            {
                uint16_t prevResult = prevDiv & (1 << 7);
                uint16_t currentResult = !(div & (1 << 7));
                isFallingEdge = prevResult && currentResult;
                break;
            }
        }

        bool currentAndResult = isFallingEdge && (tac & (1 << 2));
        if (currentAndResult)
        {
            updateTIMA();
        }
    }

    void Timer::updateTIMA()
    {
        tima++;

        if (tima == 0xFF)
        {
            tima = tma;
            interruptHandlerPtr->requestInterrupt(InterruptType::timer);
        }
    }
}

