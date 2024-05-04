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
        coreRegisters.A = 0x01;
        coreRegisters.F = 0xB0;
        coreRegisters.B = 0x00;
        coreRegisters.C = 0x13;
        coreRegisters.D = 0x00;
        coreRegisters.E = 0xD8;
        coreRegisters.H = 0x01;
        coreRegisters.L = 0x4D;
        coreRegisters.PC = 0x0100;
        coreRegisters.SP = 0xFFFE;

        isHalted = false;
        isStopped = false;
        return true;
    }

    void CPU::step()
    {
        if (!isHalted)
        {
            uint16_t pc = coreRegisters.PC;
            fetch();
            printf("Execution Instruction : %02X    PC : %04X\n", currentOpcode, pc);
            execute();
        }
    }

    void CPU::fetch()
    {
        currentOpcode = busRef->busRead(coreRegisters.PC++);
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
                fetchedData = busRef->busRead(coreRegisters.PC);
                gameBoyRef->emulateCycles(1);
                coreRegisters.PC++;
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
                uint16_t low = busRef->busRead(coreRegisters.PC);
                gameBoyRef->emulateCycles(1);
                uint16_t high = busRef->busRead(coreRegisters.PC + 1);
                gameBoyRef->emulateCycles(1);
                fetchedData = high << 8 | low;
                coreRegisters.PC += 2;
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
            case instruction::RegisterType::A: return coreRegisters.A;
            case instruction::RegisterType::F: return coreRegisters.F;
            case instruction::RegisterType::B: return coreRegisters.B;
            case instruction::RegisterType::C: return coreRegisters.C;
            case instruction::RegisterType::D: return coreRegisters.D;
            case instruction::RegisterType::E: return coreRegisters.E;
            case instruction::RegisterType::H: return coreRegisters.H;
            case instruction::RegisterType::L: return coreRegisters.L;
            case instruction::RegisterType::SP: return coreRegisters.SP;
            case instruction::RegisterType::PC: return coreRegisters.PC;
            case instruction::RegisterType::AF: return ((coreRegisters.A << 8) | coreRegisters.F);
            case instruction::RegisterType::BC: return ((coreRegisters.B << 8) | coreRegisters.C);
            case instruction::RegisterType::DE: return ((coreRegisters.D << 8) | coreRegisters.E);
            case instruction::RegisterType::HL: return ((coreRegisters.H << 8) | coreRegisters.L);
        }
        return 0;
    }

    void CPU::writeRegister(instruction::RegisterType _register, uint16_t _value)
    {
        switch (_register)
        {
            case instruction::RegisterType::NONE: return;
            case instruction::RegisterType::A: coreRegisters.A = _value & 0xFF; return;
            case instruction::RegisterType::F: coreRegisters.F = _value & 0xFF; return;
            case instruction::RegisterType::B: coreRegisters.B = _value & 0xFF; return;
            case instruction::RegisterType::C: coreRegisters.C = _value & 0xFF; return;
            case instruction::RegisterType::D: coreRegisters.D = _value & 0xFF; return;
            case instruction::RegisterType::E: coreRegisters.E = _value & 0xFF; return;
            case instruction::RegisterType::H: coreRegisters.H = _value & 0xFF; return;
            case instruction::RegisterType::L: coreRegisters.L = _value & 0xFF; return;
            case instruction::RegisterType::SP: coreRegisters.SP = _value; return;
            case instruction::RegisterType::PC: coreRegisters.PC = _value; return;
            case instruction::RegisterType::AF:
                coreRegisters.A = (_value >> 8 & 0xFF);
                coreRegisters.F = _value & 0xFF;
                return;
            case instruction::RegisterType::BC:
                coreRegisters.B = (_value >> 8 & 0xFF);
                coreRegisters.C = _value & 0xFF;
                return;
            case instruction::RegisterType::DE:
                coreRegisters.D = (_value >> 8 & 0xFF);
                coreRegisters.E = _value & 0xFF;
                return;
            case instruction::RegisterType::HL:
                coreRegisters.H = (_value >> 8 & 0xFF);
                coreRegisters.L = _value & 0xFF;
                return;
        }
    }
}
