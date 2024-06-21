//
// Created by ozgur on 12/04/2024.
//

#include "../include/CPU.h"
#include "../include/GameBoy.h"
#include "../include/MMU.h"
#include "../include/ALU.h"
#include "../include/InterruptHandler.h"

namespace gameboy
{
    CPU::CPU(GameBoy* _gb, MMU* _mmu, InterruptHandler* _interruptHandler) :
            isHalted(false),
            isStopped(false),
            cpuProcess(new InstructionSet()),
            interruptHandlerPtr(_interruptHandler),
            gameBoyPtr(_gb),
            mmuPtr(_mmu),
            AF(0x0100),
            BC(0), DE(0), HL(0),
            SP(0), PC(0x0100)
    {}

    bool CPU::init()
    {
        if (interruptHandlerPtr == nullptr)
        {
            printf("ERROR : interruptHandlerPtr is NULL!\n");
            return false;
        }

        if (gameBoyPtr == nullptr)
        {
            printf("ERROR : gameBoyPtr is NULL!\n");
            return false;
        }

        if (mmuPtr == nullptr)
        {
            printf("ERROR : mmuPtr is NULL!\n");
            return false;
        }

        alu = new ALU(this);
        if (!alu->init())
        {
            printf("ERROR : alu is NULL!\n");
            return false;
        }

        return true;
    }

    CPU::~CPU()
    {
        if (cpuProcess != nullptr) { delete cpuProcess; }
    }

    void CPU::step()
    {
        if (!isHalted)
        {
            fetch();
            execute();
        }
    }

