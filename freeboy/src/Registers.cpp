//
// Created by ozgur on 6/11/2024.
//

#include "../include/Registers.h"

namespace gameboy
{
    namespace cpu_register
    {

        Register8::Register8(uint8_t _value) : value(_value) {}

        uint8_t Register8::operator++(int)
        {
            uint8_t temp = read();
            write(temp + 1);
            return temp;
        }

        uint8_t Register8::operator++()
        {
            uint8_t temp = read() + 1;
            write(temp);
            return temp;
        }

        uint8_t Register8::operator--(int) {
            uint8_t temp = read();
            write(temp - 1);
            return temp;        }

        uint8_t Register8::operator--() {
            uint8_t temp = read() - 1;
            write(temp);
            return temp;
        }

        void Register8::operator+=(const uint8_t _value)
        {
            uint8_t current = read();
            write(current + _value);
        }

        void Register8::operator^=(const Register8 &_register)
        {
            value ^= _register.read();
        }

        void Register8::operator|=(const Register8& _register)
        {
            value |= _register.read();
        }

        void Register8::operator&=(const uint16_t _value)
        {
            value &= _value;
        }

        void Register8::operator=(const Register8 &_register)
        {
            value = _register.read();
        }

        void Register8::operator-=(const uint8_t _value)
        {
            value -= _value;
        }

        void Register8::operator>>=(const uint8_t _offset)
        {
            value >>= _offset;
        }

        uint16_t Register8::operator&(const uint16_t _offset)
        {
            return value & _offset;
        }

        void Register8::operator=(const uint8_t &_value)
        {
            value = _value;
        }

        /* -------------------------------------------------------------------------- */

        Register16::Register16(uint16_t _value) :
            high{static_cast<uint8_t>(_value >> 8 & 0xFF)},
            low{static_cast<uint8_t>(_value & 0xFF)} {}

        void Register16::write(uint16_t _value)
        {
            uint8_t h = _value >> 8 & 0xFF;
            uint8_t l = _value & 0xFF;

            high.write(h);
            low.write(l);
        };

        uint16_t Register16::operator++(int)
        {
            uint16_t temp = read();
            write(temp + 1);
            return temp;
        }

        uint16_t Register16::operator++()
        {
            uint16_t temp = read() + 1;
            write(temp);
            return temp;
        }

        void Register16::operator+=(const uint8_t _value)
        {
            uint16_t data = read() + _value;
            write(data);
        }

        uint16_t Register16::operator--(int)
        {
            uint16_t temp = read();
            write(temp - 1);
            return temp;
        }

        uint16_t Register16::operator--()
        {
            uint16_t temp = read() - 1;
            write(temp);
            return temp;
        }

        void Register16::operator=(const Register16 &_register)
        {
            uint16_t _value = _register.read();
            write(_value);
        }

        void Register16::operator=(const uint16_t _value)
        {
            write(_value);
        }

        void Register16::operator-=(const uint8_t _value)
        {
            uint16_t temp = read() - _value;
            write(temp);
        }

        void Register16::operator+=(const Register16 _register)
        {
            uint16_t result = read() + _register.read();
            write(result);
        }
    }
}