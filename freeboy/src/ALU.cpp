//
// Created by ozgur on 6/12/2024.
//

#include "../include/ALU.h"
#include "../include/CPU.h"
#include <cstdio>

namespace gameboy
{
    ALU::ALU(CPU *_cpu) : cpuPtr(_cpu)
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
        printf("\nBefore : %02X\n", cpuPtr->AF.highByte().read());
        cpuPtr->AF.highByte() = cpuPtr->AF.highByte().read() | _value;
        printf("After : %02X\n", cpuPtr->AF.highByte().read());

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
        printf("\nBefore : %02X\n", cpuPtr->AF.highByte().read());
        cpuPtr->AF.highByte() = cpuPtr->AF.highByte().read() ^ _value;
        printf("After : %02X\n", cpuPtr->AF.highByte().read());

        uint8_t result = cpuPtr->AF.highByte().read();

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::increment(Register16 &_srcRegister)
    {
        printf("\nBefore : %02X\n", _srcRegister.read());
        _srcRegister++;
        printf("After : %02X\n", _srcRegister.read());
    }

    void ALU::increment(Register8 &_srcRegister)
    {
        uint8_t data = _srcRegister.read();
        increment(data);
        _srcRegister = data;
    }

    void ALU::increment(uint8_t &_value)
    {
        printf("\nBefore : %02X\n", _value);
        _value++;
        printf("After : %02X\n", _value);


        _value == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        (_value & 0xF) == 0 ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::decrement(Register16 &_srcRegister)
    {
        printf("\nBefore : %02X\n", _srcRegister.read());
        _srcRegister--;
        printf("After : %02X\n", _srcRegister.read());
    }

    void ALU::decrement(Register8 &_srcRegister)
    {
        uint8_t data = _srcRegister.read();
        decrement(data);
        _srcRegister = data;
    }

    void ALU::decrement(uint8_t &_value)
    {
        (cpuPtr->AF.highByte() & 0xF) < (_value & 0xF) ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);

        printf("\nBefore : %02X\n", _value);
        _value--;
        printf("After : %02X\n", _value);

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
        int result = cpuPtr->AF.highByte().read() - _value;
        printf("A : %02x - Data : %02x - result : %02x\n", cpuPtr->AF.highByte().read(), _value, result);

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->setFlag(CPUFlags::n);
        (cpuPtr->AF.highByte().read() & 0x0F) < (_value & 0x0F) ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        _value > cpuPtr->AF.highByte().read() ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
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

        printf("\nBefore : %02X\n", _value);
        _value = swapped;
        printf("After : %02X\n", _value);

        swapped == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::add(const uint8_t _value)
    {
        (cpuPtr->AF.highByte().read() & 0x0F) + (_value & 0x0F) > 0x0F ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        (cpuPtr->AF.highByte().read() & 0xFF) + (_value & 0xFF) > 0xFF ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);

        printf("\nBefore : %02X\n", cpuPtr->AF.highByte().read());
        cpuPtr->AF.highByte() = cpuPtr->AF.highByte().read() + _value;
        printf("After : %02X\n", cpuPtr->AF.highByte().read());

        uint16_t result = cpuPtr->AF.highByte().read();

        result == 0x00 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
    }

    void ALU::add(const Register8 &_srcRegister)
    {
        add(_srcRegister.read());
    }

    void ALU::add(const Register16 &_srcRegister)
    {
        ((cpuPtr->HL.read() & 0x0FFF) + (_srcRegister.read() & 0x0FFF) > 0x0FFF) ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        ((cpuPtr->HL.read() & 0xFFFF) + (_srcRegister.read() & 0xFFFF) > 0xFFFF) ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
        cpuPtr->resetFlag(CPUFlags::n);

        printf("\nBefore : %02X\n", cpuPtr->HL.read());
        cpuPtr->HL = cpuPtr->HL.read() + _srcRegister.read();
        printf("After : %02X\n", cpuPtr->HL.read());
    }

    void ALU::addToStack(const uint16_t _value)
    {
        (cpuPtr->SP.read() & 0x0F) + (_value & 0x0F) > 0x0F ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        ((int)(cpuPtr->SP.read() & 0xFF) + (int)(_value & 0xFF)) > 0xFF ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
        printf("\n%02x\n", ((int)(cpuPtr->SP.read() & 0xFF) + (int)(_value & 0xFF)));

        printf("\nBefore : %02X\n", cpuPtr->SP.read());
        cpuPtr->SP = cpuPtr->SP.read() + static_cast<int8_t>(_value);
        printf("After : %02X\n", cpuPtr->SP.read());

        cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
    }

