//
// Created by ozgur on 6/12/2024.
//

#ifndef FREEBOY_ALU_H
#define FREEBOY_ALU_H

#include <cstdint>
#include "Registers.h"

namespace gameboy
{
    class CPU;

    using namespace cpu_register;

    class ALU {
    public:
        ALU(CPU* _cpu);

        bool init();

        void xor_(const Register8& _register);
        void xor_(const uint8_t _value);

        void or_(const Register8& _srcRegister);
        void or_(const uint8_t _value);

        void increment(Register16& _register);
        void increment(Register8& _register);

        void decrement(Register16& _register);
        void decrement(Register8& _register);

    private:
        CPU* cpuPtr;
    };
}


#endif //FREEBOY_ALU_H
