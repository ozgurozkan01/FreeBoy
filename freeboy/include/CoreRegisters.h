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

        class CoreRegisters
        {
        public:
            CoreRegisters() = default;
            [[maybe_unused]] uint8_t getFlagZ() const;
            [[maybe_unused]] uint8_t getFlagN() const;
            [[maybe_unused]] uint8_t getFlagH() const;
            [[maybe_unused]] uint8_t getFlagC() const;

            uint16_t getAF() const;
            uint16_t getBC() const;
            uint16_t getDE() const;
            uint16_t getHL() const;
            uint16_t getSP() const;
            uint16_t getPC() const;

            uint8_t getA() const;
            uint8_t getF() const;
            uint8_t getB() const;
            uint8_t getC() const;
            uint8_t getD() const;
            uint8_t getE() const;
            uint8_t getH() const;
            uint8_t getL() const;

            void setA(const uint8_t _data);
            void setF(const uint8_t _data);
            void setB(const uint8_t _data);
            void setC(const uint8_t _data);
            void setD(const uint8_t _data);
            void setE(const uint8_t _data);
            void setH(const uint8_t _data);
            void setL(const uint8_t _data);
            void setPC(const uint16_t _data);
            void setSP(const uint16_t _data);

            void increasePC(uint16_t _data);
            void increaseSP(uint16_t _data);
        private:
            uint8_t A;
            uint8_t F;
            uint8_t B;
            uint8_t C;
            uint8_t D;
            uint8_t E;
            uint8_t H;
            uint8_t L;
            uint16_t PC;
            uint16_t SP;
        };
    }
}

#endif //FREEBOY_COREREGISTERS_H
