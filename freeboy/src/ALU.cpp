//
// Created by ozgur on 6/12/2024.
//

#include "../include/ALU.h"
#include "../include/CPU.h"
#include <cstdio>

namespace gameboy
{
    ALU::ALU(CPU *_cpu) :
            cpuPtr(_cpu)
    {}

    bool ALU::init()
    {
        if (cpuPtr == nullptr)
        {
            printf("ERROR : CPU Ref is NULL!");
            return false;
        }

        return true;
    }

    void ALU::or_(const Register8 &_srcRegister)
    {
        or_(_srcRegister.read());
    }

    void ALU::or_(const uint8_t _value)
    {
        cpuPtr->AF.highByte() |= _value;

        uint8_t result = cpuPtr->AF.highByte().read();

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::xor_(const Register8& _srcRegister)
    {
        xor_(_srcRegister.read());
    }

    void ALU::xor_(const uint8_t _value)
    {
        cpuPtr->AF.highByte() ^= _value;

        uint8_t result = cpuPtr->AF.highByte().read();

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::increment(Register16 &_srcRegister)
    {
        _srcRegister++;
    }

    void ALU::increment(Register8 &_srcRegister)
    {
        uint8_t data = _srcRegister.read();
        increment(data);
        _srcRegister = data;
    }

    void ALU::increment(uint8_t &_value)
    {
        _value++;

        _value == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        (_value & 0xF) == 0 ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::decrement(Register16 &_srcRegister)
    {
        _srcRegister--;
    }

    void ALU::decrement(Register8 &_srcRegister)
    {
        uint8_t data = _srcRegister.read();
        decrement(data);
        _srcRegister = data;
    }

    void ALU::decrement(uint8_t &_value)
    {
        _value--;

        _value == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->setFlag(CPUFlags::n);
        (_value & 0xF) == 0xF ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::compare(Register8 &_srcRegister)
    {
        compare(_srcRegister.read());
    }

    void ALU::compare(const uint8_t _value)
    {
        uint8_t result = cpuPtr->AF.highByte().read() - _value;

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->setFlag(CPUFlags::n);
        (result & 0xF) == 0xF ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        _value > cpuPtr->AF.highByte().read() ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::add(const uint8_t _value)
    {
        cpuPtr->AF.highByte() += _value;
        uint16_t result = cpuPtr->AF.highByte().read();

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        (result & 0xF) == 0 ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        (result > 0xFF) ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::add(const Register8 &_srcRegister)
    {
        add(_srcRegister.read());
    }

    void ALU::add(const Register16 &_srcRegister)
    {
        uint32_t result = cpuPtr->HL.read() + _srcRegister.read();

        cpuPtr->HL = result & 0xFFFF;

        cpuPtr->resetFlag(CPUFlags::n);
        ((cpuPtr->HL.read() & 0x0FFF) + (_srcRegister.read() & 0x0FFF) > 0x0FFF) ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        result > 0xFFFF ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::addToStack(const int8_t _value)
    {
        cpuPtr->SP += _value;

        cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        // TODO : update h and c flag
    }

    void ALU::and_(const Register8 _srcRegister)
    {
        and_(_srcRegister.read());
    }

    void ALU::and_(const uint8_t _value)
    {
        cpuPtr->AF.highByte() &= _value;

        uint8_t result = cpuPtr->AF.highByte().read();

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->setFlag(CPUFlags::h);
        cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::subtract(const uint8_t _value)
    {
        _value > cpuPtr->AF.highByte().read() ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
        (_value & 0x0F) > (cpuPtr->AF.highByte().read() & 0x0F) ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);

        cpuPtr->AF.highByte() -= _value;

        uint8_t result = cpuPtr->AF.highByte().read();

        result == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->setFlag(CPUFlags::n);
    }

    void ALU::subtract(const Register8 &_srcRegister)
    {
        subtract(_srcRegister.read());
    }

    void ALU::adc(const Register8 &_srcRegister)
    {
        adc(_srcRegister.read());
    }

    void ALU::adc(const uint8_t _value)
    {
        uint8_t A = cpuPtr->AF.highByte().read();
        bool cFlag = cpuPtr->readFlag(CPUFlags::c);

        printf("\nA: %#02x, cFlag: %#02x, _value: %#02x\n", A, cFlag, _value);

        (A & 0xF) + (_value & 0xF) + cFlag > 0xF ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        (A & 0xFF) + (_value & 0xFF) + cFlag > 0xFF ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
        cpuPtr->resetFlag(CPUFlags::n);

        cpuPtr->AF.highByte() = (A + _value + cFlag) & 0xFF;

        cpuPtr->AF.highByte().read() == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
    }

    void ALU::srl(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        srl(data);
        _dstRegister = data;
    }

    void ALU::srl(uint8_t& _value)
    {
        (_value & 0x1) ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);

        _value >>= 1;

        _value == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::rr(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        printf("\nBefore : %02X\n", data);
        rr(data);
        printf("After : %02X\n", data);
        _dstRegister = data;
    }

    void ALU::rr(uint8_t &_value)
    {
        uint8_t cFlag = cpuPtr->readFlag(CPUFlags::c);

        (_value & 0x1) ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);

        _value = (_value >> 1) | (cFlag << 7);

        _value == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::rra()
    {
        rr(cpuPtr->AF.highByte());
        cpuPtr->resetFlag(CPUFlags::z);
    }

    void ALU::swap(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        swap(data);
        _dstRegister = data;
    }

    void ALU::swap(uint8_t &_value)
    {
        uint8_t lowerNibble = _value & 0x0F;
        uint8_t upperNibble = _value & 0xF0;

        uint8_t swapped = (lowerNibble << 4) | (upperNibble >> 4);

        _value = swapped;

        swapped == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        cpuPtr->resetFlag(CPUFlags::c);
    }

    /*
                     uint16_t value = registers->a;

                if(registers->is_flag_set(FLAG_SUBTRACT)){
                    if(registers->is_flag_set(FLAG_CARRY)) {
                        value -= 0x60;
                    }

                    if(registers->is_flag_set(FLAG_HALF_CARRY)) {
                        value -= 0x6;
                    }
                }else{
                    if(registers->is_flag_set(FLAG_CARRY) || value > 0x99) {
                        value += 0x60;
                        registers->set_flags(FLAG_CARRY, true);
                    }

                    if(registers->is_flag_set(FLAG_HALF_CARRY) || (value & 0xF) > 0x9) {
                        value += 0x6;
                    }

                }
                registers->a = value;

                registers->set_flags(FLAG_ZERO, !registers->a);
                registers->set_flags(FLAG_HALF_CARRY, false);
     * */

    void ALU::daa()
    {
        uint16_t A = cpuPtr->AF.highByte().read();

        if(cpuPtr->checkFlag(CPUFlags::n))
        {
            if(cpuPtr->checkFlag(CPUFlags::c))
            {
                A -= 0x60;
            }

            if(cpuPtr->checkFlag(CPUFlags::h))
            {
                A -= 0x06;
            }
        }

        else
        {
            if(cpuPtr->checkFlag(CPUFlags::c) || A > 0x99)
            {
                A += 0x60;
                cpuPtr->setFlag(CPUFlags::c);
            }

            if(cpuPtr->checkFlag(CPUFlags::h) || (A & 0xF) > 0x9)
            {
                A += 0x06;
            }

        }

        cpuPtr->AF.highByte() = A;

        cpuPtr->AF.highByte().read() == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::rlca()
    {
        rlc(cpuPtr->AF.highByte());
        cpuPtr->resetFlag(CPUFlags::z);
    }

    void ALU::rlc(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        rlc(data);
        _dstRegister = data;
    }

    void ALU::rlc(uint8_t &_value)
    {
        uint8_t outBit = (_value >> 7) & 0x01;

        _value = (_value << 1) | outBit;

        _value == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        outBit == 0x0 ? cpuPtr->resetFlag(CPUFlags::c) : cpuPtr->setFlag(CPUFlags::c);
    }
}