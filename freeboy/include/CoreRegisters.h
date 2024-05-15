//
// Created by ozgur on 5/14/2024.
//

#ifndef FREEBOY_COREREGISTERS_H
#define FREEBOY_COREREGISTERS_H

#include <cstdint>

namespace gameboy
{

    namespace registers
    {
        // The CPU flags are used to get information about the results of arithmetic and logical operations,
        // and to activate conditional branching
        enum class FlagType : uint8_t
        {
            // LOWER 8 BITS OF AF
            NONE = 0x0,
            ZERO_FLAG = 1 << 7,
            SUBTRACT_FLAG = 1 << 6, // (BCD)
            HALF_CARRY_FLAG = 1 << 5, // (BCD)
            CARRY_FLAG = 1 << 4
        };


        class Register8
        {
        public:
            Register8() = default;

            // GETTERS
            [[maybe_unused]] [[nodiscard]] uint8_t getData() const;
            // SETTERS
            [[maybe_unused]] void setData(uint8_t _data);

        private:
            uint8_t data;
        };

        class Register16
        {
        public:
            Register16() = default;

            // GETTERS
            [[maybe_unused]] [[nodiscard]] uint8_t lowValue() const;
            [[maybe_unused]] [[nodiscard]] uint8_t highValue() const;
            [[maybe_unused]] [[nodiscard]] uint16_t getData() const;
            [[maybe_unused]] [[nodiscard]] const Register8& lowRegister() const;
            [[maybe_unused]] [[nodiscard]] const Register8& highRegister() const;
            // SETTERS
            [[maybe_unused]] void setData(uint16_t _data);
        private:
            Register8 high;
            Register8 low;
        };
    }
}

#endif //FREEBOY_COREREGISTERS_H
