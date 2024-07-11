//
// Created by ozgur on 6/15/2024.
//

#include "../include/IO.h"
#include "../include/InterruptHandler.h"
#include "../include/Joypad.h"
#include "../include/Timer.h"
#include "../include/DMA.h"

namespace gameboy
{
    IO::IO(Joypad *_joypad, Timer* _timer, InterruptHandler* _interruptHandler, LCD* _lcd) :
    interruptHandlerPtr(_interruptHandler),
    joypadPtr(_joypad),
    timerPtr(_timer),
    lcd(_lcd),
    sb(0x0),
    sc(0x7E)
    {}

    uint8_t IO::read(const uint16_t _address)
    {
        if (_address == 0xFF00)
        {
            return joypadPtr->read();
        }
        else if (_address == 0xFF01)
        {
            return sb;
        }
        else if (_address == 0xFF02)
        {
            return sc;
        }
        else if (_address >= 0xFF04 && _address <= 0xFF07)
        {
            return timerPtr->read(_address);
        }
        else if (_address == 0xFF0F)
        {
            return interruptHandlerPtr->getIF().read();
        }
        else if (_address <= 0xFF4B && _address >= 0xFF40)
        {
            return lcd->read(_address);
        }

        exit(-1);
    }

    void IO::write(const uint16_t _address, const uint8_t _value)
    {
        if (_address == 0xFF00)
        {
            joypadPtr->setSelections(_value);
            return;
        }

        else if (_address == 0xFF01)
        {
            sb = _value;
            return;
        }
        else if (_address == 0xFF02)
        {
            sc = _value;
            return;
        }
        else if (_address >= 0xFF04 && _address <= 0xFF07)
        {
            timerPtr->write(_address, _value);
            return;
        }
        else if (_address == 0xFF0F)
        {
            interruptHandlerPtr->setIF(_value);
            return;
        }
        else if (_address <= 0xFF4B && _address >= 0xFF40)
        {
            lcd->write(_address, _value);
            return;
        }
    }
}