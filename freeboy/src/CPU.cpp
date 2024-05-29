//
// Created by ozgur on 12/04/2024.
//

#include "../include/CPU.h"
#include "../include/Bus.h"
#include "../include/GameBoy.h"
#include "../include/InterruptHandler.h"
#include <string>

namespace gameboy
{
    CPU::CPU(Bus* _bus, GameBoy* _gb) : busRef(_bus), gameBoyRef(_gb) {}

    bool CPU::init()
    {
        interruptHandler = new InterruptHandler();
        cpuProcess = new Instructions(this);

        if (interruptHandler == nullptr)
        {
            printf("Interrupt handler could not be initialized!\n");
            return false;
        }
        
        if (!cpuProcess->init())
        {
            printf("CPU Proc could not be initialized!\n");
            return false;
        }

        if (busRef == nullptr)
        {
            printf("BusRef could not be initialized!\n");
            return false;
        }

        if (gameBoyRef == nullptr)
        {
            printf("GameBoyRef could not be initialized!\n");
            return false;
        }

        coreRegisters.A = 0x01;
        coreRegisters.F = 0xB0;
        coreRegisters.B = 0x00;
        coreRegisters.C = 0x13;
        coreRegisters.D = 0x00;
        coreRegisters.E = 0xD8;
        coreRegisters.H = 0x01;
        coreRegisters.L = 0x4D;
        coreRegisters.SP = 0xFFFE;
        coreRegisters.PC = 0x0100;

        isHalted = false;
        isStopped = false;
        isDestMem = false;

        return true;
    }

    void CPU::step()
    {
        if (!isHalted)
        {
            fetch();
            decode();
        }
    }

    void CPU::emulateCycles(uint8_t cycleCount) { gameBoyRef->emulateCycles(cycleCount); }

    void CPU::fetch()
    {
        currentOpcode = busRef->read8(coreRegisters.PC++);
        currentInstruction = &cpuProcess->standardInstructions[currentOpcode];
    }

    void CPU::decode()
    {
        memoryDestination = 0;
        isDestMem = false;

        // Call function pointer

        if (currentInstruction != nullptr && currentInstruction->operation != nullptr && currentInstruction->addrMode != nullptr)
        {
            uint16_t pc = coreRegisters.PC - 1;
            printf("%04X: (%02X %02X %02X) A: %02X B: %02X C: %02X\n",
                   pc, currentOpcode,
                   busRef->read8(pc + 1), busRef->read8(pc + 2),
                   coreRegisters.A, coreRegisters.B, coreRegisters.C);

            (this->*currentInstruction->addrMode)();
            (this->*currentInstruction->operation)();
            emulateCycles(currentInstruction->tCycles);
        }

        else
        {
            printf("Current Instruction NULL!");
            exit(-1);
        }
    }

    uint16_t CPU::readRegister(RegisterType _register) const
    {
        switch (_register)
        {
            case RegisterType::NONE: return 0;
            case RegisterType::A: return coreRegisters.A;
            case RegisterType::F: return coreRegisters.F;
            case RegisterType::B: return coreRegisters.B;
            case RegisterType::C: return coreRegisters.C;
            case RegisterType::D: return coreRegisters.D;
            case RegisterType::E: return coreRegisters.E;
            case RegisterType::H: return coreRegisters.H;
            case RegisterType::L: return coreRegisters.L;
            case RegisterType::SP: return coreRegisters.SP;
            case RegisterType::PC: return coreRegisters.PC;
            case RegisterType::AF: return  coreRegisters.A << 8 | coreRegisters.F;;
            case RegisterType::BC: return coreRegisters.B << 8 |coreRegisters.C;
            case RegisterType::DE: return coreRegisters.D << 8 | coreRegisters.E;
            case RegisterType::HL: return coreRegisters.H << 8 | coreRegisters.L;
        }

        return 0;
    }

