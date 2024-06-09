//
// Created by ozgur on 6/8/2024.
//

#include "../include/CPURegisters.h"

namespace gameboy
{/*
    coreRegisters.A = 0x01;
    coreRegisters.F = 0xB0;
    coreRegisters.B = 0x00;
    coreRegisters.C = 0x13;
    coreRegisters.D = 0x00;
    coreRegisters.E = 0xD8;
    coreRegisters.H = 0x01;
    coreRegisters.L = 0x4D;
    coreRegisters.SP = 0xFFFE;
    coreRegisters.PC = 0x0100;*/
    namespace cpu_registers
    {
        CPURegisters::CPURegisters() :
        A(0x01),
        F(0xB0),
        B(0x00),
        C(0x13),
        D(0x00),
        E(0x08),
        H(0x01),
        L(0x40),
        SP(0xFFFE),
        PC(0x0100) {}


        uint16_t CPURegisters::readRegister(RegisterType _register) const
        {
            switch (_register)
            {
                case RegisterType::NONE: return 0;
                case RegisterType::A: return A;
                case RegisterType::F: return F;
                case RegisterType::B: return B;
                case RegisterType::C: return C;
                case RegisterType::D: return D;
                case RegisterType::E: return E;
                case RegisterType::H: return H;
                case RegisterType::L: return L;
                case RegisterType::SP: return SP;
                case RegisterType::PC: return PC;
                case RegisterType::AF: return  A << 8 | F;;
                case RegisterType::BC: return B << 8 |C;
                case RegisterType::DE: return D << 8 | E;
                case RegisterType::HL: return H << 8 | L;
            }

            return 0;
        }

        void CPURegisters::writeRegister(RegisterType _register, uint16_t _value) {
            switch (_register) {
                case RegisterType::NONE:
                    return;
                case RegisterType::A:
                    A = _value & 0xFF;
                    return;
                case RegisterType::F:
                    F = _value & 0xFF;
                    return;
                case RegisterType::B:
                    B = _value & 0xFF;
                    return;
                case RegisterType::C:
                    C = _value & 0xFF;
                    return;
                case RegisterType::D:
                    D = _value & 0xFF;
                    return;
                case RegisterType::E:
                    E = _value & 0xFF;
                    return;
                case RegisterType::H:
                    H = _value & 0xFF;
                    return;
                case RegisterType::L:
                    L = _value & 0xFF ;
                    return;
                case RegisterType::SP:
                    SP = _value ;
                    return;
                case RegisterType::PC:
                    PC = _value;
                    return;
                case RegisterType::AF:
                    A = _value >> 8 & 0xFF;
                    F = _value & 0xFF;
                    return;
                case RegisterType::BC:
                    B = _value >> 8 & 0xFF;
                    C = _value & 0xFF;
                    return;
                case RegisterType::DE:
                    D = _value >> 8 & 0xFF;
                    E = _value & 0xFF;
                    return;
                case RegisterType::HL:
                    H = _value >> 8 & 0xFF;
                    L = _value & 0xFF;
                    return;
            }
        }
    }
};
