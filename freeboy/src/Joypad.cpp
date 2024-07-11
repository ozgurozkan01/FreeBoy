//
// Created by ozgur on 6/26/2024.
//

#include "../include/Joypad.h"
#include <cstdio>

namespace gameboy
{
    Joypad::Joypad() :
    joypad(0xCF),
    start(false),
    select(false),
    a(false),
    b(false),
    up(false),
    down(false),
    left(false),
    right(false),
    button(false),
    dPad(false)
    {}

    void Joypad::setSelections(const uint8_t _value)
    {
        button = _value & 0x20;
        dPad = _value & 0x10;
    }

    uint8_t Joypad::read()
    {
        joypad = 0xCF;

        if (!button)
        {
            if (a)           { joypad &= ~(static_cast<uint8_t>(Input::a)); }
            else if (b)      { joypad &= ~(static_cast<uint8_t>(Input::b)); }
            else if (select) { joypad &= ~(static_cast<uint8_t>(Input::select)); }
            else if (start)  { joypad &= ~(static_cast<uint8_t>(Input::start)); }
        }

        if (!dPad)
        {
            if (right)       { joypad &= ~(static_cast<uint8_t>(Input::right)); }
            else if (left)   { joypad &= ~(static_cast<uint8_t>(Input::left)); }
            else if (up)     { joypad &= ~(static_cast<uint8_t>(Input::up)); }
            else if (down)   { joypad &= ~(static_cast<uint8_t>(Input::down)); }
        }

        return joypad.read();
    }

    void Joypad::handleJoypad(bool _isPressed, const uint32_t _keyCode)
    {
        switch(_keyCode)
        {
            case SDLK_z:        b = _isPressed; break;
            case SDLK_x:        a = _isPressed; break;
            case SDLK_RETURN:   start = _isPressed; break;
            case SDLK_TAB:      select = _isPressed; break;
            case SDLK_UP:       up = _isPressed; break;
            case SDLK_DOWN:     down = _isPressed; break;
            case SDLK_LEFT:     left = _isPressed; break;
            case SDLK_RIGHT:    right = _isPressed; break;
        }
    }
}