//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_Info_H
#define FREEBOY_Info_H

#include <cstdint>
#include <array>

namespace gameboy::instruction
{
    class CPU;
    enum class AddressMode : uint8_t
    {
        IMP,
        R_D16,
        R_R,
        MR_R,
        R,
        R_D8,
        R_MR,
        R_HLI,
        R_HLD,
        HLI_R,
        HLD_R,
        R_A8,
        A8_R,
        HL_SPR,
        D16,
        D8,
        D16_R,
        MR_D8,
        MR,
        A16_R,
        R_A16
    };

    enum class RegisterType : uint8_t
    {
        NONE,
        A,
        F,
        B,
        C,
        D,
        E,
        H,
        L,
        AF,
        BC,
        DE,
        HL,
        SP,
        PC
    };

    enum class InstructionType : uint8_t
    {
        NONE,
        NOP,
        LD,
        INC,
        DEC,
        RLCA,
        ADD,
        RRCA,
        STOP,
        RLA,
        JR,
        RRA,
        DAA,
        CPL,
        SCF,
        CCF,
        HALT,
        ADC,
        SUB,
        SBC,
        AND,
        XOR,
        OR,
        CP,
        POP,
        JP,
        PUSH,
        RET,
        CB,
        CALL,
        RETI,
        LDH,
        JPHL,
        DI,
        EI,
        RST,
        ERR,
        //CB Infos...
        RLC,
        RRC,
        RL,
        RR,
        SLA,
        SRA,
        SWAP,
        SRL,
        BIT,
        RES,
        SET
    };

    enum class ConditionCode : uint8_t
    {
        NONE,   // Nothing
        NC,     // Execute if C is not set.
        C,      // Execute if C is set.
        NZ,     // Execute if Z is not set.
        Z       // Execute if Z is set.
    };

    class CPU_Instructions;

    struct InstructionHeader
    {
        uint8_t byteLength = 0u;
        uint8_t cycleCount = 0u; // CycleCount : Duration in T-states / 4 !!!
        void (CPU_Instructions::*addrFunction)(CPU* cpu);
        InstructionType InfoType = InstructionType::NONE;
        AddressMode addrMode = AddressMode::IMP;
        RegisterType dstRegister = RegisterType::NONE;
        RegisterType srcRegister = RegisterType::NONE;
        ConditionCode condition = ConditionCode::NONE;
        uint8_t param = 0u; // For RST Info
    };

    class CPU_Instructions
    {
    public:
        void NOP(CPU* _cpu);
        void LD(CPU* _cpu);
        void JP(CPU* _cpu);
        void XOR(CPU* _cpu);
        void DEC(CPU* _cpu);
    };

    static constexpr std::array<InstructionHeader, 0x100> STANDARD_INSTRUCTIONS
            {
                    // 0x0
                    InstructionHeader{1, 1, &CPU_Instructions::NOP, InstructionType::NOP, AddressMode::IMP}, // 0x00
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{1, 1, &CPU_Instructions::DEC, InstructionType::DEC, AddressMode::R, RegisterType::B}, // 0x05
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{2, 2, &CPU_Instructions::LD, InstructionType::LD, AddressMode::R_D8, RegisterType::C}, // 0x0E
                    InstructionHeader{},

                    // 0x1
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0x2
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0x3
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0x4
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0x5
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0x6
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0x7
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0x8
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0x9
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0xA
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{1, 1, &CPU_Instructions::XOR, InstructionType::XOR, AddressMode::R, RegisterType::A}, // 0xAF

                    // 0xB
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0xC
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{3, 4, &CPU_Instructions::JP, InstructionType::JP, AddressMode::D16}, // 0xC3
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0xD
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0xE
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},

                    // 0xF
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
                    InstructionHeader{},
            };

    static constexpr std::array<InstructionHeader, 0x100> EXTENDED_InfoS // CB Infos
            {
            };
}

#endif //FREEBOY_Info_H