    void CPU::fetch()
    {
        currentOpcode = mmuPtr->read8(PC++);
        currentInstruction = &cpuProcess->standardInstructions[currentOpcode];


        char flags[16];
        sprintf(flags, "%c%c%c%c",
                AF.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::z)) ? 'Z' : '-',
                AF.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::n)) ? 'N' : '-',
                AF.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::h)) ? 'H' : '-',
                AF.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::c)) ? 'C' : '-'
        );

        printf("\n");
        printf("%08lX - PC : %02X, Opcode : (%02X %02X %02X) : (A : %02X - F : %s - BC : %04X - DE : %04X - HL : %04X - SP : %04X) - %s",
               gameBoyPtr->ticks,
               PC.read() - 1,
               currentOpcode,
               mmuPtr->read8(PC.read()),
               mmuPtr->read8(PC.read() + 1),
               AF.highByte().read(),
               flags,
               BC.read(),
               DE.read(),
               HL.read(),
               SP.read(),
               currentInstruction->nmenomic.c_str());


        if (gameBoyPtr->ticks == 0x30000)
            exit(-1);
    }

    void CPU::execute()
    {
        if (currentOpcode != 0xCB)  { decodeStandardInstructions(); }
        else                        { decodeExtendedInstructions(); }
    }

    void CPU::emulateCycles(uint8_t cycleCount) { gameBoyPtr->emulateCycles(cycleCount); }

    void CPU::load(Register16 &_register, const uint16_t _value) { _register = _value; }
    void CPU::load(const uint16_t _address, const Register8 &_register) { mmuPtr->write8(_address, _register.read()); }
    void CPU::load(Register8 &_register, const uint8_t _value) { _register = _value; }
    void CPU::load(Register8 &_dstRegister, const Register8 &_srcRegister) { _dstRegister = _srcRegister; }
    void CPU::load(const uint16_t _address, const uint16_t _value) { mmuPtr->write16(_address, _value); }

    void CPU::setFlag(const CPUFlags _flag) { AF.lowByte() |= (1 << static_cast<uint8_t>(_flag)); }
    void CPU::resetFlag(const CPUFlags _flag) { AF.lowByte() &= ~(1 << static_cast<uint8_t>(_flag)); }
    uint8_t CPU::readFlag(const CPUFlags _flag) { return (AF.lowByte().read() >> static_cast<uint8_t>(_flag)) & 0x1; }
    bool CPU::checkFlag(const CPUFlags _flag) { return (AF.lowByte().read() >> static_cast<uint8_t>(_flag)) & 0x1; }

    void CPU::jump(const uint16_t _value)
    {
        PC = _value;
    }

    void CPU::jump(const Register16 &_register)
    {
        PC = _register;
    }

    void CPU::relativeJump(const uint16_t _data)
    {
        PC = PC.read() + _data;
    }

    void CPU::call(const uint16_t _value)
    {
        mmuPtr->push(SP, PC);
        PC = _value;
    }

    void CPU::di()
    {
        interruptHandlerPtr->setIME(false);
    }

    void CPU::ret()
    {
        PC = mmuPtr->pop(SP);
    }

    void CPU::halt()
    {
    }

    void CPU::decodeStandardInstructions()
    {
        switch (currentOpcode)
        {
            case 0x00: // NOP - 1/4t
                return;
            case 0x01: // LD BC, n16 - 3/12t
                printf("\nBefore : %02X\n", BC.read());
                load(BC, mmuPtr->read16(PC.read()));
                printf("After : %02X\n", BC.read());
                PC += 2;
                return;
            case 0x02: // LD [BC], A - 1/8t
                printf("\nBefore : %02X\n", mmuPtr->read8(BC.read()));
                load(BC.read(), AF.highByte());
                printf("After : %02X\n", mmuPtr->read8(BC.read()));
                return;
            case 0x03: // INC BC 1/8t
                printf("\nBefore : %02X\n", BC.read());
                alu->increment(BC);
                printf("After : %02X\n", BC.read());
                return;
            case 0x04: // INC B - 1/4t
                printf("\nBefore : %02X\n", BC.highByte().read());
                alu->increment(BC.highByte());
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x05: // DEC B - 1/4t
                printf("\nBefore : %02X\n", BC.highByte().read());
                alu->decrement(BC.highByte());
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x06: // LD B, n8 - 2/8t
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(BC.highByte(), mmuPtr->read8(PC++));
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x08: // LD [a16], SP
            {
                uint16_t address = mmuPtr->read16(PC.read());
                PC += 2;
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(address, SP.read());
                printf("After : %02X\n", BC.highByte().read());
                return;
            }
            case 0x09: // ADD HL, BC - 1/8t
                printf("\nBefore : %02X\n", HL.read());
                alu->add(BC);
                printf("After : %02X\n", HL.read());
                return;
            case 0x0A: // LD A, [BC] - 1/8t
            {
                uint16_t data = mmuPtr->read8(BC.read());
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), data);
                printf("After : %02X\n", AF.highByte().read());
                return;
            }
            case 0x0B: // DEC BC - 1/8t
                printf("\nBefore : %02X\n", BC.read());
                alu->decrement(BC);
                printf("After : %02X\n", BC.read());
                return;
            case 0x0C: // INC C 1/4t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                alu->increment(BC.lowByte());
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x0D: // DEC C - 1/4t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                alu->decrement(BC.lowByte());
                printf("After : %02X\n", BC.lowByte().read());
                return;
                return;
            case 0x0E: // LD C, n8 - 2/8t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                load(BC.lowByte(), mmuPtr->read8(PC++));
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x11: // LD DE, n16 - 3/12t
                printf("\nBefore : %02X\n", DE.read());
                load(DE, mmuPtr->read16(PC.read()));
                printf("After : %02X\n", DE.read());
                PC += 2;
                return;
            case 0x12: // LD [DE], A - 1/8t
                printf("\nBefore : %02X\n", mmuPtr->read8(DE.read()));
                load(DE.read(), AF.highByte());
                printf("After : %02X\n", mmuPtr->read8(DE.read()));
                return;
            case 0x13: // INC DE - 1/8t
                printf("\nBefore : %02X\n", DE.read());
                alu->increment(DE);
                printf("After : %02X\n", DE.read());
                return;
            case 0x14: // INC D - 1/4t
                printf("\nBefore : %02X\n", DE.highByte().read());
                alu->increment(DE.highByte());
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x15: // DEC D - 1/4t
                printf("\nBefore : %02X\n", DE.highByte().read());
                alu->decrement(DE.highByte());
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x16: // LD D, n8 - 2/8t
                printf("\nBefore : %02X\n", DE.highByte().read());
                load(DE.highByte(), mmuPtr->read8(PC++));
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x18: // JR e8 - 2/12t
            {
                int8_t e8 = static_cast<int8_t >(mmuPtr->read8(PC++) & 0xFF);
                printf("\nBefore : %02X\n", PC.read());
                relativeJump(e8);
                printf("After : %02X\n", PC.read());
                return;
            }
            case 0x19: // ADD HL, DE - 1/8t
                printf("\nBefore : %02X\n", HL.read());
                alu->add(DE);
                printf("After : %02X\n", HL.read());
                return;
            case 0x1A: // LD A, [DE] - 1/8t
            {
                uint16_t data = mmuPtr->read8(DE.read());
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), data);
                printf("After : %02X\n", AF.highByte().read());
                return;
            }
            case 0x1B: // DEC DE - 1/8t
                printf("\nBefore : %02X\n", DE.read());
                alu->decrement(DE);
                printf("After : %02X\n", DE.read());
                return;
            case 0x1C: // INC E - 1/4t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                alu->increment(DE.lowByte());
                printf("After : %02X\n", DE.lowByte().read());
                return;
            case 0x1D: // DEC E - 1/4t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                alu->decrement(DE.lowByte());
                printf("Before : %02X\n", DE.lowByte().read());
                return;
            case 0x1E: // LD E, n8 - 2/8t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                load(DE.lowByte(), mmuPtr->read8(PC++));
                printf("Before : %02X\n", DE.lowByte().read());
                return;
            case 0x1F:
                rra();
                return;
            case 0x20: // JR NZ, e8 - 2/12-8t
            {
                int8_t e8 = static_cast<int8_t >(mmuPtr->read8(PC++) & 0xFF);
                printf("\nBefore : %02X\n", PC.read());
                if (!checkFlag(CPUFlags::z))
                {
                    relativeJump(e8);
                }
                printf("After : %02X\n", PC.read());
                return;
            }
            case 0x21: // LD HL, n16 - 3/12t
                printf("\nBefore : %02X\n", HL.read());
                load(HL, mmuPtr->read16(PC.read()));
                printf("After : %02X\n", HL.read());
                PC += 2;
                return;
            case 0x22: // LD [HL+], A - 1/8t
                printf("\nBefore : %02X\n", mmuPtr->read8(HL.read()));
                load(HL++, AF.highByte());
                printf("After : %02X\n", mmuPtr->read8(HL.read()));
                return;
            case 0x23: // INC HL - 1/8t
                printf("\nBefore : %02X\n", HL.read());
                alu->increment(HL);
                printf("After : %02X\n", HL.read());
                return;
            case 0x24: // INC H - 1/4t
                printf("\nBefore : %02X\n", HL.highByte().read());
                alu->increment(HL.highByte());
                printf("After : %02X\n", HL.highByte().read());
                return;
            case 0x25: // DEC H - 1/4t
                printf("\nBefore : %02X\n", HL.highByte().read());
                alu->decrement(HL.highByte());
                printf("After : %02X\n", HL.highByte().read());
                return;
            case 0x26: // LD H, n8 - 2/8t
                printf("\nBefore : %02X\n", HL.highByte().read());
                load(HL.highByte(), mmuPtr->read8(PC++));
                printf("After : %02X\n", HL.highByte().read());
                return;
            case 0x28: // JR Z, e8 - 2/12-8t
            {
                int8_t e8 = static_cast<int8_t >(mmuPtr->read8(PC++) & 0xFF);
                if (checkFlag(CPUFlags::z))
                {
                    printf("\nBefore : %02X\n", PC.read());
                    relativeJump(e8);
                    printf("After : %02X\n", PC.read());
                }
                return;
            }
            case 0x29: // ADD HL, HL - 1/8t
                printf("\nBefore : %02X\n",HL.read());
                alu->add(HL);
                printf("Before : %02X\n", HL.read());
                return;
            case 0x2A: // LD A, [HL+] - 1/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), mmuPtr->read8(HL++));
                printf("Before : %02X\n", AF.highByte().read());
                return;
            case 0x2B: // DEC HL - 1/8t
                printf("\nBefore : %02X\n", HL.read());
                alu->decrement(HL);
                printf("After : %02X\n", HL.read());
                return;
            case 0x2C: // INC L - 1/4t
                printf("\nBefore : %02X\n", HL.lowByte().read());
                alu->increment(HL.lowByte());
                printf("Before : %02X\n", HL.lowByte().read());
                return;
            case 0x2D: // DEC L - 1/4t
                printf("\nBefore : %02X\n", HL.lowByte().read());
                alu->decrement(HL.lowByte());
                printf("Before : %02X\n", HL.lowByte().read());
                return;
            case 0x2E: // LD L, n8 - 2/8t7
                printf("\nBefore : %02X\n", HL.lowByte().read());
                load(HL.lowByte(), mmuPtr->read8(PC++));
                printf("Before : %02X\n", HL.lowByte().read());
                return;
            case 0x30: // JR NC, e8 - 2/12-8t
            {
                int8_t e8 = static_cast<int8_t >(mmuPtr->read8(PC++) & 0xFF);
                if (!checkFlag(CPUFlags::c))
                {
                    printf("\nBefore : %02X\n", PC.read());
                    relativeJump(e8);
                    printf("After : %02X\n", PC.read());
                }
                return;
            }
            case 0x31: // LD SP, n16 - 3/12t
                printf("\nBefore : %02X\n", SP.read());
                load(SP, mmuPtr->read16(PC.read()));
                printf("After : %02X\n", SP.read());
                PC += 2;
                return;
            case 0x32: // LD [HL-], A
                printf("\nBefore : %02X, A : %02X\n", mmuPtr->read8(HL.read()), AF.highByte().read());
                load(HL--, AF.highByte());
                printf("After : %02X\n", mmuPtr->read8(HL.read() + 1));
                return;
            case 0x33: // INC SP - 1/8t
                printf("\nBefore : %02X\n", SP.read());
                alu->increment(SP);
                printf("After : %02X\n", SP.read());
                return;
            case 0x34: // INC [HL] - 1/12t
            {
                uint8_t data = mmuPtr->read8(HL.read());
                printf("\nBefore : %02X\n", data);
                alu->increment(data);
                printf("After : %02X\n", data);
                mmuPtr->write8(HL.read(), data);
                return;
            }
            case 0x35: // DEC [HL] - 1/12t
            {
                uint8_t data = mmuPtr->read8(HL.read());
                printf("\nBefore : %02X\n", data);
                alu->decrement(data);
                printf("After : %02X\n", data);
                mmuPtr->write8(HL.read(), data);
                return;
            }
            case 0x36: // LD [HL], n8 - 2/12t
                printf("\nBefore : %02X, n8 : %02X\n", mmuPtr->read8(HL.read()), mmuPtr->read8(PC.read()));
                load(HL.read(), mmuPtr->read8(PC++));
                printf("After : %02X\n", mmuPtr->read8(HL.read()));
                return;
            case 0x38: // JR C, e8 - 2/12-8t
            {
                int8_t e8 = static_cast<int8_t >(mmuPtr->read8(PC++) & 0xFF);
                if (checkFlag(CPUFlags::c))
                {
                    printf("\nBefore : %02X\n", PC.read());
                    relativeJump(e8);
                    printf("After : %02X\n", PC.read());
                }
                return;
            }
            case 0x39: // ADD HL, SP - 1/8t
                printf("\nBefore : %02X\n",SP.read());
                alu->add(SP);
                printf("Before : %02X\n", SP.read());
                return;
            case 0x3A: // LD A, [HL-] - 1/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), mmuPtr->read8(HL--));
                printf("Before : %02X\n", AF.highByte().read());
                return;
            case 0x3B: // DEC SP - 1/8t
                printf("\nBefore : %02X\n", SP.read());
                alu->decrement(SP);
                printf("After : %02X\n", SP.read());
                return;
            case 0x3C: // INC A - 1/4t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->increment(AF.highByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x3D: // DEC A - 1/4t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->decrement(AF.highByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x3E: // LD A, n8 - 2/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), mmuPtr->read8(PC++));
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x40: // LD B, B - 1/4t
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(BC.highByte(), BC.highByte());
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x41: // LD B, C - 1/4t
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(BC.highByte(), BC.lowByte());
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x42: // LD B, D - 1/4t
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(BC.highByte(), DE.highByte());
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x43: // LD B, E - 1/4t
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(BC.highByte(), DE.lowByte());
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x44: // LD B, H - 1/4t
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(BC.highByte(), HL.highByte());
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x45: // LD B, L - 1/4t
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(BC.highByte(), HL.lowByte());
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x46: // LD B, [HL] - 1/8t
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(BC.highByte(), mmuPtr->read8(HL.read()));
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x47: // LD B, A - 1/4t
                printf("\nBefore : %02X\n", BC.highByte().read());
                load(BC.highByte(), AF.highByte());
                printf("After : %02X\n", BC.highByte().read());
                return;
            case 0x48: // LD C, B - 1/4t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                load(BC.lowByte(), BC.highByte());
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x49: // LD C, C - 1/4t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                load(BC.lowByte(), BC.lowByte());
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x4A: // LD C, D - 1/4t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                load(BC.lowByte(), DE.highByte());
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x4B: // LD C, E - 1/4t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                load(BC.lowByte(), DE.lowByte());
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x4C: // LD C, H - 1/4t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                load(BC.lowByte(), HL.highByte());
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x4D: // LD C, L - 1/4t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                load(BC.lowByte(), HL.lowByte());
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x4E: // LD C, [HL] - 1/8t
                printf("\nBefore : %02X\n", BC.lowByte().read());
                load(BC.lowByte(), mmuPtr->read8(HL.read()));
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x4F: // LD C, A - 1/4t
                printf("\nBefore : %02X, A : %02X\n", BC.lowByte().read(), AF.highByte().read());
                load(BC.lowByte(), AF.highByte());
                printf("After : %02X\n", BC.lowByte().read());
                return;
            case 0x50: // LD D, B - 1/4t
                printf("\nBefore : %02X\n", DE.highByte().read());
                load(DE.highByte(), BC.highByte());
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x51: // LD D, C - 1/4t
                printf("\nBefore : %02X\n", DE.highByte().read());
                load(DE.highByte(), BC.lowByte());
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x52: // LD D D - 1/4t
                printf("\nBefore : %02X\n", DE.highByte().read());
                load(DE.highByte(), DE.highByte());
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x53: // LD D, E - 1/4t
                printf("\nBefore : %02X\n", DE.highByte().read());
                load(DE.highByte(), DE.lowByte());
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x54: // LD D, H - 1/4t
                printf("\nBefore : %02X\n", DE.highByte().read());
                load(DE.highByte(), HL.highByte());
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x55: // LD D, L - 1/4t
                printf("\nBefore : %02X\n", DE.highByte().read());
                load(DE.highByte(), HL.lowByte());
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x56: // LD D, [HL] - 1/8t
                printf("\nBefore : %02X\n", DE.highByte().read());
                load(DE.highByte(), mmuPtr->read8(HL.read()));
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x57: // LD D, A - 1/4t
                printf("\nBefore : %02X, A : %02X\n", DE.highByte().read(), AF.highByte().read());
                load(DE.highByte(), AF.highByte());
                printf("After : %02X\n", DE.highByte().read());
                return;
            case 0x58: // LD E, B - 1/4t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                load(DE.lowByte(), BC.highByte());
                printf("After : %02X\n", DE.lowByte().read());
                return;
            case 0x59: // LD E, C - 1/4t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                load(DE.lowByte(), BC.lowByte());
                printf("After : %02X\n", DE.lowByte().read());
                return;
            case 0x5A: // LD E, D - 1/4t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                load(DE.lowByte(), DE.highByte());
                printf("After : %02X\n", DE.lowByte().read());
                return;
            case 0x5B: // LD E, E - 1/4t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                load(DE.lowByte(), DE.lowByte());
                printf("After : %02X\n", DE.lowByte().read());
                return;
            case 0x5C: // LD E, H - 1/4t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                load(DE.lowByte(), HL.highByte());
                printf("After : %02X\n", DE.lowByte().read());
                return;
            case 0x5D: // LD E, L - 1/4t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                load(DE.lowByte(), HL.lowByte());
                printf("After : %02X\n", DE.lowByte().read());
                return;
            case 0x5E: // LD E, [HL] - 1/8t
                printf("\nBefore : %02X\n", DE.lowByte().read());
                load(DE.lowByte(), mmuPtr->read8(HL.read()));
                printf("After : %02X\n", DE.lowByte().read());
                return;
            case 0x5F: // LD E, A - 1/4t
                printf("\nBefore : %02X, A : %02X\n", DE.lowByte().read(), AF.highByte().read());
                load(DE.lowByte(), AF.highByte());
                printf("After : %02X\n", DE.lowByte().read());
                return;
            case 0x60: // LD H, B - 1/4t
                printf("\nBefore : %02X\n", HL.highByte().read());
                load(HL.highByte(), BC.highByte());
                printf("After : %02X\n", HL.highByte().read());
                return;



            case 0x66: // LD H, [HL] - 1/8t
                printf("\nBefore : %02X\n", HL.highByte().read());
                load(HL.highByte(), mmuPtr->read8(HL.read()));
                printf("After : %02X\n", HL.highByte().read());
                return;
            case 0x6C: // LD L, H - 1/4t
                printf("\nBefore : %02X\n", HL.lowByte().read());
                load(HL.lowByte(), HL.highByte());
                printf("After : %02X\n", HL.lowByte().read());
                return;
            case 0x6E: // LD L, [HL] - 1/8t
                printf("\nBefore : %02X\n", HL.lowByte().read());
                load(HL.lowByte(), mmuPtr->read8(HL.read()));
                printf("After : %02X\n", HL.lowByte().read());
                return;
            case 0x6F: // LD L, A - 1/4t
                printf("\nBefore : %02X\n", HL.lowByte().read());
                load(HL.lowByte(), AF.highByte());
                printf("After : %02X\n", HL.lowByte().read());
                return;
            case 0x70: // LD [HL], B - 1/8t
                printf("\nBefore : %02X, C : %02X\n", mmuPtr->read8(HL.read()), BC.highByte().read());
                load(HL.read(), BC.highByte());
                printf("After : %02X\n", mmuPtr->read8(HL.read()));
                return;
            case 0x71: // LD [HL], C - 1/8t
                printf("\nBefore : %02X, C : %02X\n", mmuPtr->read8(HL.read()), BC.lowByte().read());
                load(HL.read(), BC.lowByte());
                printf("After : %02X\n", mmuPtr->read8(HL.read()));
                return;
            case 0x72: // LD [HL], D - 1/8t
                printf("\nBefore : %02X, C : %02X\n", mmuPtr->read8(HL.read()), DE.highByte().read());
                load(HL.read(), DE.highByte());
                printf("After : %02X\n", mmuPtr->read8(HL.read()));
                return;
            case 0x76: // HALT - 1/4t
                halt();
                return;
            case 0x77: // LD [HL], A - 1/8t
                printf("\nBefore : %02X\n", mmuPtr->read8(HL.read()));
                load(HL.read(), AF.highByte().read());
                printf("After : %02X\n", mmuPtr->read8(HL.read()));
                return;
            case 0x78: // LD A, B - 1/4t
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), BC.highByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x79: // LD A, C - 1/4t
                printf("\nBefore : %02X, C : %02X\n", AF.highByte().read(), BC.lowByte().read());
                load(AF.highByte(), BC.lowByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x7A: // LD A, D - 1/4t
                printf("\nBefore : %02X, C : %02X\n", AF.highByte().read(), DE.highByte().read());
                load(AF.highByte(), DE.highByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x7B: // LD A, E - 1/4t
                printf("\nBefore : %02X, C : %02X\n", AF.highByte().read(), DE.lowByte().read());
                load(AF.highByte(), DE.lowByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x7C: // LD A, H - 1/4t
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), HL.highByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x7D: // LD A, L - 1/4t
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), HL.lowByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x7E: // LD A, [HL] - 1/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), mmuPtr->read8(HL.read()));
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xA9: // XOR A, C - 1/4t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->xor_(BC.lowByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xAE: // XOR A, [HL] - 1/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->xor_(mmuPtr->read8(HL.read()));
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xB1: // OR A, C - 1/4t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->or_(BC.lowByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xB6:
            {
                uint16_t address = HL.read();
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->or_(mmuPtr->read8(address));
                printf("After : %02X\n", AF.highByte().read());
                return;
            }
            case 0xB7: // OR A, A - 1/4t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->or_(AF.highByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xC1: // POP BC - 1/12t
                printf("\nBefore : %02X\n", BC.read());
                BC = mmuPtr->pop(SP);
                printf("After : %02X\n", BC.read());
                return;
            case 0xC3: // JP a16 - 3/16t
                printf("\nBefore : %02X\n", PC.read());
                jump(mmuPtr->read16(PC.read()));
                printf("After : %02X\n", PC.read());
                return;
            case 0xC4: // CALL NZ, a16 - 3/24-12t
            {
                uint16_t n16 = mmuPtr->read16(PC.read());
                printf("\nBefore : %02X\n", PC.read());
                PC += 2;
                if (!checkFlag(CPUFlags::z))
                {
                    call(n16);
                }
                printf("After : %02X\n", PC.read());
                return;
            }
            case 0xC5: // PUSH BC - 1/16t
                mmuPtr->push(SP, BC);
                return;
            case 0xC6: // ADD A, n8 - 2/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->add(mmuPtr->read8(PC++));
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xC8: // RET Z - 1/20-8t
                printf("\nBefore : %02X, zFlag : %d\n", PC.read(), readFlag(CPUFlags::z));
                if (checkFlag(CPUFlags::z))
                {
                    ret();
                }
                printf("After : %02X\n", PC.read());
                return;
            case 0xC9: // RET - 1/16t
                printf("\nBefore : %02X\n", PC.read());
                ret();
                printf("After : %02X\n", PC.read());
                return;
            case 0xCB:
                /* Extended Instructions (Prefix) */
                return;
            case 0xCD: // CALL a16 - 3/24t
            {
                printf("\nBefore : %02X\n", PC.read());
                uint16_t n16 = mmuPtr->read16(PC.read());
                PC += 2;
                call(n16);
                printf("After : %02X\n", PC.read());
                return;
            }
            case 0xCE: // ADC A, n8 - 2/8t
                // printf("\nBefore : %02X\n", AF.highByte().read());
                alu->adc(mmuPtr->read8(PC++));
                // printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xD0: // RET NC - 1/20-8t
                printf("\nBefore : %02X\n", PC.read());
                if (!checkFlag(CPUFlags::c))
                {
                    ret();
                }
                printf("After : %02X\n", PC.read());
                return;
            case 0xD1: // POP DE - 1/12t
                printf("\nBefore : %02X\n", DE.read());
                DE = mmuPtr->pop(SP);
                printf("After : %02X\n", DE.read());
                return;
            case 0xD5: // PUSH DE - 1/16t
                mmuPtr->push(SP, DE);
                return;
            case 0xD6: // SUB A, n8 - 2/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->subtract(mmuPtr->read8(PC++));
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xE0: // LD ($FF00+a8),A - 2/12t
            {
                uint16_t address = 0xFF00 | mmuPtr->read8(PC++);
                printf("\nBefore : %02X\n", mmuPtr->read8(address));
                load(address, AF.highByte());
                printf("After : %02X\n", mmuPtr->read8(address));
                return;
            }
            case 0xE1: // POP HL - 1/12t
                printf("\nBefore : %02X\n", HL.read());
                HL = mmuPtr->pop(SP);
                printf("After : %02X\n", HL.read());
                return;
            case 0xE5: // PUSH HL - 1/16t
                mmuPtr->push(SP, HL);
                return;
            case 0xE6: // AND A, n8 - 2/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->and_(mmuPtr->read8(PC++));
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xE9:
                printf("\nBefore : %02X\n", PC.read());
                jump(HL);
                printf("After : %02X\n", PC.read());
                return;
            case 0xEA: // LD [a16], A - 3/16t
                printf("\nBefore : %02X, address : %02X\n", mmuPtr->read8(mmuPtr->read16(PC.read())), mmuPtr->read16(PC.read()));
                load(mmuPtr->read16(PC.read()), AF.highByte());
                printf("After : %02X, address : %02X\n", mmuPtr->read8(mmuPtr->read16(PC.read())), mmuPtr->read16(PC.read()));
                PC += 2;
                return;
            case 0XEE: // XOR A, n8 - 2/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->xor_(mmuPtr->read8(PC++));
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xF0: // LD A, ($FF00+a8) - 2/12t
            {
                uint16_t address = 0xFF00 | mmuPtr->read8(PC++);
                uint8_t data = mmuPtr->read8(address);
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), data);
                printf("After : %02X\n", AF.highByte().read());
                return;
            }
            case 0xF1: // POP AF - 1/12t
                printf("\nBefore : %02X\n", AF.read());
                AF = (mmuPtr->pop(SP) & 0xFFF0);
                printf("After : %02X\n", AF.read());
                return;
            case 0xF3: // DI - 1/4t
                di();
                return;
            case 0xF5: // PUSH AF - 1/16t
                mmuPtr->push(SP, AF);
                return;
            case 0xF6: // OR A, n8 - 2/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                alu->or_(mmuPtr->read8(PC++));
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0xFA: // LD A, [a16] - 3/16t
            {
                uint16_t address = mmuPtr->read16(PC.read());
                PC += 2;
                printf("\nBefore : %02X\n", AF.highByte().read());
                load(AF.highByte(), mmuPtr->read8(address));
                printf("After : %02X\n", AF.highByte().read());
                return;
            }
            case 0xFE: // CP A, n8 - 2/8t
                alu->compare(mmuPtr->read8(PC++));
                return;
            default:
                printf(" -> INVALID INSTRUCTION!");
                exit(-1);
        }
    }

    void CPU::decodeExtendedInstructions()
    {
        currentOpcode = mmuPtr->read8(PC++);

        printf("\nCB INST : %#02x\n", currentOpcode);

        switch (currentOpcode)
        {
            case 0x19: // RR C - 2/8t
                rr(BC.lowByte());
                return;
            case 0x1A:// RR D - 2/8t
                rr(DE.highByte());
                return;
            case 0x37: // SWAP A - 2/8t
                printf("\nBefore : %02X\n", AF.highByte().read());
                swap(AF.highByte());
                printf("After : %02X\n", AF.highByte().read());
                return;
            case 0x38: // SRL B - 2/8t
                srl(BC.highByte());
                return;
            case 0x3F: // SRL A - 2/8t
                srl(AF.highByte());
                return;
        }

        printf(" -> INVALID INSTRUCTION!");
        exit(-1);
        // TODO : WILL BE IMPLEMENTED CB INSTRUCTIONS
    }

    void CPU::srl(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        srl(data);
        _dstRegister = data;
    }

    void CPU::srl(uint8_t& _value)
    {
        (_value & 0x1) ? setFlag(CPUFlags::c) : resetFlag(CPUFlags::c);

        _value >>= 1;

        _value == 0x0 ? setFlag(CPUFlags::z) : resetFlag(CPUFlags::z);
        resetFlag(CPUFlags::n);
        resetFlag(CPUFlags::h);
    }

    void CPU::rr(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        printf("\nBefore : %02X\n", data);
        rr(data);
        printf("After : %02X\n", data);
        _dstRegister = data;
    }

    void CPU::rr(uint8_t &_value)
    {
        uint8_t cFlag = readFlag(CPUFlags::c);

        (_value & 0x1) ? setFlag(CPUFlags::c) : resetFlag(CPUFlags::c);

        _value = (_value >> 1) | (cFlag << 7);

        _value == 0x0 ? setFlag(CPUFlags::z) : resetFlag(CPUFlags::z);
        resetFlag(CPUFlags::n);
        resetFlag(CPUFlags::h);
    }

    void CPU::rra()
    {
        rr(AF.highByte());
        resetFlag(CPUFlags::z);
    }

    void CPU::swap(Register8 &_dstRegister)
    {
        uint8_t data = _dstRegister.read();
        swap(data);
        _dstRegister = data;
    }

    void CPU::swap(uint8_t &_value)
    {
        uint8_t lowerNibble = _value & 0x0F;
        uint8_t upperNibble = _value & 0xF0;

        uint8_t swapped = (lowerNibble << 4) | (upperNibble >> 4);

        _value = swapped;

        swapped == 0x0 ? setFlag(CPUFlags::z) : resetFlag(CPUFlags::z);
        resetFlag(CPUFlags::n);
        resetFlag(CPUFlags::h);
        resetFlag(CPUFlags::c);
    }
}

// 1100