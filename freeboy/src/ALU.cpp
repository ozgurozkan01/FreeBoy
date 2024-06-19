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
        _srcRegister++;

        uint8_t result = _srcRegister.read();

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        (result & 0xF) == 0 ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::decrement(Register16 &_srcRegister)
    {
        _srcRegister--;
    }

    void ALU::decrement(Register8 &_srcRegister)
    {
        _srcRegister--;

        uint8_t result = _srcRegister.read();

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->setFlag(CPUFlags::n);
        (result & 0xF) == 0xF ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::compare(Register8 &_srcRegister)
    {

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
        cpuPtr->HL += _srcRegister;
    }

    void ALU::addToStack(const int8_t _value)
    {

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
}