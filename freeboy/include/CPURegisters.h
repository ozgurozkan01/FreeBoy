//
// Created by ozgur on 6/8/2024.
//

#ifndef FREEBOY_CPUREGISTERS_H
#define FREEBOY_CPUREGISTERS_H

#include <cstdint>
#include "../include/Instructions.h"

namespace gameboy
{
    using namespace instruction;

    namespace cpu_registers
    {
        class CPURegisters {
        public:
            CPURegisters();

            uint16_t readRegister(RegisterType _register) const; // Read value of core register
            void writeRegister(RegisterType _register, uint16_t _value); // Read value into core register

            uint8_t A = 0; // Accumulator Register -> Holds result of logical, arithmetic op and data
            uint8_t F = 0; // Flag-Status Register -> ZNHC0000
            // General Purpose Register
            uint8_t B = 0;
            uint8_t C = 0;
            uint8_t D = 0;
            uint8_t E = 0;
            uint8_t H = 0;
            uint8_t L = 0;
            uint16_t PC = 0; // Program Counter -> Holds address of the next instruction
            uint16_t SP = 0; // Stack Pointer -> Holds address of the last added element into the stack
        };
    }
}

#endif //FREEBOY_CPUREGISTERS_H