    void CPU::writeRegister(RegisterType _register, uint16_t _value) {
        switch (_register) {
            case RegisterType::NONE:
                return;
            case RegisterType::A:
                coreRegisters.A = _value & 0xFF;
                return;
            case RegisterType::F:
                coreRegisters.F = _value & 0xFF;
                return;
            case RegisterType::B:
                coreRegisters.B = _value & 0xFF;
                return;
            case RegisterType::C:
                coreRegisters.C = _value & 0xFF;
                return;
            case RegisterType::D:
                coreRegisters.D = _value & 0xFF;
                return;
            case RegisterType::E:
                coreRegisters.E = _value & 0xFF;
                return;
            case RegisterType::H:
                coreRegisters.H = _value & 0xFF;
                return;
            case RegisterType::L:
                coreRegisters.L = _value & 0xFF ;
                return;
            case RegisterType::SP:
                coreRegisters.SP = _value ;
                return;
            case RegisterType::PC:
                coreRegisters.PC = _value;
                return;
            case RegisterType::AF:
                coreRegisters.A = _value >> 8 & 0xFF;
                coreRegisters.F = _value & 0xFF;
                return;
            case RegisterType::BC:
                coreRegisters.B = _value >> 8 & 0xFF;
                coreRegisters.C = _value & 0xFF;
                return;
            case RegisterType::DE:
                coreRegisters.D = _value >> 8 & 0xFF;
                coreRegisters.E = _value & 0xFF;
                return;
            case RegisterType::HL:
                coreRegisters.H = _value >> 8 & 0xFF;
                coreRegisters.L = _value & 0xFF;
                return;
        }
    }

    bool CPU::getflag(FlagType _flag) { return coreRegisters.F & _flag; }

    void CPU::setFlag(FlagType _flag, bool _state)
    {
        if (_state) { coreRegisters.F |= (_flag); }
        else { coreRegisters.F &= ~(_flag); }
    }

    void CPU::setFlags(bool _zState, bool _nState, bool _hState, bool _cState)
    {
        setFlag(FlagType::ZERO_FLAG, _zState);
        setFlag(FlagType::SUBTRACT_FLAG, _nState);
        setFlag(FlagType::HALF_CARRY_FLAG, _hState);
        setFlag(FlagType::CARRY_FLAG, _cState);
    }

    bool CPU::isConditionPassed()
    {
        bool z = getflag(FlagType::ZERO_FLAG);
        bool c = getflag(FlagType::CARRY_FLAG);

        switch (currentInstruction->condition)
        {
            case ConditionCode::NONE: return true;
            case ConditionCode::NC: return !c;
            case ConditionCode::C: return c;
            case ConditionCode::NZ: return !z;
            case ConditionCode::Z: return z;
        }

        return false;
    }

    void CPU::addr_IMP() {}
    void CPU::addr_R() { fetchedData = readRegister(currentInstruction->dstRegister); }
    void CPU::addr_R_D8() { fetchedData = busRef->read8(coreRegisters.PC++); }
    void CPU::addr_D16()
    {
        uint16_t lo = busRef->read8(coreRegisters.PC);
        uint16_t hi = busRef->read8(coreRegisters.PC + 1);
        fetchedData = lo | (hi << 8);
        coreRegisters.PC += 2;
    }
    void CPU::addr_R_D16()
    {
        uint16_t lo = busRef->read8(coreRegisters.PC);
        uint16_t hi = busRef->read8(coreRegisters.PC + 1);
        fetchedData = lo | (hi << 8);
        coreRegisters.PC += 2;
    }
    void CPU::addr_R_R() { fetchedData = readRegister(currentInstruction->srcRegister); }
    void CPU::addr_MR_R()
    {
        fetchedData = readRegister(currentInstruction->srcRegister);
        memoryDestination = readRegister(currentInstruction->dstRegister);
        isDestMem = true;

        if (currentInstruction->dstRegister == RegisterType::C) { memoryDestination |= 0xFF00; }
    }
    void CPU::addr_R_MR()
    {
        uint16_t srcAddress = readRegister(currentInstruction->srcRegister);

        if (currentInstruction->srcRegister == RegisterType::C) { srcAddress |= 0xFF00; }

        fetchedData = busRef->read8(srcAddress);
    }
    void CPU::addr_R_HLI()
    {
        fetchedData = busRef->read8(readRegister(currentInstruction->srcRegister));
        writeRegister(RegisterType::HL, readRegister(RegisterType::HL) + 1);
    }
    void CPU::addr_R_HLD()
    {
        fetchedData = busRef->read8(readRegister(currentInstruction->srcRegister));
        writeRegister(RegisterType::HL, readRegister(RegisterType::HL) - 1);
    }
    void CPU::addr_HLI_R()
    {
        fetchedData = readRegister(currentInstruction->srcRegister);
        memoryDestination = readRegister(currentInstruction->dstRegister);
        isDestMem = true;

        writeRegister(RegisterType::HL, readRegister(RegisterType::HL) + 1);
    }
    void CPU::addr_HLD_R()
    {
        fetchedData = readRegister(currentInstruction->srcRegister);
        memoryDestination = readRegister(currentInstruction->dstRegister);
        isDestMem = true;
        writeRegister(RegisterType::HL, readRegister(RegisterType::HL) - 1);
    }
    void CPU::addr_R_A8() { fetchedData = busRef->read8(coreRegisters.PC++); }
    void CPU::addr_A8_R()
    {
        isDestMem = true;
        memoryDestination = busRef->read8(coreRegisters.PC++ | 0xFF00);
    }
    void CPU::addr_HL_SPR() { fetchedData = busRef->read8(coreRegisters.PC++); }
    void CPU::addr_D8() { fetchedData = busRef->read8(coreRegisters.PC++); }
    void CPU::addr_D16_R()
    {
        uint16_t lo = busRef->read8(coreRegisters.PC);
        uint16_t hi = busRef->read8(coreRegisters.PC + 1);
        memoryDestination = lo | (hi << 8);
        isDestMem = true;
        coreRegisters.PC += 2;
        fetchedData = readRegister(currentInstruction->srcRegister);
    }
    void CPU::addr_MR_D8()
    {
        fetchedData = busRef->read8(coreRegisters.PC++);
        memoryDestination = readRegister(currentInstruction->dstRegister);
        isDestMem = true;
    }
    void CPU::addr_MR()
    {
        memoryDestination = readRegister(currentInstruction->dstRegister);
        isDestMem = true;
        fetchedData = busRef->read8(memoryDestination);;
    }
    void CPU::addr_A16_R()
    {
        uint16_t lo = busRef->read8(coreRegisters.PC);
        uint16_t hi = busRef->read8(coreRegisters.PC + 1);
        memoryDestination = lo | (hi << 8);
        isDestMem = true;
        coreRegisters.PC += 2;
        fetchedData = readRegister(currentInstruction->srcRegister);
    }
    void CPU::addr_R_A16()
    {
        uint16_t lo = busRef->read8(coreRegisters.PC);
        uint16_t hi = busRef->read8(coreRegisters.PC + 1);
        uint16_t dstAddr = lo | (hi << 8);
        coreRegisters.PC += 2;
        fetchedData = busRef->read8(dstAddr);
    }

