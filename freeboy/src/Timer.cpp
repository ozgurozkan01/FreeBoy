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
            previousBit(false),
            interruptHandlerPtr(_interruptHandler),
            div(0xABCC),
            tima(0x00),
            tma(0x00),
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
        div++;

        bool currentBit;

        switch (tac & 0x03)
        {
            case 0x00: currentBit = getCurrentBit(9); break;
            case 0x01: currentBit = getCurrentBit(3); break;
            case 0x10: currentBit = getCurrentBit(5); break;
            case 0x11: currentBit = getCurrentBit(7); break;
        }

        bool isFallingEdge = !currentBit && previousBit;
        if (isFallingEdge)
        {
            tima++;

            if (tima == 0x00)
            {
                tima = tma;
                interruptHandlerPtr->setIFBit(InterruptType::timer);
            }
        }

        previousBit = currentBit;
    }

    bool Timer::getCurrentBit(const uint8_t _bit)
    {
        return (div & (1 << _bit)) && (tac & (1 << 3));
    }
}