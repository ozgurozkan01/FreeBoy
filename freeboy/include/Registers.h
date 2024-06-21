//
// Created by ozgur on 6/11/2024.
//

#ifndef FREEBOY_REGISTERS_H
#define FREEBOY_REGISTERS_H

#include <cstdint>

namespace gameboy
{
    namespace cpu_register
    {
        class Register8{
        public:
            Register8() = default;
            Register8(uint8_t _value);

            constexpr uint8_t read() const { return value; };
            constexpr void write(const uint8_t _value) { value = _value; };

            uint8_t operator++(int); // Post-Increment
            uint8_t operator++(); // Pre-Increment

            uint8_t operator--(int); // Post-Increment
            uint8_t operator--(); // Pre-Increment

            uint16_t operator&(const uint16_t _offset);

            void operator+=(const uint8_t _value);
            void operator-=(const uint8_t _value);
            void operator=(const Register8& _register);

            void operator^=(const Register8& _register);
            void operator|=(const Register8& _register);
            void operator&=(const uint16_t _value);

            void operator>>=(const uint8_t _offset);
        private:
            uint8_t value;
        };

        /* -------------------------------------------------------------------------- */

        class Register16{
        public:
            Register16() = default;
            Register16(uint16_t _value);

            Register8& highByte() { return high; };
            Register8& lowByte() { return low; };

            uint16_t read() const { return (high.read() << 8) | low.read(); };
            void write(uint16_t _value);

            uint16_t operator++(int); // Post-Increment
            uint16_t operator++(); // Pre-Increment

            uint16_t operator--(int); // Post-Increment
            uint16_t operator--(); // Pre-Increment

            void operator+=(const Register16 _register);

            void operator+=(const uint8_t _value);
            void operator-=(const uint8_t _value);
            void operator=(const Register16& _register);
            void operator=(const uint16_t _value);
        private:
            Register8 high;
            Register8 low;
        };
    }
}

#endif //FREEBOY_REGISTERS_H