    void CPU::nop() {}
    void CPU::dec() {}
    void CPU::ld()
    {
        if (isDestMem) // if the data is stored in the memory
        {
            // if stored value is 16-bit
            if (currentInstruction->srcRegister >= RegisterType::AF) { busRef->write16(memoryDestination, fetchedData); }
            // if stored value is 8-bit
            else { busRef->write8(memoryDestination, fetchedData); }
        }

        // SPR -> SP + e8 (opcode 0xF8)
        else if (currentInstruction->addrMode == &CPU::addr_HL_SPR)
        {
            uint8_t hFlag = (readRegister(currentInstruction->srcRegister) & 0xF) + (fetchedData& 0xF) >= 0x10;
            uint8_t cFlag = (readRegister(currentInstruction->srcRegister) & 0xFF) + (fetchedData & 0xFF) >= 0x100;

            setFlags(0, 0, hFlag, cFlag);
            writeRegister(currentInstruction->dstRegister, readRegister(currentInstruction->srcRegister) + (char)fetchedData);
        }
        //
        else { writeRegister(currentInstruction->dstRegister, fetchedData); }
    }

    void CPU::jp() { if (isConditionPassed()) { coreRegisters.PC = fetchedData; } }
    void CPU::di() { interruptHandler->setIME(false); }
    void CPU::halt() {}
    void CPU::call() {}
    void CPU::cb() {}
    void CPU::ccf() {}
    void CPU::cpl() {}
    void CPU::daa() {}
    void CPU::ei() {}
    void CPU::inc() {}
    void CPU::jr() {}
    void CPU::ldh() {}
    void CPU::pop() {}
    void CPU::push() {}
    void CPU::ret() {}
    void CPU::reti() {}
    void CPU::rlca() {}
    void CPU::rla() {}
    void CPU::rra() {}
    void CPU::rrca() {}
    void CPU::rst() {}
    void CPU::scf() {}
    void CPU::stop() {}

    void CPU::alu_adc() {}
    void CPU::alu_add() {}
    void CPU::alu_and() {}
    void CPU::alu_cp() {}
    void CPU::alu_sub() {}
    void CPU::alu_sbc() {}
    void CPU::alu_or() {}
    void CPU::alu_xor()
    {
        coreRegisters.A ^= (fetchedData & 0xFF);
        setFlags(coreRegisters.A == 0, 0, 0, 0);
    }
    void CPU::cb_bit() {}
    void CPU::cb_res() {}
    void CPU::cb_rl() {}
    void CPU::cb_rlc() {}
    void CPU::cb_rr() {}
    void CPU::cb_rrc() {}
    void CPU::cb_set() {}
    void CPU::cb_sla() {}
    void CPU::cb_sra() {}
    void CPU::cb_srl() {}
    void CPU::cb_swap() {}
}