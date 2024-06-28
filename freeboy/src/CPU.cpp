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


        if (currentOpcode == 0xCB)
        {

        }
        printf("\n");

        char flags[16];
        sprintf(flags, "%c%c%c%c",
                AF.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::z)) ? 'Z' : '-',
                AF.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::n)) ? 'N' : '-',
                AF.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::h)) ? 'H' : '-',
                AF.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::c)) ? 'C' : '-'
        );

        printf("%08lX - PC : %02X, Opcode : (%02X %02X %02X) : (A : %02X - F : %s - BC : %04X - DE : %04X - HL : %04X - SP : %04X) - %s\n",
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

        if (gameBoyPtr->ticks > 0x177379)
            exit(-1);
    }

    void CPU::execute()
    {
        if (currentOpcode != 0xCB)  { decodeStandardInstructions(); }
        else                        { decodeExtendedInstructions(); }
    }

    void CPU::emulateCycles(uint8_t cycleCount) { gameBoyPtr->emulateCycles(cycleCount); }

    void CPU::load(Register16 &_register, const uint16_t _value)
    {
        printf("\nBefore : %02X\n", _register.read());
        _register = _value;
        printf("After : %02X\n", _register.read());
    }
    void CPU::load(const uint16_t _address, const Register8 &_register)
    {
        printf("\nBefore : %02X\n", mmuPtr->read8(_address));
        mmuPtr->write8(_address, _register.read());
        printf("After : %02X\n", mmuPtr->read8(_address));
    }
    void CPU::load(Register8 &_register, const uint8_t _value)
    {
        printf("\nBefore : %02X\n", _register.read());
        _register = _value;
        printf("After : %02X\n", _register.read());
    }
    void CPU::load(Register8 &_dstRegister, const Register8 &_srcRegister)
    {
        printf("\nBefore : %02X\n", _dstRegister.read());
        _dstRegister = _srcRegister;
        printf("After : %02X\n", _dstRegister.read());
    }
    void CPU::load(const uint16_t _address, const uint16_t _value)
    {
        printf("\nBefore : %02X\n", mmuPtr->read16(_address));
        mmuPtr->write16(_address, _value);
        printf("After : %02X\n", mmuPtr->read16(_address));
    }

    void CPU::setFlag(const CPUFlags _flag) { AF.lowByte() |= (1 << static_cast<uint8_t>(_flag)); }
    void CPU::resetFlag(const CPUFlags _flag) { AF.lowByte() &= ~(1 << static_cast<uint8_t>(_flag)); }
    void CPU::toggleFlag(const CPUFlags _flag) { AF.lowByte() ^= (1 << static_cast<uint8_t>(_flag)); };
    uint8_t CPU::readFlag(const CPUFlags _flag) { return (AF.lowByte().read() >> static_cast<uint8_t>(_flag)) & 0x1; }
    bool CPU::checkFlag(const CPUFlags _flag) { return (AF.lowByte().read() >> static_cast<uint8_t>(_flag)) & 0x1; }

    void CPU::jump(const uint16_t _value)
    {
        printf("\nBefore : %02X\n", PC.read());
        PC = _value;
        printf("After : %02X\n", PC.read());
    }

    void CPU::jump(const Register16 &_register)
    {
        printf("\nBefore : %02X\n", PC.read());
        PC = _register;
        printf("After : %02X\n", PC.read());
    }

    void CPU::relativeJump(const uint16_t _data)
    {
        printf("\nBefore : %02X\n", PC.read());
        PC = PC.read() + _data;
        printf("After : %02X\n", PC.read());
    }

    void CPU::call(const uint16_t _value)
    {
        printf("\nBefore : %02X\n", PC.read());
        mmuPtr->push(SP, PC);
        PC = _value;
        printf("After : %02X\n", PC.read());
    }

    void CPU::rst(const uint8_t vector)
    {
        mmuPtr->push(SP, PC);
        PC = vector;
    }

    void CPU::di() { interruptHandlerPtr->setIME(false); }
    void CPU::ei() { interruptHandlerPtr->setIME(true); }


    void CPU::ret()
    {
        printf("\nBefore : %02X\n", PC.read());
        PC = mmuPtr->pop(SP);
        printf("After : %02X\n", PC.read());
    }

    void CPU::reti()
    {
        ret();
        ei();
    }

    void CPU::halt() { printf("NOT IMPLEMENTED YET\n"); exit(-1); }

    void CPU::decodeStandardInstructions()
    {
        switch (currentOpcode)
        {
            case 0x00: // NOP - 1/4t
                return;
            case 0x01: // LD BC, n16
            {
                uint16_t n16 = mmuPtr->read16(PC.read());
                PC += 2;
                load(BC, n16);
                return;
            }
            case 0x02:
            {
                uint16_t address = BC.read();
                load(address, AF.highByte());
                return;
            }
            case 0x03:
                alu->increment(BC);
                return;
            case 0x04:
                alu->increment(BC.highByte());
                return;
            case 0x05:
                alu->decrement(BC.highByte());
                return;
            case 0x06:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                load(BC.highByte(), n8);
                return;
            }
            case 0x07:
                alu->rlca();
                return;
            case 0x08:
            {
                uint16_t a16 = mmuPtr->read16(PC.read());
                PC += 2;
                load(a16, SP.read());
                return;
            }
            case 0x09:
                alu->add(BC);
                return;
            case 0x0A:
            {
                uint16_t address = BC.read();
                uint8_t data = mmuPtr->read8(address);
                load(AF.highByte(), data);
                return;
            }
            case 0x0B:
                alu->decrement(BC);
                return;
            case 0x0C:
                alu->increment(BC.lowByte());
                return;
            case 0x0D:
                alu->decrement(BC.lowByte());
                return;
            case 0x0E:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                load(BC.lowByte(), n8);
                return;
            }
            case 0x0F:
                alu->rrca();
                return;
            case 0x10:
                // STOP 0
                return;
            case 0x11:
            {
                uint16_t n16 = mmuPtr->read16(PC.read());
                PC += 2;
                load(DE, n16);
                return;
            }
            case 0x12:
            {
                uint16_t address = DE.read();
                load(address, AF.highByte());
                return;
            }
            case 0x13:
                alu->increment(DE);
                return;
            case 0x14:
                alu->increment(DE.highByte());
                return;
            case 0x15:
                alu->decrement(DE.highByte());
            case 0x16:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                load(DE.highByte(), n8);
                return;
            }
            case 0x17:
                alu->rla();
                return;
            case 0x18: // JR e8 - 2/12t
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(PC++));
                relativeJump(e8);
                return;
            }
            case 0x19:
                alu->add(DE);
                return;
            case 0x1A:
            {
                uint16_t address = DE.read();
                uint8_t data = mmuPtr->read16(address);
                load(AF.highByte(), data);
                return;
            }
            case 0x1B:
                alu->decrement(DE);
                return;
            case 0x1C:
                alu->increment(DE.lowByte());
                return;
            case 0x1D:
                alu->decrement(DE.lowByte());
                return;
            case 0x1E:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                load(DE.lowByte(), n8);
                return;
            }
            case 0x1F:
                alu->rra();
                return;
            case 0x20:
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(PC++));
                if (!checkFlag(CPUFlags::z))
                {
                    relativeJump(e8);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0x21: // LD HL,n16 - 3/12t
            {
                uint16_t address = PC.read();
                uint16_t data = mmuPtr->read16(address);
                PC += 2;
                load(HL, data);
                return;
            }
            case 0x22:
            {
                uint16_t address = HL++;
                load(address, AF.highByte());
                return;
            }
            case 0x23:
                alu->increment(HL);
                return;
            case 0x24:
                alu->increment(HL.highByte());
                return;
            case 0x25:
                alu->decrement(HL.highByte());
                return;
            case 0x26:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                load(HL.highByte(), n8);
                return;
            }
            case 0x27:
                alu->daa();
                return;
            case 0x28:
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(PC++));
                if (checkFlag(CPUFlags::z))
                {
                    relativeJump(e8);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0x29:
                alu->add(HL);
                return;
            case 0x2A: // LD A, [HL+]
            {
                load(AF.highByte(), mmuPtr->read8(HL++));
                return;
            }
            case 0x2B:
                alu->decrement(HL);
                return;
            case 0x2C:
                alu->increment(HL.lowByte());
                return;
            case 0x2D:
                alu->decrement(HL.lowByte());
                return;
            case 0x2E:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                load(HL.lowByte(), n8);
                return;
            }
            case 0x2F:
            {
                alu->complement();
                return;
            }
            case 0x30:
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(PC++));
                if (!checkFlag(CPUFlags::c))
                {
                    relativeJump(e8);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0x31: // LD SP, n16 - 3/12t
            {
                uint16_t address = PC.read();
                load(SP, mmuPtr->read16(address));
                PC += 2;
                return;
            }
            case 0x32:
            {
                uint16_t address = HL--;
                load(address, AF.highByte());
                return;
            }
            case 0x33:
                alu->increment(SP);
                return;
            case 0x34:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->increment(data);
                mmuPtr->write8(address, data);
            }
            case 0x35:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->decrement(data);
                mmuPtr->write8(address, data);
                return;
            }
            case 0x36:
            {
                uint16_t address = HL.read();
                uint8_t n8 = mmuPtr->read8(PC++);
                load(address, n8);
                return;
            }
            case 0x38:
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(PC++));
                if (checkFlag(CPUFlags::c))
                {
                    relativeJump(e8);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0x39:
                alu->add(SP);
                return;
            case 0x3A:
                load(AF.highByte(), mmuPtr->read8(HL--));
                return;
            case 0x3B:
                alu->decrement(SP);
                return;
            case 0x3C:
                alu->increment(AF.highByte());
                return;
            case 0x3D:
                alu->decrement(AF.highByte());
                return;
            case 0x3F:
                alu->complementCarryFlag();
                return;
            case 0x3E: // LD A,n8 - 2/8t
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                load(AF.highByte(), n8);
                return;
            }
            case 0x40:
                load(BC.highByte(), BC.highByte());
                return;
            case 0x41:
                load(BC.highByte(), BC.lowByte());
                return;
            case 0x42:
                load(BC.highByte(), DE.highByte());
                return;
            case 0x43:
                load(BC.highByte(), DE.lowByte());
                return;
            case 0x44:
                load(BC.highByte(), HL.highByte());
                return;
            case 0x45:
                load(BC.highByte(), HL.lowByte());
                return;
            case 0x46:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                load(BC.highByte(), data);
                return;
            }
            case 0x47:
                load(BC.highByte(), AF.highByte());
                return;
            case 0x48:
                load(BC.lowByte(), BC.highByte());
                return;
            case 0x49:
                load(BC.lowByte(), BC.lowByte());
                return;
            case 0x4A:
                load(BC.lowByte(), DE.highByte());
                return;
            case 0x4B:
                load(BC.lowByte(), DE.lowByte());
                return;
            case 0x4C:
                load(BC.lowByte(), HL.highByte());
                return;
            case 0x4D:
                load(BC.lowByte(), HL.lowByte());
                return;
            case 0x4E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                load(BC.lowByte(), data);
                return;
            }
            case 0x4F:
                load(BC.lowByte(), AF.highByte());
                return;
            case 0x50:
                load(DE.highByte(), BC.highByte());
                return;
            case 0x51:
                load(DE.highByte(), BC.lowByte());
                return;
            case 0x52:
                load(DE.highByte(), DE.highByte());
                return;
            case 0x53:
                load(DE.highByte(), DE.lowByte());
                return;
            case 0x54:
                load(DE.highByte(), HL.highByte());
                return;
            case 0x55:
                load(DE.highByte(), HL.lowByte());
                return;
            case 0x56:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                load(DE.highByte(), data);
                return;
            }
            case 0x57:
                load(DE.highByte(), AF.highByte());
                return;
            case 0x58:
                load(DE.lowByte(), BC.highByte());
                return;
            case 0x59:
                load(DE.lowByte(), BC.lowByte());
                return;
            case 0x5A:
                load(DE.lowByte(), DE.highByte());
                return;
            case 0x5B:
                load(DE.lowByte(), DE.lowByte());
                return;
            case 0x5C:
                load(DE.lowByte(), HL.highByte());
                return;
            case 0x5D:
                load(DE.lowByte(), HL.lowByte());
                return;
            case 0x5E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                load(DE.lowByte(), data);
                return;
            }
            case 0x5F:
                load(DE.lowByte(), AF.highByte());
                return;
            case 0x60:
                load(HL.highByte(), BC.highByte());
                return;
            case 0x61:
                load(HL.highByte(), BC.lowByte());
                return;
            case 0x62:
                load(HL.highByte(), DE.highByte());
                return;
            case 0x63:
                load(HL.highByte(), DE.lowByte());
                return;
            case 0x64:
                load(HL.highByte(), HL.highByte());
                return;
            case 0x65:
                load(HL.highByte(), HL.lowByte());
                return;
            case 0x66:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                load(HL.highByte(), data);
                return;
            }
            case 0x67:
                load(HL.highByte(), AF.highByte());
                return;
            case 0x68:
                load(HL.lowByte(), BC.highByte());
                return;
            case 0x69:
                load(HL.lowByte(), BC.lowByte());
                return;
            case 0x6A:
                load(HL.lowByte(), DE.highByte());
                return;
            case 0x6B:
                load(HL.lowByte(), DE.lowByte());
                return;
            case 0x6C:
                load(HL.lowByte(), HL.highByte());
                return;
            case 0x6D:
                load(HL.lowByte(), HL.lowByte());
                return;
            case 0x6E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                load(HL.lowByte(), data);
                return;
            }
            case 0x6F:
                load(HL.lowByte(), AF.highByte());
                return;
            case 0x70:
            {
                uint16_t address = HL.read();
                load(address, BC.highByte());
                return;
            }
            case 0x71:
            {
                uint16_t address = HL.read();
                load(address, BC.lowByte());
                return;
            }
            case 0x72:
            {
                uint16_t address = HL.read();
                load(address, DE.highByte());
                return;
            }
            case 0x73:
            {
                uint16_t address = HL.read();
                load(address, DE.lowByte());
                return;
            }
            case 0x74:
            {
                uint16_t address = HL.read();
                load(address, HL.highByte());
                return;
            }
            case 0x75:
            {
                uint16_t address = HL.read();
                load(address, HL.lowByte());
                return;
            }
            case 0x76:
                halt();
                return;
            case 0x77:
            {
                uint16_t address = HL.read();
                load(address, AF.highByte());
                return;
            }
            case 0x78:
                load(AF.highByte(), BC.highByte());
                return;
            case 0x79:
                load(AF.highByte(), BC.lowByte());
                return;
            case 0x7A:
                load(AF.highByte(), DE.highByte());
                return;
            case 0x7B:
                load(AF.highByte(), DE.lowByte());
                return;
            case 0x7C: // LD A, H - 1/4t
                load(AF.highByte(), HL.highByte());
                return;
            case 0x7D: // LD A, L - 1/4t
                load(AF.highByte(), HL.lowByte());
                return;
            case 0x7E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                load(AF.highByte(), data);
                return;
            }
            case 0x7F:
                load(AF.highByte(), AF.highByte());
                return;
            case 0x80:
                alu->add(BC.highByte());
                return;
            case 0x81:
                alu->add(BC.lowByte());
                return;
            case 0x82:
                alu->add(DE.highByte());
                return;
            case 0x83:
                alu->add(DE.lowByte());
                return;
            case 0x84:
                alu->add(HL.highByte());
                return;
            case 0x85:
                alu->add(HL.lowByte());
                return;
            case 0x86:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->add(data);
                return;
            }
            case 0x87:
                alu->add(AF.highByte());
                return;
            case 0x88:
                alu->adc(BC.highByte());
                return;
            case 0x89:
                alu->adc(BC.lowByte());
                return;
            case 0x8A:
                alu->adc(DE.highByte());
                return;
            case 0x8B:
                alu->adc(DE.lowByte());
                return;
            case 0x8C:
                alu->adc(HL.highByte());
                return;
            case 0x8D:
                alu->adc(HL.lowByte());
                return;
            case 0x8E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->adc(data);
                return;
            }
            case 0x8F:
                alu->adc(AF.highByte());
                return;
            case 0x90:
                alu->subtract(BC.highByte());
                return;
            case 0x91:
                alu->subtract(BC.lowByte());
                return;
            case 0x92:
                alu->subtract(DE.highByte());
                return;
            case 0x93:
                alu->subtract(DE.lowByte());
                return;
            case 0x94:
                alu->subtract(HL.highByte());
                return;
            case 0x95:
                alu->subtract(HL.lowByte());
                return;
            case 0x96:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->subtract(data);
                return;
            }
            case 0x97:
                alu->subtract(AF.highByte());
                return;
            case 0x98:
                alu->subtractCarry(BC.highByte());
                return;
            case 0x99:
                alu->subtractCarry(BC.lowByte());
                return;
            case 0x9A:
                alu->subtractCarry(DE.highByte());
                return;
            case 0x9B:
                alu->subtractCarry(DE.lowByte());
                return;
            case 0x9C:
                alu->subtractCarry(HL.highByte());
                return;
            case 0x9D:
                alu->subtractCarry(HL.lowByte());
                return;
            case 0x9E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->subtractCarry(data);
                return;
            }
            case 0x9F:
                alu->subtractCarry(AF.highByte());
                return;
            case 0xA0:
                alu->and_(BC.highByte());
                return;
            case 0xA1:
                alu->and_(BC.lowByte());
                return;
            case 0xA2:
                alu->and_(DE.highByte());
                return;
            case 0xA3:
                alu->and_(DE.lowByte());
                return;
            case 0xA4:
                alu->and_(HL.highByte());
                return;
            case 0xA5:
                alu->and_(HL.lowByte());
                return;
            case 0xA6:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->and_(data);
                return;
            }
            case 0xA7:
                alu->and_(AF.highByte());
                return;
            case 0xA8:
                alu->xor_(BC.highByte());
                return;
            case 0xA9:
                alu->xor_(BC.lowByte());
                return;
            case 0xAA:
                alu->xor_(DE.highByte());
                return;
            case 0xAB:
                alu->xor_(DE.lowByte());
                return;
            case 0xAC:
                alu->xor_(HL.highByte());
                return;
            case 0xAD:
                alu->xor_(HL.lowByte());
                return;
            case 0xAE:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->xor_(data);
                return;
            }
            case 0xAF:
                alu->xor_(AF.highByte());
                return;
            case 0xB0:
                alu->or_(BC.highByte());
                return;
            case 0xB1:
                alu->or_(BC.lowByte());
                return;
            case 0xB2:
                alu->or_(DE.highByte());
                return;
            case 0xB3:
                alu->or_(DE.lowByte());
                return;
            case 0xB4:
                alu->or_(HL.highByte());
                return;
            case 0xB5:
                alu->or_(HL.lowByte());
                return;
            case 0xB6:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->or_(data);
                return;
            }
            case 0xB7:
                alu->or_(AF.highByte());
                return;
            case 0xB8:
                alu->compare(BC.highByte());
                return;
            case 0xB9:
                alu->compare(BC.lowByte());
                return;
            case 0xBA:
                alu->compare(DE.highByte());
                return;
            case 0xBB:
                alu->compare(DE.lowByte());
                return;
            case 0xBC:
                alu->compare(HL.highByte());
                return;
            case 0xBD:
                alu->compare(HL.lowByte());
                return;
            case 0xBE:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->compare(data);
                return;
            }
            case 0xBF:
                alu->compare(AF.highByte());
                return;
            case 0xC1:
                printf("\nBefore : %02X\n", BC.read());
                BC = mmuPtr->pop(SP);
                printf("After : %02X\n", BC.read());
                return;
            case 0xC2:
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                if (!checkFlag(CPUFlags::z))
                {
                    jump(a16);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0xC3: // JP a16 - 3/16t
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                jump(a16);
                return;
            }
            case 0xC4:
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                if (!checkFlag(CPUFlags::z))
                {
                    call(a16);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0xC5:
                printf("\nBC : %02x\n", BC.read());
                mmuPtr->push(SP, BC);
                return;
            case 0xC6:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                alu->add(n8);
                return;
            }
            case 0xC7:
                rst(0x00);
                return;
            case 0xC8:
                if (checkFlag(CPUFlags::z))
                {
                    ret();
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            case 0xC9: // RET - 1/16t
                ret();
                return;
            case 0xCA:
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                if (checkFlag(CPUFlags::z))
                {
                    jump(a16);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0xCC:
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                if (checkFlag(CPUFlags::z))
                {
                    call(a16);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0xCD: // CALL a16 - 3/24t
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                call(a16);
                return;
            }
            case 0xCE:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                alu->adc(n8);
                return;
            }
            case 0xCF:
                rst(0x08);
                return;
            case 0xD0:
                if (!checkFlag(CPUFlags::c))
                {
                    ret();
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            case 0xD1:
                printf("\nBefore : %02X\n", DE.read());
                DE = mmuPtr->pop(SP);
                printf("After : %02X\n", DE.read());
                return;
            case 0xD2:
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                if (!checkFlag(CPUFlags::c))
                {
                    jump(a16);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0xD4:
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                if (!checkFlag(CPUFlags::c))
                {
                    call(a16);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0xD5:
                printf("\nDE : %02x\n", DE.read());
                mmuPtr->push(SP, DE);
                return;
            case 0xD6:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                alu->subtract(n8);
                return;
            }
            case 0xD7:
                rst(0x10);
                return;
            case 0xD8:
                if (checkFlag(CPUFlags::c))
                {
                    ret();
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            case 0xD9:
                reti();
                return;
            case 0xDA:
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                if (checkFlag(CPUFlags::c))
                {
                    jump(a16);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0xDC:
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                if (checkFlag(CPUFlags::c))
                {
                    call(a16);
                }
                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }
                return;
            }
            case 0xDE:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                alu->subtractCarry(n8);
                return;
            }
            case 0xDF:
                rst(0x18);
                return;
            case 0xE0: // LD ($FF00+a8),A
            {
                uint8_t a8 = mmuPtr->read8(PC++);
                uint16_t address = 0xFF00 | a8;
                printf("\nFF00 | DATA : %02X -- A: %02x\n", address, AF.highByte().read());
                load(address, AF.highByte());
                return;
            }
            case 0xE1: // POP HL
                printf("\nBefore : %02X\n", HL.read());
                HL = mmuPtr->pop(SP);
                printf("After : %02X\n", HL.read());
                return;
            case 0xE2:
            {
                uint8_t C = BC.lowByte().read();
                uint16_t address = 0xFF00 | C;
                load(address, AF.highByte());
                return;
            }
            case 0xE5: // PUSH HL
                printf("\nHL : %02x\n", HL.read());
                mmuPtr->push(SP, HL);
                return;
            case 0xE6:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                alu->and_(n8);
                return;
            }
            case 0xE7:
                rst(0x20);
                return;
            case 0xE8:
            {
                uint16_t data =  mmuPtr->read8(PC++);
                alu->addToStack(data);
                return;
            }
            case 0xE9:
                jump(HL);
                return;
            case 0xEA: // LD [a16], A - 3/16t
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                load(a16, AF.highByte());
                return;
            }
            case 0xEE:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                alu->xor_(n8);
                return;
            }
            case 0xEF:
                rst(0x28);
                return;
            case 0xF0: // LD A, ($FF00+a8)
            {
                uint8_t a8 = mmuPtr->read8(PC++);
                uint16_t address = 0xFF00 | a8;
                uint8_t data = mmuPtr->read8(address);
                load(AF.highByte(), data);
                return;
            }
            case 0xF1:
                printf("\nBefore : %02X\n", AF.read());
                AF = mmuPtr->pop(SP) & 0xFFF0;
                printf("After : %02X\n", AF.read());
                return;
            case 0xF2:
            {
                uint8_t C = BC.lowByte().read();
                uint16_t address = 0xFF00 | C;
                uint8_t data = mmuPtr->read8(address);
                load(AF.highByte(), data);
                return;
            }
            case 0xF3: // DI - 1/4t
                di();
                return;
            case 0xF5: // PUSH AF
                printf("\nAF : %02x\n", AF.read());
                mmuPtr->push(SP, AF);
                return;
            case 0xF6:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                alu->or_(n8);
                return;
            }
            case 0xF7:
                rst(0x30);
                return;
            case 0xF8:
            {
                uint16_t e8 = mmuPtr->read8(PC++);
                uint16_t sp_e8 = SP.read() + static_cast<int8_t>(e8);

                printf("\nSP+e8 : %04x\n", (SP.read() & 0x0F) + (e8 & 0x0F));
                (SP.read() & 0x0F) + (e8 & 0x0F) > 0x0F ? setFlag(CPUFlags::h): resetFlag(CPUFlags::h);
                printf("\nSP+e8 : %04x\n", (SP.read() & 0xFF) + (e8 & 0xFF));
                (SP.read() & 0xFF) + (e8 & 0xFF) > 0xFF ? setFlag(CPUFlags::c): resetFlag(CPUFlags::c);

                load(HL, sp_e8);

                resetFlag(CPUFlags::z);
                resetFlag(CPUFlags::n);
                return;
            }
            case 0xF9:
                load(SP, HL.read());
                return;
            case 0xFA:
            {
                uint16_t address = PC.read();
                uint16_t a16 = mmuPtr->read16(address);
                PC += 2;
                uint8_t data = mmuPtr->read8(a16);
                load(AF.highByte(), data);
                return;
            }
            case 0xFB:
                ei();
                return;
            case 0xFE:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                alu->compare(n8);
                return;
            }
            case 0xFF:
                rst(0x38);
                return;
            default:
                exit(-1);
        }
    }

    void CPU::decodeExtendedInstructions()
    {
        currentOpcode = mmuPtr->read8(PC++);
        switch (currentOpcode)
        {
            case 0x00:
                alu->rlc(BC.highByte());
                break;
            case 0x01:
                alu->rlc(BC.lowByte());
                break;
            case 0x02:
                alu->rlc(DE.highByte());
                break;
            case 0x03:
                alu->rlc(DE.lowByte());
                break;
            case 0x04:
                alu->rlc(HL.highByte());
                break;
            case 0x05:
                alu->rlc(HL.lowByte());
                break;
            case 0x06:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->rlc(data);
                mmuPtr->write8(address, data);
                break;
            }
            case 0x07:
                alu->rlc(AF.highByte());
                break;
            case 0x08:
                alu->rrc(BC.highByte());
                return;
            case 0x09:
                alu->rrc(BC.lowByte());
                break;
            case 0x0A:
                alu->rrc(DE.highByte());
                return;
            case 0x0B:
                alu->rrc(DE.lowByte());
                return;
            case 0x0C:
                alu->rrc(HL.highByte());
                return;
            case 0x0D:
                alu->rrc(HL.lowByte());
                return;
            case 0x0E: {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->rrc(data);
                mmuPtr->write8(address, data);
                break;
            }
            case 0x0F:
                alu->rrc(AF.highByte());
                return;

            case 0x10:
                alu->rl(BC.highByte());
                break;
            case 0x11:
                alu->rl(BC.lowByte());
                break;
            case 0x12:
                alu->rl(DE.highByte());
                break;
            case 0x13:
                alu->rl(DE.lowByte());
                break;
            case 0x14:
                alu->rl(HL.highByte());
                break;
            case 0x15:
                alu->rl(HL.lowByte());
                break;
            case 0x16:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->rl(data);
                mmuPtr->write8(address, data);
                break;
            }
            case 0x17:
                alu->rl(AF.highByte());
                break;
            case 0x18:
                alu->rr(BC.highByte());
                return;
            case 0x19:
                alu->rr(BC.lowByte());
                break;
            case 0x1A:
                alu->rr(DE.highByte());
                return;
            case 0x1B:
                alu->rr(DE.lowByte());
                return;
            case 0x1C:
                alu->rr(HL.highByte());
                return;
            case 0x1D:
                alu->rr(HL.lowByte());
                return;
            case 0x1E: {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->rr(data);
                mmuPtr->write8(address, data);
                break;
            }
            case 0x1F:
                alu->rr(AF.highByte());
                return;
            case 0x20:
                alu->sla(BC.highByte());
                return;
            case 0x21:
                alu->sla(BC.lowByte());
                return;
            case 0x22:
                alu->sla(DE.highByte());
                return;
            case 0x23:
                alu->sla(DE.lowByte());
                return;
            case 0x24:
                alu->sla(HL.highByte());
                return;
            case 0x25:
                alu->sla(HL.lowByte());
                return;
            case 0x26:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->sla(data);
                mmuPtr->write8(address, data);
            }
            case 0x27:
                alu->sla(AF.highByte());
                return;
            case 0x28:
                alu->sra(BC.highByte());
                return;
            case 0x29:
                alu->sra(BC.lowByte());
                return;
            case 0x2A:
                alu->sra(DE.highByte());
                return;
            case 0x2B:
                alu->sra(DE.lowByte());
                return;
            case 0x2C:
                alu->sra(HL.highByte());
                return;
            case 0x2D:
                alu->sra(HL.lowByte());
                return;
            case 0x2E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->sra(data);
                mmuPtr->write8(address, data);
            }
            case 0x2F:
                alu->sra(AF.highByte());
                return;
            case 0x30:
                alu->swap(BC.highByte());
                return;
            case 0x31:
                alu->swap(BC.lowByte());
                return;
            case 0x32:
                alu->swap(DE.highByte());
                return;
            case 0x33:
                alu->swap(DE.lowByte());
                return;
            case 0x34:
                alu->swap(HL.highByte());
                return;
            case 0x35:
                alu->swap(HL.lowByte());
                return;
            case 0x36:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->swap(data);
                mmuPtr->write8(address, data);
            }
            case 0x37:
                alu->swap(AF.highByte());
                return;
            case 0x38:
                alu->srl(BC.highByte());
                return;
            case 0x39:
                alu->srl(BC.lowByte());
                return;
            case 0x3A:
                alu->srl(DE.highByte());
                return;
            case 0x3B:
                alu->srl(DE.lowByte());
                return;
            case 0x3C:
                alu->srl(HL.highByte());
                return;
            case 0x3D:
                alu->srl(HL.lowByte());
                return;
            case 0x3E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->srl(data);
                mmuPtr->write8(address, data);
            }
            case 0x3F:
                alu->srl(AF.highByte());
                return;
            case 0x40:
                alu->bit(BC.highByte(), 0);
                return;
            case 0x41:
                alu->bit(BC.lowByte(), 0);
                return;
            case 0x42:
                alu->bit(DE.highByte(), 0);
                return;
            case 0x43:
                alu->bit(DE.lowByte(), 0);
                return;
            case 0x44:
                alu->bit(HL.highByte(), 0);
                return;
            case 0x45:
                alu->bit(HL.lowByte(), 0);
                return;
            case 0x46:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 0);
            }
            case 0x47:
                alu->bit(AF.highByte(), 0);
                return;
            case 0x48:
                alu->bit(BC.highByte(), 1);
                return;
            case 0x49:
                alu->bit(BC.lowByte(), 1);
                return;
            case 0x4A:
                alu->bit(DE.highByte(), 1);
                return;
            case 0x4B:
                alu->bit(DE.lowByte(), 1);
                return;
            case 0x4C:
                alu->bit(HL.highByte(), 1);
                return;
            case 0x4D:
                alu->bit(HL.lowByte(), 1);
                return;
            case 0x4E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 1);
            }
            case 0x4F:
                alu->bit(AF.highByte(), 1);
                return;

            case 0x50:
                alu->bit(BC.highByte(), 2);
                return;
            case 0x51:
                alu->bit(BC.lowByte(), 2);
                return;
            case 0x52:
                alu->bit(DE.highByte(), 2);
                return;
            case 0x53:
                alu->bit(DE.lowByte(), 2);
                return;
            case 0x54:
                alu->bit(HL.highByte(), 2);
                return;
            case 0x55:
                alu->bit(HL.lowByte(), 2);
                return;
            case 0x56:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 2);
            }
            case 0x57:
                alu->bit(AF.highByte(), 2);
                return;
            case 0x58:
                alu->bit(BC.highByte(), 3);
                return;
            case 0x59:
                alu->bit(BC.lowByte(), 3);
                return;
            case 0x5A:
                alu->bit(DE.highByte(), 3);
                return;
            case 0x5B:
                alu->bit(DE.lowByte(), 3);
                return;
            case 0x5C:
                alu->bit(HL.highByte(), 3);
                return;
            case 0x5D:
                alu->bit(HL.lowByte(), 3);
                return;
            case 0x5E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 3);
            }
            case 0x5F:
                alu->bit(AF.highByte(), 3);
                return;


            case 0x60:
                alu->bit(BC.highByte(), 4);
                return;
            case 0x61:
                alu->bit(BC.lowByte(), 4);
                return;
            case 0x62:
                alu->bit(DE.highByte(), 4);
                return;
            case 0x63:
                alu->bit(DE.lowByte(), 4);
                return;
            case 0x64:
                alu->bit(HL.highByte(), 4);
                return;
            case 0x65:
                alu->bit(HL.lowByte(), 4);
                return;
            case 0x66:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 4);
            }
            case 0x67:
                alu->bit(AF.highByte(), 4);
                return;
            case 0x68:
                alu->bit(BC.highByte(), 5);
                return;
            case 0x69:
                alu->bit(BC.lowByte(), 5);
                return;
            case 0x6A:
                alu->bit(DE.highByte(), 5);
                return;
            case 0x6B:
                alu->bit(DE.lowByte(), 5);
                return;
            case 0x6C:
                alu->bit(HL.highByte(), 5);
                return;
            case 0x6D:
                alu->bit(HL.lowByte(), 5);
                return;
            case 0x6E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 5);
            }
            case 0x6F:
                alu->bit(AF.highByte(), 5);
                return;

            case 0x70:
                alu->bit(BC.highByte(), 6);
                return;
            case 0x71:
                alu->bit(BC.lowByte(), 6);
                return;
            case 0x72:
                alu->bit(DE.highByte(), 6);
                return;
            case 0x73:
                alu->bit(DE.lowByte(), 6);
                return;
            case 0x74:
                alu->bit(HL.highByte(), 6);
                return;
            case 0x75:
                alu->bit(HL.lowByte(), 6);
                return;
            case 0x76:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 6);
            }
            case 0x77:
                alu->bit(AF.highByte(), 6);
                return;
            case 0x78:
                alu->bit(BC.highByte(), 7);
                return;
            case 0x79:
                alu->bit(BC.lowByte(), 7);
                return;
            case 0x7A:
                alu->bit(DE.highByte(), 7);
                return;
            case 0x7B:
                alu->bit(DE.lowByte(), 7);
                return;
            case 0x7C:
                alu->bit(HL.highByte(), 7);
                return;
            case 0x7D:
                alu->bit(HL.lowByte(), 7);
                return;
            case 0x7E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 7);
            }
            case 0x7F:
                alu->bit(AF.highByte(), 7);
                return;


            case 0x80:
                alu->res(BC.highByte(), 0);
                return;
            case 0x81:
                alu->res(BC.lowByte(), 0);
                return;
            case 0x82:
                alu->res(DE.highByte(), 0);
                return;
            case 0x83:
                alu->res(DE.lowByte(), 0);
                return;
            case 0x84:
                alu->res(HL.highByte(), 0);
                return;
            case 0x85:
                alu->res(HL.lowByte(), 0);
                return;
            case 0x86:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 0);
                mmuPtr->write8(address, data);
            }
            case 0x87:
                alu->res(AF.highByte(), 0);
                return;
            case 0x88:
                alu->res(BC.highByte(), 1);
                return;
            case 0x89:
                alu->res(BC.lowByte(), 1);
                return;
            case 0x8A:
                alu->res(DE.highByte(), 1);
                return;
            case 0x8B:
                alu->res(DE.lowByte(), 1);
                return;
            case 0x8C:
                alu->res(HL.highByte(), 1);
                return;
            case 0x8D:
                alu->res(HL.lowByte(), 1);
                return;
            case 0x8E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 1);
                mmuPtr->write8(address, data);
            }
            case 0x8F:
                alu->res(AF.highByte(), 1);
                return;


            case 0x90:
                alu->res(BC.highByte(), 2);
                return;
            case 0x91:
                alu->res(BC.lowByte(), 2);
                return;
            case 0x92:
                alu->res(DE.highByte(), 2);
                return;
            case 0x93:
                alu->res(DE.lowByte(), 2);
                return;
            case 0x94:
                alu->res(HL.highByte(), 2);
                return;
            case 0x95:
                alu->res(HL.lowByte(), 2);
                return;
            case 0x96:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 2);
                mmuPtr->write8(address, data);
            }
            case 0x97:
                alu->res(AF.highByte(), 2);
                return;
            case 0x98:
                alu->res(BC.highByte(), 3);
                return;
            case 0x99:
                alu->res(BC.lowByte(), 3);
                return;
            case 0x9A:
                alu->res(DE.highByte(), 3);
                return;
            case 0x9B:
                alu->res(DE.lowByte(), 3);
                return;
            case 0x9C:
                alu->res(HL.highByte(), 3);
                return;
            case 0x9D:
                alu->res(HL.lowByte(), 3);
                return;
            case 0x9E:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 3);
                mmuPtr->write8(address, data);
            }
            case 0x9F:
                alu->res(AF.highByte(), 3);
                return;


            case 0xA0:
                alu->res(BC.highByte(), 4);
                return;
            case 0xA1:
                alu->res(BC.lowByte(), 4);
                return;
            case 0xA2:
                alu->res(DE.highByte(), 4);
                return;
            case 0xA3:
                alu->res(DE.lowByte(), 4);
                return;
            case 0xA4:
                alu->res(HL.highByte(), 4);
                return;
            case 0xA5:
                alu->res(HL.lowByte(), 4);
                return;
            case 0xA6:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 4);
                mmuPtr->write8(address, data);
            }
            case 0xA7:
                alu->res(AF.highByte(), 4);
                return;
            case 0xA8:
                alu->res(BC.highByte(), 5);
                return;
            case 0xA9:
                alu->res(BC.lowByte(), 5);
                return;
            case 0xAA:
                alu->res(DE.highByte(), 5);
                return;
            case 0xAB:
                alu->res(DE.lowByte(), 5);
                return;
            case 0xAC:
                alu->res(HL.highByte(), 5);
                return;
            case 0xAD:
                alu->res(HL.lowByte(), 5);
                return;
            case 0xAE:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 5);
                mmuPtr->write8(address, data);
            }
            case 0xAF:
                alu->res(AF.highByte(), 5);
                return;
            case 0xB0:
                alu->res(BC.highByte(), 6);
                return;
            case 0xB1:
                alu->res(BC.lowByte(), 6);
                return;
            case 0xB2:
                alu->res(DE.highByte(), 6);
                return;
            case 0xB3:
                alu->res(DE.lowByte(), 6);
                return;
            case 0xB4:
                alu->res(HL.highByte(), 6);
                return;
            case 0xB5:
                alu->res(HL.lowByte(), 6);
                return;
            case 0xB6:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 6);
                mmuPtr->write8(address, data);
            }
            case 0xB7:
                alu->res(AF.highByte(), 6);
                return;
            case 0xB8:
                alu->res(BC.highByte(), 7);
                return;
            case 0xB9:
                alu->res(BC.lowByte(), 7);
                return;
            case 0xBA:
                alu->res(DE.highByte(), 7);
                return;
            case 0xBB:
                alu->res(DE.lowByte(), 7);
                return;
            case 0xBC:
                alu->res(HL.highByte(), 7);
                return;
            case 0xBD:
                alu->res(HL.lowByte(), 7);
                return;
            case 0xBE:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 7);
                mmuPtr->write8(address, data);
            }
            case 0xBF:
                alu->res(AF.highByte(), 7);
                return;

            case 0xC0:
                alu->set(BC.highByte(), 0);
                return;
            case 0xC1:
                alu->set(BC.lowByte(), 0);
                return;
            case 0xC2:
                alu->set(DE.highByte(), 0);
                return;
            case 0xC3:
                alu->set(DE.lowByte(), 0);
                return;
            case 0xC4:
                alu->set(HL.highByte(), 0);
                return;
            case 0xC5:
                alu->set(HL.lowByte(), 0);
                return;
            case 0xC6:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 0);
                mmuPtr->write8(address, data);
            }
            case 0xC7:
                alu->set(AF.highByte(), 0);
                return;
            case 0xC8:
                alu->set(BC.highByte(), 1);
                return;
            case 0xC9:
                alu->set(BC.lowByte(), 1);
                return;
            case 0xCA:
                alu->set(DE.highByte(), 1);
                return;
            case 0xCB:
                alu->set(DE.lowByte(), 1);
                return;
            case 0xCC:
                alu->set(HL.highByte(), 1);
                return;
            case 0xCD:
                alu->set(HL.lowByte(), 1);
                return;
            case 0xCE:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 1);
                mmuPtr->write8(address, data);
            }
            case 0xCF:
                alu->set(AF.highByte(), 1);
                return;


            case 0xD0:
                alu->set(BC.highByte(), 2);
                return;
            case 0xD1:
                alu->set(BC.lowByte(), 2);
                return;
            case 0xD2:
                alu->set(DE.highByte(), 2);
                return;
            case 0xD3:
                alu->set(DE.lowByte(), 2);
                return;
            case 0xD4:
                alu->set(HL.highByte(), 2);
                return;
            case 0xD5:
                alu->set(HL.lowByte(), 2);
                return;
            case 0xD6:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 2);
                mmuPtr->write8(address, data);
            }
            case 0xD7:
                alu->set(AF.highByte(), 2);
                return;
            case 0xD8:
                alu->set(BC.highByte(), 3);
                return;
            case 0xD9:
                alu->set(BC.lowByte(), 3);
                return;
            case 0xDA:
                alu->set(DE.highByte(), 3);
                return;
            case 0xDB:
                alu->set(DE.lowByte(), 3);
                return;
            case 0xDC:
                alu->set(HL.highByte(), 3);
                return;
            case 0xDD:
                alu->set(HL.lowByte(), 3);
                return;
            case 0xDE:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 3);
                mmuPtr->write8(address, data);
            }
            case 0xDF:
                alu->set(AF.highByte(), 3);
                return;


            case 0xE0:
                alu->set(BC.highByte(), 4);
                return;
            case 0xE1:
                alu->set(BC.lowByte(), 4);
                return;
            case 0xE2:
                alu->set(DE.highByte(), 4);
                return;
            case 0xE3:
                alu->set(DE.lowByte(), 4);
                return;
            case 0xE4:
                alu->set(HL.highByte(), 4);
                return;
            case 0xE5:
                alu->set(HL.lowByte(), 4);
                return;
            case 0xE6:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 4);
                mmuPtr->write8(address, data);
            }
            case 0xE7:
                alu->set(AF.highByte(), 4);
                return;
            case 0xE8:
                alu->set(BC.highByte(), 5);
                return;
            case 0xE9:
                alu->set(BC.lowByte(), 5);
                return;
            case 0xEA:
                alu->set(DE.highByte(), 5);
                return;
            case 0xEB:
                alu->set(DE.lowByte(), 5);
                return;
            case 0xEC:
                alu->set(HL.highByte(), 5);
                return;
            case 0xED:
                alu->set(HL.lowByte(), 5);
                return;
            case 0xEE:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 5);
                mmuPtr->write8(address, data);
            }
            case 0xEF:
                alu->set(AF.highByte(), 5);
                return;
            case 0xF0:
                alu->set(BC.highByte(), 6);
                return;
            case 0xF1:
                alu->set(BC.lowByte(), 6);
                return;
            case 0xF2:
                alu->set(DE.highByte(), 6);
                return;
            case 0xF3:
                alu->set(DE.lowByte(), 6);
                return;
            case 0xF4:
                alu->set(HL.highByte(), 6);
                return;
            case 0xF5:
                alu->set(HL.lowByte(), 6);
                return;
            case 0xF6:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 6);
                mmuPtr->write8(address, data);
            }
            case 0xF7:
                alu->set(AF.highByte(), 6);
                return;
            case 0xF8:
                alu->set(BC.highByte(), 7);
                return;
            case 0xF9:
                alu->set(BC.lowByte(), 7);
                return;
            case 0xFA:
                alu->set(DE.highByte(), 7);
                return;
            case 0xFB:
                alu->set(DE.lowByte(), 7);
                return;
            case 0xFC:
                alu->set(HL.highByte(), 7);
                return;
            case 0xFD:
                alu->set(HL.lowByte(), 7);
                return;
            case 0xFE:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 7);
                mmuPtr->write8(address, data);
            }
            case 0xFF:
                alu->set(AF.highByte(), 7);
                return;
            default:
                printf("%#02x -> INVALID INSTRUCTION!", currentOpcode);
                exit(-1);
        }

    }
}