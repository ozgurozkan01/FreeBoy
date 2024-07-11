//
// Created by ozgur on 6/26/2024.
//

#ifndef FREEBOY_JOYPAD_H
#define FREEBOY_JOYPAD_H

#include <cstdint>
#include "SDL2/SDL.h"
#include "Registers.h"

namespace gameboy
{
    using namespace cpu_register;

    class Joypad {

        enum class Input : uint8_t
        {
            a = (1 << 0),
            b = (1 << 1),
            select = (1 << 2),
            start = (1 << 3),

            right = (1 << 0),
            left = (1 << 1),
            up = (1 << 2),
            down = (1 << 3),
        };

    public:
        Joypad();

        void setSelections(const uint8_t _value);
        void handleJoypad(bool _isPressed, const  uint32_t _keyCode);

        uint8_t read();
    private:
        bool start;
        bool select;
        bool a;
        bool b;
        bool up;
        bool down;
        bool left;
        bool right;

        bool button;
        bool dPad;
        Register8 joypad;
    };
}


#endif //FREEBOY_JOYPAD_H
