//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_CPU_H
#define FREEBOY_CPU_H

#include <cstdint>
#include "Register.h"
#include "Instruction.h"

namespace gameboy
{
    class Bus;

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
        CPU(Bus* _bus);
        bool init();
    private:
        uint8_t currentOpcode;

        registers::Register16 PC; // program counter
        registers::Register16 SP; // stack pointer
        /* accumulator register */
        registers::Register16 AF; // A -> high, ZNHC0000 -> low
        /* general purpose register */
        registers::Register16 BC; // B -> high, C -> low
        registers::Register16 DE; // D -> high, E -> low
        registers::Register16 HL; // H -> high, L -> low

        Instruction currentInstraction;
        Bus* busRef;

        bool isStopped;
        bool isHalted;

        void step();

        /* Instruction Methods */
        void nop();

        // LD Instructions
        [[maybe_unused]] void ld(registers::Register8& _register, uint8_t _data);
        [[maybe_unused]] void ld(registers::Register8& r_target, const registers::Register8& r_source);
        [[maybe_unused]] void ld(registers::Register16& _register, uint16_t _data);
        [[maybe_unused]] void ld(registers::Register16& r_target, const registers::Register16& r_source);
    };
}

#endif //FREEBOY_CPU_H
