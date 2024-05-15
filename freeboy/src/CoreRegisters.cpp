//
// Created by ozgur on 5/14/2024.
//

#include "../include/CoreRegisters.h"

namespace gameboy
{
    namespace registers
    {
        // REGISTER 8 BIT
        uint8_t Register8::getData() const { return data; }
        void Register8::setData(uint8_t _data) { data = _data; }
        // REGISTER 16 BIT
        [[maybe_unused]] uint8_t Register16::lowValue() const { return low.getData(); }
        [[maybe_unused]] uint8_t Register16::highValue() const { return high.getData();}
        [[maybe_unused]] uint16_t Register16::getData() const { return high.getData() << 8 | low.getData(); }
        [[maybe_unused]] const Register8 &Register16::lowRegister() const { return low; }
        [[maybe_unused]] const Register8 &Register16::highRegister() const { return high; }

        void Register16::setData(uint16_t _data) {  high.setData(_data >> 8 & 0xFF); low.setData(_data & 0xFF); }
    }
}