    void ALU::adc(const Register8 &_srcRegister)
    {
        adc(_srcRegister.read());
    }

    void ALU::adc(const uint8_t _value)
    {
        uint8_t A = cpuPtr->AF.highByte().read();
        bool cFlag = cpuPtr->readFlag(CPUFlags::c);

        (A & 0xF) + (_value & 0xF) + cFlag > 0xF ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        (A & 0xFF) + (_value & 0xFF) + cFlag > 0xFF ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
        cpuPtr->resetFlag(CPUFlags::n);

        printf("\nBefore : %02X\n", cpuPtr->AF.highByte().read());
        cpuPtr->AF.highByte() += _value + cFlag;
        printf("After : %02X\n", cpuPtr->AF.highByte().read());

        uint8_t result = cpuPtr->AF.highByte().read();

        result == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
    }

    void ALU::and_(const Register8 _srcRegister)
    {
        and_(_srcRegister.read());
    }

    void ALU::and_(const uint8_t _value)
    {
        printf("\nBefore : %02X\n", cpuPtr->AF.highByte().read());
        cpuPtr->AF.highByte() = cpuPtr->AF.highByte().read() & _value;
        printf("After : %02X\n", cpuPtr->AF.highByte().read());

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

        printf("\nBefore : %02X\n", cpuPtr->AF.highByte().read());
        cpuPtr->AF.highByte() = cpuPtr->AF.highByte().read() - _value;
        printf("After : %02X\n", cpuPtr->AF.highByte().read());

        uint8_t result = cpuPtr->AF.highByte().read();

        result == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->setFlag(CPUFlags::n);
    }

    void ALU::subtract(const Register8 &_srcRegister)
    {
        subtract(_srcRegister.read());
    }

    void ALU::subtractCarry(const Register8 &_srcRegister)
    {
        subtractCarry(_srcRegister.read());
    }

    void ALU::subtractCarry(const uint8_t _value)
    {
        uint8_t A = cpuPtr->AF.highByte().read();
        bool cFlag = cpuPtr->readFlag(CPUFlags::c);

        ((_value & 0x0F) + cFlag) > (A & 0x0F) ? cpuPtr->setFlag(CPUFlags::h) : cpuPtr->resetFlag(CPUFlags::h);
        (_value + cFlag) > A ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
        cpuPtr->setFlag(CPUFlags::n);

        printf("\nBefore : %02X\n", cpuPtr->AF.highByte().read());
        cpuPtr->AF.highByte() = A - _value - cFlag;
        printf("After : %02X\n", cpuPtr->AF.highByte().read());

        uint8_t result = cpuPtr->AF.highByte().read();

        result == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
    }

    void ALU::srl(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        srl(data);
        _dstRegister = data;
    }

    void ALU::srl(uint8_t& _value)
    {
        (_value & 0x01) ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);

        printf("\nBefore : %02X\n", _value);
        _value >>= 1;
        printf("After : %02X\n", _value);

        _value == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::sla(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        sla(data);
        _dstRegister = data;
    }

