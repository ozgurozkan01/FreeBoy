//
// Created by ozgur on 4/15/2024.
//

#ifndef FREEBOY_REGISTER_H
#define FREEBOY_REGISTER_H

#include <cstdint>

namespace gameboy::registers
{
    // Registers in GB are 8 and 16-bit
    class Register8
    {
    public:
        Register8() = default;
        explicit Register8(uint8_t _value) : value(_value) {}
        inline void setValue(uint8_t _value) { value = _value; }
        inline uint8_t& getValue() { return value; }
    private:
        uint8_t value{};
    };


    class Register16
    {
    public:
        Register16() = default;
        [[maybe_unused]] Register16(uint8_t _high, uint8_t _low) : high(_high), low(_low) {}

        inline void setHigh(uint8_t _lowValue) { high.setValue(_lowValue); }
        inline void setLow(uint8_t _highValue) { low.setValue(_highValue); }

        inline uint8_t& getHigh() { return high.getValue(); }
        inline uint8_t& getLow() { return low.getValue(); }
    private:
        Register8 high;
        Register8 low;
    };
}

#endif //FREEBOY_REGISTER_H
