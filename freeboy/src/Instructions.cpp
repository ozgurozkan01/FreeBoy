//
// Created by ozgur on 12/04/2024.
//

#include "../include/Instructions.h"
#include "../include/CPU.h"
#include <stdio.h>

namespace gameboy
{
    namespace instruction
    {
        Instructions::Instructions(CPU *_cpu) : cpuRef(_cpu)
        {
            standardInstructions =
                    {
                            // 0x0
                            Info{1, 4, &CPU::nop, &CPU::addr_IMP}, // 0x00
                            Info{3, 14, &CPU::ld, &CPU::addr_R_D16, RegisterType::BC}, // 0X01
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::BC, RegisterType::A}, // 0X02
                            Info{}, // 0x03
                            Info{}, // 0x04
                            Info{1, 4, &CPU::dec, &CPU::addr_R, RegisterType::B}, // 0x05
                            Info{2, 8, &CPU::ld, &CPU::addr_R_D8, RegisterType::B}, // 0x06
                            Info{}, // 0X07
                            Info{3, 20, &CPU::ld, &CPU::addr_A16_R, RegisterType::NONE, RegisterType::SP}, // 0X08
                            Info{}, // 0x09
                            Info{1, 8, &CPU::ld, &CPU::addr_R_MR, RegisterType::A, RegisterType::BC}, // 0x0A
                            Info{}, // 0x0B
                            Info{}, // 0x0C
                            Info{}, // 0x0D
                            Info{2, 8, &CPU::ld, &CPU::addr_R_D8, RegisterType::C}, // 0x0E
                            Info{}, // 0x0F

                            // 0x1
                            Info{}, // 0x10
                            Info{3, 12, &CPU::ld, &CPU::addr_R_D16, RegisterType::DE}, // 0x11
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::DE, RegisterType::A}, // 0x12
                            Info{}, // 0x13
                            Info{}, // 0x14
                            Info{1, 4, &CPU::dec, &CPU::addr_R, RegisterType::D}, // 0x15
                            Info{2, 8, &CPU::ld, &CPU::addr_R_D8, RegisterType::D}, // 0x16
                            Info{}, // 0x17
                            Info{2, 12, &CPU::jr, &CPU::addr_D8}, // 0x18
                            Info{}, // 0x19
                            Info{1, 8, &CPU::ld, &CPU::addr_R_MR, RegisterType::A, RegisterType::DE}, // 0x1A
                            Info{}, // 0x1B
                            Info{}, // 0x1C
                            Info{}, // 0x1D
                            Info{2, 8, &CPU::ld, &CPU::addr_R_D8, RegisterType::E}, // 0x1E
                            Info{}, // 0x1F

                            // 0x2
                            Info{2, 12, &CPU::jr, &CPU::addr_D8, RegisterType::NONE, RegisterType::NONE, ConditionCode::NZ}, // 0x20
                            Info{3, 12, &CPU::ld, &CPU::addr_R_D16, RegisterType::HL}, // 0x21
                            Info{1, 8, &CPU::ld, &CPU::addr_HLI_R, RegisterType::HL, RegisterType::A}, // 0x22
                            Info{}, // 0x23
                            Info{}, // 0x24
                            Info{1, 4, &CPU::dec, &CPU::addr_R, RegisterType::H}, // 0x25
                            Info{2, 8, &CPU::ld, &CPU::addr_R_D8, RegisterType::H}, // 0x26
                            Info{}, // 0x27
                            Info{2, 12, &CPU::jr, &CPU::addr_D8, RegisterType::NONE, RegisterType::NONE, ConditionCode::Z}, // 0x28
                            Info{}, // 0x29
                            Info{1, 8, &CPU::ld, &CPU::addr_R_HLI, RegisterType::A, RegisterType::HL}, // 0x2A
                            Info{}, // 0x2B
                            Info{}, // 0x2C
                            Info{}, // 0x2D
                            Info{2, 8, &CPU::ld, &CPU::addr_R_D8, RegisterType::L}, // 0x2E
                            Info{}, // 0x2F

                            // 0x3
                            Info{2, 12, &CPU::jr, &CPU::addr_D8, RegisterType::NONE, RegisterType::NONE, ConditionCode::NC}, // 0x30
                            Info{3, 12, &CPU::ld, &CPU::addr_R_D16, RegisterType::SP}, // 0x31
                            Info{1, 8, &CPU::ld, &CPU::addr_HLD_R, RegisterType::HL, RegisterType::A}, // 0x32
                            Info{}, // 0x33
                            Info{}, // 0x34
                            Info{1, 12, &CPU::dec, &CPU::addr_R, RegisterType::HL}, // 0x35
                            Info{2, 12, &CPU::ld, &CPU::addr_MR_D8, RegisterType::HL}, // 0x36
                            Info{}, // 0x37
                            Info{2, 12, &CPU::jr, &CPU::addr_D8, RegisterType::NONE, RegisterType::NONE, ConditionCode::C}, // 0x38
                            Info{}, // 0x39
                            Info{1, 8, &CPU::ld, &CPU::addr_R_HLD, RegisterType::A, RegisterType::HL}, // 0x3A
                            Info{}, // 0x3B
                            Info{}, // 0x3C
                            Info{}, // 0x3D
                            Info{2, 8, &CPU::ld, &CPU::addr_R_D8, RegisterType::A}, // 0x3E
                            Info{}, // 0x3F

