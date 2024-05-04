//
// Created by ozgur on 12/04/2024.
//

#include <string>
#include "../include/CPU.h"
#include "../include/Bus.h"
#include "../include/GameBoy.h"

namespace gameboy
{
    CPU::CPU(Bus* _bus, GameBoy* _gb) : busRef(_bus), gameBoyRef(_gb) {}

    bool CPU::init()
    {
        registers.A = 0x01;
        registers.F = 0xB0;
        registers.B = 0x00;
        registers.C = 0x13;
        registers.D = 0x00;
        registers.E = 0xD8;
        registers.H = 0x01;
        registers.L = 0x4D;
        registers.PC = 0x0100;
        registers.SP = 0xFFFE;

        isHalted = false;
        isStopped = false;
        return true;
    }

    void CPU::step()
    {
        if (!isHalted)
        {
            uint16_t pc = registers.PC;
            fetch();
            printf("Execution Instruction : %02X    PC : %04X\n", currentOpcode, pc);
            execute();
        }
    }

    void CPU::fetch()
    {
        currentOpcode = busRef->busRead(registers.PC++);
        currentInstraction = instruction::STANDART_INSTRUCTIONS[currentOpcode];
    }

    void CPU::execute()
    {
        switch (currentInstraction.addrMode)
        {
            case instruction::AddressMode::IMP:
                break;
            case instruction::AddressMode::R_D16:
                break;
            case instruction::AddressMode::R_R:
                break;
            case instruction::AddressMode::MR_R:
                break;
            case instruction::AddressMode::R:
                fetchedData = readRegister(currentInstraction.dstRegister);
                break;
            case instruction::AddressMode::R_D8:
                fetchedData = busRef->busRead(registers.PC);
                gameBoyRef->emulateCycles(1);
                registers.PC++;
                return;
            case instruction::AddressMode::R_MR:
                return;
            case instruction::AddressMode::R_HLI:
                return;
            case instruction::AddressMode::R_HLD:
                return;
            case instruction::AddressMode::HLI_R:
                return;
            case instruction::AddressMode::HLD_R:
                return;
            case instruction::AddressMode::R_A8:
                return;
            case instruction::AddressMode::A8_R:
                return;
            case instruction::AddressMode::HL_SPR:
                return;
            case instruction::AddressMode::D16:
            {
                uint16_t low = busRef->busRead(registers.PC);
                gameBoyRef->emulateCycles(1);
                uint16_t high = busRef->busRead(registers.PC + 1);
                gameBoyRef->emulateCycles(1);
                fetchedData = high << 8 | low;
                registers.PC += 2;
                return;
            }
            case instruction::AddressMode::D8:
                return;
            case instruction::AddressMode::D16_R:
                return;
            case instruction::AddressMode::MR_D8:
                return;
            case instruction::AddressMode::MR:
                return;
            case instruction::AddressMode::A16_R:
                return;
            case instruction::AddressMode::R_A16:
                return;
        }
    }

    uint16_t CPU::readRegister(instruction::RegisterType _register) const
    {
        switch (_register)
        {
            case instruction::RegisterType::NONE: return 0;
            case instruction::RegisterType::A: return registers.A;
            case instruction::RegisterType::F: return registers.F;
            case instruction::RegisterType::B: return registers.B;
            case instruction::RegisterType::C: return registers.C;
            case instruction::RegisterType::D: return registers.D;
            case instruction::RegisterType::E: return registers.E;
            case instruction::RegisterType::H: return registers.H;
            case instruction::RegisterType::L: return registers.L;
            case instruction::RegisterType::SP: return registers.SP;
            case instruction::RegisterType::PC: return registers.PC;
            case instruction::RegisterType::AF: return ((registers.A << 8) | registers.F);
            case instruction::RegisterType::BC: return ((registers.B << 8) | registers.C);
            case instruction::RegisterType::DE: return ((registers.D << 8) | registers.E);
            case instruction::RegisterType::HL: return ((registers.H << 8) | registers.L);
        }

        return 0;
    }

    void CPU::writeRegister(instruction::RegisterType _register, uint16_t _value)
    {
        switch (_register)
        {
            case instruction::RegisterType::NONE: return;
            case instruction::RegisterType::A: registers.A = _value & 0xFF; return;
            case instruction::RegisterType::F: registers.F = _value & 0xFF; return;
            case instruction::RegisterType::B: registers.B = _value & 0xFF; return;
            case instruction::RegisterType::C: registers.C = _value & 0xFF; return;
            case instruction::RegisterType::D: registers.D = _value & 0xFF; return;
            case instruction::RegisterType::E: registers.E = _value & 0xFF; return;
            case instruction::RegisterType::H: registers.H = _value & 0xFF; return;
            case instruction::RegisterType::L: registers.L = _value & 0xFF; return;
            case instruction::RegisterType::SP: registers.SP = _value; return;
            case instruction::RegisterType::PC: registers.PC = _value; return;
            case instruction::RegisterType::AF:
                registers.A = (_value >> 8 & 0xFF);
                registers.F = _value & 0xFF;
                return;
            case instruction::RegisterType::BC:
                registers.B = (_value >> 8 & 0xFF);
                registers.C = _value & 0xFF;
                return;
            case instruction::RegisterType::DE:
                registers.D = (_value >> 8 & 0xFF);
                registers.E = _value & 0xFF;
                return;
            case instruction::RegisterType::HL:
                registers.H = (_value >> 8 & 0xFF);
                registers.L = _value & 0xFF;
                return;
        }
    }
}
