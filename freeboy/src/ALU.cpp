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

}