//
// Created by ozgur on 4/27/2024.
//

#ifndef FREEBOY_JOYPAD_H
#define FREEBOY_JOYPAD_H

#include <cstdint>

namespace gameboy
{
    enum class KEY_FLAG : uint8_t
    {
        KEY_A = (1u << 0),
        KEY_B = (1u << 1),
        KEY_SELECT = (1u << 2),
        KEY_START = (1u << 3),

        KEY_RIGHT = (1u << 4),
        KEY_LEFT = (1u << 5),
        KEY_UP = (1u << 6),
        KEY_DOWN = (1u << 7)
    };

    class Joypad {

    };
}

#endif //FREEBOY_JOYPAD_H
