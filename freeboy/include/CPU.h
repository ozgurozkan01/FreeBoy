//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_CPU_H
#define FREEBOY_CPU_H

#include <cstdint>
#include "Register.h"

namespace gameboy
{
    enum class FLAG_TYPE : uint8_t
    {
        // LOWER 8 BITS OF AF
        NONE = 0x0,
        ZERO_FLAG = 1 << 7,
        SUBTRACT_FLAG = 1 << 6, // (BCD)
        HALF_CARRY_FLAG = 1 << 5, // (BCD)
        CARRY_FLAG = 1 << 4
    };

    class CPU {
    public:
        CPU();
        bool init();
    private:
        uint16_t fetchData; // get the data for decoding

        Register16 PC; // program counter
        Register16 SP; // stack pointer
        /* accumulator register */
        Register16 AF; // A -> high, ZNHC0000 -> low
        /* general purpose register */
        Register16 BC; // B -> high, C -> low
        Register16 DE; // D -> high, E -> low
        Register16 HL; // H -> high, L -> low
    };
}

#endif //FREEBOY_CPU_H
