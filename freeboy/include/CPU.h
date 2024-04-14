//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_CPU_H
#define FREEBOY_CPU_H

#include <cstdint>

namespace gameboy
{
    enum class FLAG_TYPE : uint8_t
    {
        // LOWER 8 BITS OF AF
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
    };
}

#endif //FREEBOY_CPU_H
