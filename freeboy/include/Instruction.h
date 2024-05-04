//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_INSTRUCTION_H
#define FREEBOY_INSTRUCTION_H

#include <cstdint>
#include <string_view>
#include <array>

namespace gameboy::instruction
{
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
        //CB instructions...
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

    enum class Condition : uint8_t
    {
        NONE,
        NC,
        C,
        NZ,
        Z
    };

    struct Instruction
    {
        uint8_t length = 0u;
        uint8_t cycleCount = 0u;
        InstructionType instructionType = InstructionType::NONE;
        AddressMode addrMode = AddressMode::IMP;
        RegisterType dstRegister = RegisterType::NONE;
        RegisterType srcRegister = RegisterType::NONE;
        Condition condition = Condition::NONE;
        uint8_t param = 0u; // For RST instruction
    };

    static constexpr std::array<Instruction, 0x100> STANDART_INSTRUCTIONS
            {
                    // 0x0
                    Instruction{1, 4, InstructionType::NOP, AddressMode::IMP}, // 0x00
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{1, 4, InstructionType::DEC, AddressMode::R, RegisterType::B}, // 0x05
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{2, 8, InstructionType::LD, AddressMode::R_D8, RegisterType::C}, // 0x0E
                    Instruction{},

                    // 0x1
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0x2
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0x3
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0x4
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0x5
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0x6
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0x7
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0x8
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0x9
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0xA
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{1, 4, InstructionType::XOR, AddressMode::R, RegisterType::A}, // 0xAF

                    // 0xB
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0xC
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{3, 16, InstructionType::JP, AddressMode::D16}, // 0xC3
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0xD
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0xE
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},

                    // 0xF
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
                    Instruction{},
            };

    static constexpr std::array<Instruction, 0x100> EXTENDED_INSTRUCTIONS // CB instructions
            {
            };
}

#endif //FREEBOY_INSTRUCTION_H
