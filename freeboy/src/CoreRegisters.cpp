//
// Created by ozgur on 5/14/2024.
//

#include "../include/CoreRegisters.h"

namespace gameboy
{
    namespace registers
    {
        uint8_t CoreRegisters::getFlagZ() const { return (F & static_cast<uint8_t>(registers::FlagType::ZERO_FLAG)); }
        uint8_t CoreRegisters::getFlagN() const { return (F & static_cast<uint8_t>(registers::FlagType::SUBTRACT_FLAG)); }
        uint8_t CoreRegisters::getFlagH() const { return (F & static_cast<uint8_t>(registers::FlagType::HALF_CARRY_FLAG)); }
        uint8_t CoreRegisters::getFlagC() const { return (F & static_cast<uint8_t>(registers::FlagType::CARRY_FLAG)); }

        uint16_t CoreRegisters::getAF() const { return A << 8 | F; }
        uint16_t CoreRegisters::getBC() const { return B << 8 | C; }
        uint16_t CoreRegisters::getDE() const { return D << 8 | E; }
        uint16_t CoreRegisters::getHL() const { return H << 8 | L; }
        uint16_t CoreRegisters::getSP() const { return SP; }
        uint16_t CoreRegisters::getPC() const { return PC; }

        uint8_t CoreRegisters::getA() const { return A; }
        uint8_t CoreRegisters::getF() const { return F; }
        uint8_t CoreRegisters::getB() const { return B; }
        uint8_t CoreRegisters::getC() const { return C; }
        uint8_t CoreRegisters::getD() const { return D; }
        uint8_t CoreRegisters::getE() const { return E; }
        uint8_t CoreRegisters::getH() const { return H; }
        uint8_t CoreRegisters::getL() const { return L; }

        void CoreRegisters::setA(const uint8_t _data) { A = _data; }
        void CoreRegisters::setF(const uint8_t _data) { F = _data; }
        void CoreRegisters::setB(const uint8_t _data) { B = _data; }
        void CoreRegisters::setC(const uint8_t _data) { C = _data; }
        void CoreRegisters::setD(const uint8_t _data) { D = _data; }
        void CoreRegisters::setE(const uint8_t _data) { E = _data; }
        void CoreRegisters::setH(const uint8_t _data) { H = _data; }
        void CoreRegisters::setL(const uint8_t _data) { L = _data; }
        void CoreRegisters::setPC(const uint16_t _data) { PC = _data; }
        void CoreRegisters::setSP(const uint16_t _data) { SP = _data; }

        void CoreRegisters::increasePC(uint16_t _data) { PC += _data; }
        void CoreRegisters::increaseSP(uint16_t _data) { SP += _data; }
    }
}
