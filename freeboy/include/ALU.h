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

        void xor_(const Register8& _srcRegister);
        void xor_(const uint8_t _value);

        void or_(const Register8& _srcRegister);
        void or_(const uint8_t _value);

        void and_(const Register8 _srcRegister);
        void and_(const uint8_t _value);

        void increment(Register16& _srcRegister);
        void increment(Register8& _srcRegister);

        void decrement(Register16& _srcRegister);
        void decrement(Register8& _srcRegister);

        void compare(Register8& _srcRegister);
        void compare(const uint8_t _value);

        void add(const Register8& _srcRegister);
        void add(const Register16& _srcRegister);
        void addToStack(const int8_t _value);
        void add(const uint8_t _value);

    private:
        CPU* cpuPtr;
    };
}


#endif //FREEBOY_ALU_H
