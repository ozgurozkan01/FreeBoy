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

        /* STANDARD INSTRUCTIONS */
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

        void adc(const Register8& _srcRegister);
        void adc(const uint8_t _value);

        void xor_(const Register8& _srcRegister);
        void xor_(const uint8_t _value);

        void or_(const Register8& _srcRegister);
        void or_(const uint8_t _value);

        void and_(const Register8 _srcRegister);
        void and_(const uint8_t _value);

        void compare(Register8& _srcRegister);
        void compare(const uint8_t _value);

        void subtract(const Register8& _srcRegister);
        void subtract(const uint8_t _value);

        void subtractCarry(const Register8& _srcRegister);
        void subtractCarry(const uint8_t _value);

        void complement();
        void complementCarryFlag();
        void rlca();
        void rrca();
        void daa();
        void rla();
        void rra();

        /* CB INSTRUCTIONS */
        void srl(Register8& _dstRegister);
        void srl(uint8_t& _value);

        void sla(Register8& _dstRegister);
        void sla(uint8_t& _value);

        void sra(Register8& _dstRegister);
        void sra(uint8_t& _value);

        void rr(Register8& _dstRegister);
        void rr(uint8_t& _value);

        void rl(Register8& _dstRegister);
        void rl(uint8_t& _value);

        void rrc(Register8& _dstRegister);
        void rrc(uint8_t& _value);

        void rlc(Register8& _dstRegister);
        void rlc(uint8_t& _value);

        void res(Register8& _dstRegister, const uint8_t _bit);
        void res(uint8_t& _value, const uint8_t _bit);

        void set(Register8& _dstRegister, const uint8_t _bit);
        void set(uint8_t& _value, const uint8_t _bit);

        void swap(Register8& _dstRegister);
        void swap(uint8_t& _value);

        void bit(Register8& _dstRegister, const uint8_t _bit);
        void bit(uint8_t& _value, const uint8_t _bit);

    private:
        CPU* cpuPtr;
    };
}


#endif //FREEBOY_ALU_H
