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
        void increment(uint8_t& _value);

        void decrement(Register16& _srcRegister);
        void decrement(Register8& _srcRegister);
        void decrement(uint8_t& _value);

        void add(const Register8& _srcRegister);
        void add(const Register16& _srcRegister);
        void addToStack(const uint16_t _value);
        void add(const uint8_t _value);

        void compare(Register8& _srcRegister);
        void compare(const uint8_t _value);

        void subtract(const Register8& _srcRegister);
        void subtract(const uint8_t _value);


        void subtractCarry(const Register8& _srcRegister);
        void subtractCarry(const uint8_t _value);

        void adc(const Register8& _srcRegister); // Add value plus carry to A
        void adc(const uint8_t _value);

        void srl(Register8& _dstRegister); // Shift Right Logically
        void srl(uint8_t& _value);

        void rr(Register8& _dstRegister); // Rotate Register
        void rr(uint8_t& _value);

        void swap(Register8& _dstRegister); // Rotate Register
        void swap(uint8_t& _value);

        void rlc(Register8& _dstRegister);
        void rlc(uint8_t& _value);

        void rra(); // Rotate Register A
        void daa(); // Decimal Adjust Accumulator
        void rlca(); // Rotate register A left

    private:
        CPU* cpuPtr;
    };
}


#endif //FREEBOY_ALU_H