    void ALU::sla(uint8_t &_value)
    {
        bool newFlagC = (_value >> 7) & 0x01;

        _value <<= 1;

        _value == 0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        newFlagC ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::sra(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        sra(data);
        _dstRegister = data;
    }

    void ALU::sra(uint8_t &_value)
    {
        uint8_t msb = _value & 0x80;
        bool newFlagC = _value & 0x01;

        _value = (_value >> 1) | msb;

        _value == 0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        newFlagC ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::rr(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        rr(data);
        _dstRegister.write(data);
    }

    void ALU::rr(uint8_t &_value)
    {
        bool oldCFlag = cpuPtr->readFlag(CPUFlags::c);
        bool newCFlag = _value & 0x01;

        printf("\nBefore : %02X\n", _value);
        _value = (_value >> 1) | (oldCFlag << 7);
        printf("After : %02X\n", _value);

        _value == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        (newCFlag) ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::rra()
    {
        rr(cpuPtr->AF.highByte());
        cpuPtr->resetFlag(CPUFlags::z);
    }

    void ALU::rl(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        rl(data);
        _dstRegister = data;
    }

    void ALU::rl(uint8_t &_value)
    {
        bool oldFlagC = cpuPtr->readFlag(CPUFlags::c);
        bool newFlagC = (_value >> 7) & 0x01;

        _value = (_value << 1) | oldFlagC;

        _value == 0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        newFlagC ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::rla()
    {
        rl(cpuPtr->AF.highByte());
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
        uint8_t outBit = (_value & 0x80) >> 7 ;

        printf("\nBefore : %02X\n", _value);
        _value = (_value << 1) | outBit;
        printf("After : %02X\n", _value);

        _value == 0x0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        outBit == 0x0 ? cpuPtr->resetFlag(CPUFlags::c) : cpuPtr->setFlag(CPUFlags::c);
    }

    void ALU::rlca()
    {
        rlc(cpuPtr->AF.highByte());
        cpuPtr->resetFlag(CPUFlags::z);
    }

    void ALU::rrca()
    {
        rrc(cpuPtr->AF.highByte());
        cpuPtr->resetFlag(CPUFlags::z);
    }

    void ALU::rrc(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        rrc(data);
        _dstRegister = data;
    }

    void ALU::rrc(uint8_t &_value)
    {
        bool newCFlag = _value & 0x01;

        _value = (_value >> 1) | (newCFlag << 7);

        _value == 0 ? cpuPtr->setFlag(CPUFlags::z) : cpuPtr->resetFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        newCFlag ? cpuPtr->setFlag(CPUFlags::c) : cpuPtr->resetFlag(CPUFlags::c);
    }

    void ALU::complement()
    {
        cpuPtr->AF.highByte() = ~(cpuPtr->AF.highByte().read());

        cpuPtr->setFlag(CPUFlags::n);
        cpuPtr->setFlag(CPUFlags::h);
    }

    void ALU::complementCarryFlag()
    {
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->resetFlag(CPUFlags::h);
        cpuPtr->toggleFlag(CPUFlags::c);
    }

    void ALU::daa()
    {
        cpuPtr->resetFlag(CPUFlags::z);

        if (!cpuPtr->checkFlag(CPUFlags::n))
        {
            if (cpuPtr->checkFlag(CPUFlags::c) || cpuPtr->AF.highByte().read() > 0x99)
            {
                cpuPtr->AF.highByte() += 0x60;
                cpuPtr->setFlag(CPUFlags::c);
            }
            if (cpuPtr->checkFlag(CPUFlags::h) || (cpuPtr->AF.highByte().read() & 0x0F) > 0x09)
            {
                cpuPtr->AF.highByte() += 0x6;
            }
        }
        else
        {
            if (cpuPtr->checkFlag(CPUFlags::c))
            {
                cpuPtr->AF.highByte() -= 0x60;
            }
            if (cpuPtr->checkFlag(CPUFlags::h))
            {
                cpuPtr->AF.highByte() -= 0x06;
            }
        }
        if (cpuPtr->AF.highByte().read() == 0)
        {
            cpuPtr->setFlag(CPUFlags::z);
        }
        cpuPtr->resetFlag(CPUFlags::h);
    }

    void ALU::res(Register8 &_dstRegister, const uint8_t _bit)
    {
        uint8_t data = _dstRegister.read();
        res(data, _bit);
        _dstRegister = data;
    }

    void ALU::res(uint8_t &_value, const uint8_t _bit)
    {
        _value &= ~(1 << _bit);
    }

    void ALU::set(Register8 &_dstRegister, const uint8_t _bit)
    {
        uint8_t data = _dstRegister.read();
        set(data, _bit);
        _dstRegister = data;
    }

    void ALU::set(uint8_t &_value, const uint8_t _bit)
    {
        _value |= (1 << _bit);
    }

    void ALU::bit(Register8 &_dstRegister, const uint8_t _bit)
    {
        uint8_t data = _dstRegister.read();
        bit(data, _bit);
    }

    void ALU::bit(uint8_t &_value, const uint8_t _bit)
    {
        bool isBitSet = (_value >> _bit) & 0x01;

        isBitSet ? cpuPtr->resetFlag(CPUFlags::z) : cpuPtr->setFlag(CPUFlags::z);
        cpuPtr->resetFlag(CPUFlags::n);
        cpuPtr->setFlag(CPUFlags::h);
    }
}