                            // 0x4
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::B, RegisterType::B}, // 0x40
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::B, RegisterType::C}, // 0x41
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::B, RegisterType::D}, // 0x42
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::B, RegisterType::E}, // 0x43
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::B, RegisterType::H}, // 0x44
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::B, RegisterType::L}, // 0x45
                            Info{1, 8, &CPU::ld, &CPU::addr_R_MR, RegisterType::B, RegisterType::HL}, // 0x46
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::B, RegisterType::A}, // 0x47
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::C, RegisterType::B}, // 0x48
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::C, RegisterType::C}, // 0x49
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::C, RegisterType::D}, // 0x4A
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::C, RegisterType::E}, // 0x4B
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::C, RegisterType::H}, // 0x4C
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::C, RegisterType::L}, // 0x4D
                            Info{1, 8, &CPU::ld, &CPU::addr_R_MR, RegisterType::C, RegisterType::HL}, // 0x4E
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::C, RegisterType::A}, // 0x4F

                            // 0x5
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::D, RegisterType::B}, // 0x50
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::D, RegisterType::C}, // 0x51
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::D, RegisterType::D}, // 0x52
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::D, RegisterType::E}, // 0x53
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::D, RegisterType::H}, // 0x54
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::D, RegisterType::L}, // 0x55
                            Info{1, 8, &CPU::ld, &CPU::addr_R_MR, RegisterType::D, RegisterType::HL}, // 0x56
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::D, RegisterType::A}, // 0x57
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::E, RegisterType::B}, // 0x58
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::E, RegisterType::C}, // 0x59
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::E, RegisterType::D}, // 0x5A
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::E, RegisterType::E}, // 0x5B
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::E, RegisterType::H}, // 0x5C
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::E, RegisterType::L}, // 0x5D
                            Info{1, 8, &CPU::ld, &CPU::addr_R_MR, RegisterType::E, RegisterType::HL}, // 0x5E
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::E, RegisterType::A}, // 0x5F

                            // 0x6
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::H, RegisterType::B}, // 0x60
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::H, RegisterType::C}, // 0x61
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::H, RegisterType::D}, // 0x62
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::H, RegisterType::E}, // 0x63
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::H, RegisterType::H}, // 0x64
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::H, RegisterType::L}, // 0x65
                            Info{1, 8, &CPU::ld, &CPU::addr_R_MR, RegisterType::H, RegisterType::HL}, // 0x66
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::H, RegisterType::A}, // 0x67
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::L, RegisterType::B}, // 0x68
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::L, RegisterType::C}, // 0x69
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::L, RegisterType::D}, // 0x6A
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::L, RegisterType::E}, // 0x6B
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::L, RegisterType::H}, // 0x6C
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::L, RegisterType::L}, // 0x6D
                            Info{1, 8, &CPU::ld, &CPU::addr_R_MR, RegisterType::L, RegisterType::HL}, // 0x6E
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::L, RegisterType::A}, // 0x6F

                            // 0x7
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::HL, RegisterType::B}, // 0x70
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::HL, RegisterType::C}, // 0x71
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::HL, RegisterType::D}, // 0x72
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::HL, RegisterType::E}, // 0x73
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::HL, RegisterType::H}, // 0x74
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::HL, RegisterType::L}, // 0x75
                            Info{1, 4, &CPU::halt, &CPU::addr_IMP}, // 0x76
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::HL, RegisterType::A}, // 0x77
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::A, RegisterType::B}, // 0x78
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::A, RegisterType::C}, // 0x79
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::A, RegisterType::D}, // 0x7A
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::A, RegisterType::E}, // 0x7B
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::A, RegisterType::H}, // 0x7C
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::A, RegisterType::L}, // 0x7D
                            Info{1, 4, &CPU::ld, &CPU::addr_R_MR, RegisterType::A, RegisterType::HL}, // 0x7E
                            Info{1, 4, &CPU::ld, &CPU::addr_R_R, RegisterType::A, RegisterType::A}, // 0x7F

                            // 0x8
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},

                            // 0x9
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},

                            // 0xA
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{1, 4, &CPU::alu_xor, &CPU::addr_R, RegisterType::A}, // 0xAF

                            // 0xB
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},
                            Info{},

                            // 0xC
                            Info{1, 20, &CPU::ret, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NZ},
                            Info{1, 12, &CPU::pop, &CPU::addr_IMP, RegisterType::BC},
                            Info{3, 16, &CPU::jp, &CPU::addr_D16, RegisterType::NONE, RegisterType::NONE, ConditionCode::NZ},
                            Info{3, 16, &CPU::jp, &CPU::addr_D16}, // 0xC3
                            Info{3, 24, &CPU::call, &CPU::addr_D16, RegisterType::NONE, RegisterType::NONE, ConditionCode::NZ},
                            Info{1, 16, &CPU::push, &CPU::addr_IMP, RegisterType::BC},
                            Info{},
                            Info{1, 16, &CPU::rst, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NONE, 0x00},
                            Info{1, 20, &CPU::ret, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::Z},
                            Info{1, 16, &CPU::ret, &CPU::addr_IMP},
                            Info{3, 16, &CPU::jp, &CPU::addr_D16, RegisterType::NONE, RegisterType::NONE, ConditionCode::Z},
                            Info{},
                            Info{3, 24, &CPU::call, &CPU::addr_D16, RegisterType::NONE, RegisterType::NONE, ConditionCode::Z},
                            Info{3, 24, &CPU::call, &CPU::addr_D16},
                            Info{},
                            Info{1, 16, &CPU::rst, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NONE, 0x08},

                            // 0xD
                            Info{1, 20, &CPU::ret, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NC},
                            Info{1, 12, &CPU::pop, &CPU::addr_IMP, RegisterType::DE},
                            Info{3, 16, &CPU::jp, &CPU::addr_D16, RegisterType::NONE, RegisterType::NONE, ConditionCode::NC},
                            Info{},
                            Info{3, 24, &CPU::call, &CPU::addr_D16, RegisterType::NONE, RegisterType::NONE, ConditionCode::NC},
                            Info{1, 16, &CPU::push, &CPU::addr_IMP, RegisterType::DE},
                            Info{},
                            Info{1, 16, &CPU::rst, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NONE, 0x10},
                            Info{1, 20, &CPU::ret, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::C},
                            Info{1, 16, &CPU::reti, &CPU::addr_IMP},
                            Info{3, 16, &CPU::jp, &CPU::addr_D16, RegisterType::NONE, RegisterType::NONE, ConditionCode::C},
                            Info{},
                            Info{3, 24, &CPU::call, &CPU::addr_D16, RegisterType::NONE, RegisterType::NONE, ConditionCode::C},
                            Info{},
                            Info{},
                            Info{1, 16, &CPU::rst, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NONE, 0x18},

                            // 0xE
                            Info{2, 12, &CPU::ldh, &CPU::addr_A8_R, RegisterType::NONE, RegisterType::A}, // 0xE0
                            Info{1, 12, &CPU::pop, &CPU::addr_IMP, RegisterType::HL}, // 0xE1
                            Info{1, 8, &CPU::ld, &CPU::addr_MR_R, RegisterType::C, RegisterType::A}, // 0xE2
                            Info{}, // 0xE3
                            Info{}, // 0xE4
                            Info{1, 16, &CPU::push, &CPU::addr_IMP, RegisterType::HL}, // 0xE5
                            Info{}, // 0xE6
                            Info{1, 16, &CPU::rst, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NONE, 0x20}, // 0xE7
                            Info{}, // 0xE8
                            Info{1, 4, &CPU::jp, &CPU::addr_MR, RegisterType::HL}, // 0xE9
                            Info{3, 16, &CPU::ld, &CPU::addr_A16_R,  RegisterType::NONE, RegisterType::A}, // 0xEA
                            Info{}, // 0xEB
                            Info{}, // 0xEC
                            Info{}, // 0xED
                            Info{}, // 0xEE
                            Info{1, 16, &CPU::rst, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NONE, 0x28}, // 0xEF

                            // 0xF
                            Info{2, 12, &CPU::ldh, &CPU::addr_R_A8, RegisterType::A}, // 0xF0
                            Info{1, 12, &CPU::pop, &CPU::addr_IMP, RegisterType::AF}, // 0xF1
                            Info{1, 8, &CPU::ld, &CPU::addr_R_MR, RegisterType::A, RegisterType::C}, // 0xF2
                            Info{1, 4, &CPU::di, &CPU::addr_IMP}, // 0xF3
                            Info{}, // 0xF4
                            Info{1, 16, &CPU::push, &CPU::addr_IMP, RegisterType::AF}, // 0xF5
                            Info{}, // 0xF6
                            Info{1, 16, &CPU::rst, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NONE, 0x30}, // 0xF7
                            Info{}, // 0xF8
                            Info{}, // 0xF9
                            Info{1, 8, &CPU::ld, &CPU::addr_R_A16, RegisterType::A}, // 0xFA
                            Info{}, // 0xFB
                            Info{}, // 0xFC
                            Info{}, // 0xFD
                            Info{}, // 0xFE
                            Info{1, 16, &CPU::rst, &CPU::addr_IMP, RegisterType::NONE, RegisterType::NONE, ConditionCode::NONE, 0x38}, // 0xFF
                    };

            cbInstructions = {};
        }

        bool Instructions::init()
        {
            if (cpuRef == nullptr)
            {
                printf("CPU Ref could not be initialized in Instruction!!\n");
                return false;
            }

            return true;
        }
    }
}
