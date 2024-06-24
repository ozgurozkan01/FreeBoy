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
        // Somethings are wrong in instructions
        // NO problem until 1CFFFF
        // Problem in 0x001DB68A
/*        if (gameBoyPtr->ticks == 0x00028ADF)
            exit(-1);*/
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
        //printf("\nBefore : %02X\n", mmuPtr->read16(_address));
        mmuPtr->write16(_address, _value);
        //printf("After : %02X\n", mmuPtr->read16(_address));
    }

    void CPU::setFlag(const CPUFlags _flag) { AF.lowByte() |= (1 << static_cast<uint8_t>(_flag)); }
    void CPU::resetFlag(const CPUFlags _flag) { AF.lowByte() &= ~(1 << static_cast<uint8_t>(_flag)); }
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

    void CPU::di() { interruptHandlerPtr->setIME(false); }
    void CPU::ei() { interruptHandlerPtr->setIME(true); }


    void CPU::ret()
    {
        printf("\nBefore : %02X\n", PC.read());
        PC = mmuPtr->pop(SP);
       printf("After : %02X\n", PC.read());
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
            case 0x0E:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                load(BC.lowByte(), n8);
                return;
            }
            case 0x11:
            {
                uint16_t n16 = mmuPtr->read16(PC.read());
                PC += 2;
                load(DE, n16);
                return;
            }
            case 0x13:
                alu->increment(DE);
                return;
            case 0x18: // JR e8 - 2/12t
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(PC++));
                relativeJump(e8);
                return;
            }
            case 0x1A:
            {
                uint16_t data = mmuPtr->read16(DE.read());
                load(AF.highByte(), data);
                return;
            }
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
                uint16_t data = mmuPtr->read16(PC.read());
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
            case 0x2C:
                alu->increment(HL.lowByte());
                return;
            case 0x2D:
                alu->decrement(HL.lowByte());
                return;
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
                load(SP, mmuPtr->read16(PC.read()));
                PC += 2;
                return;
            case 0x32:
            {
                uint16_t address = HL--;
                load(address, AF.highByte());
                return;
            }
            case 0x35:
            {
                uint16_t address = HL.read();
                uint8_t data = mmuPtr->read8(address);
                alu->decrement(data);
                mmuPtr->write8(address, data);
                return;
            }
            case 0x3C:
                alu->increment(AF.highByte());
                return;
            case 0x3D:
                alu->decrement(AF.highByte());
                return;
            case 0x3E: // LD A,n8 - 2/8t
                load(AF.highByte(), mmuPtr->read8(PC++));
                return;
            case 0x46:
            {
                uint8_t data = mmuPtr->read8(HL.read());
                load(BC.highByte(), data);
                return;
            }
            case 0x47:
                load(BC.highByte(), AF.highByte());
                return;
            case 0x4E:
            {
                uint8_t data = mmuPtr->read8(HL.read());
                load(BC.lowByte(), data);
                return;
            }
            case 0x4F:
                load(BC.lowByte(), AF.highByte());
                return;
            case 0x56:
            {
                uint8_t data = mmuPtr->read8(HL.read());
                load(DE.highByte(), data);
                return;
            }
            case 0x57:
                load(DE.highByte(), AF.highByte());
                return;
            case 0x5F:
                load(DE.lowByte(), AF.highByte());
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
            case 0x7B:
                load(AF.highByte(), DE.lowByte());
                return;
            case 0x7C: // LD A, H - 1/4t
                load(AF.highByte(), HL.highByte());
                return;
            case 0x7D: // LD A, L - 1/4t
                load(AF.highByte(), HL.lowByte());
                return;
            case 0x7A:
                load(AF.highByte(), DE.highByte());
                return;
            case 0x81:
                alu->add(BC.lowByte());
                return;
            case 0x91:
                alu->subtract(BC.lowByte());
                return;
            case 0xA9:
                alu->xor_(BC.lowByte());
                return;
            case 0xAE:
            {
                uint8_t data = mmuPtr->read8(HL.read());
                alu->xor_(data);
                return;
            }
            case 0xB1:
                alu->or_(BC.lowByte());
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
            case 0xC1:
                printf("\nBefore : %02X\n", BC.read());
                BC = mmuPtr->pop(SP);
                printf("After : %02X\n", BC.read());
                return;
            case 0xC2:
            {
                uint16_t a16 = mmuPtr->read16(PC.read());
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
                uint16_t a16 = mmuPtr->read16(PC.read());
                PC += 2;
                jump(a16);
                return;
            }
            case 0xC4:
            {
                uint16_t a16 = mmuPtr->read16(PC.read());
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
            case 0xCD: // CALL a16 - 3/24t
            {
                uint16_t a16 = mmuPtr->read16(PC.read());
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
            case 0xE0: // LD ($FF00+a8),A
            {
                uint8_t a8 = mmuPtr->read8(PC++);
                printf("\nFF00 | DATA : %02X\n", 0xFF00 | a8);
                uint16_t address = 0xFF00 | a8;
                load(address, AF.highByte());
                return;
            }
            case 0xE1: // POP HL
                printf("\nBefore : %02X\n", HL.read());
                HL = mmuPtr->pop(SP);
                printf("After : %02X\n", HL.read());
                return;
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
            case 0xE9:
                jump(HL);
                return;
            case 0xEA: // LD [a16], A - 3/16t
            {
                uint16_t a16 = mmuPtr->read16(PC.read());
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
            case 0xF8:
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(PC++));
                uint16_t sp_e8 = SP.read() + e8;
                load(HL, sp_e8);
                return;
            }
            case 0xFA:
            {
                uint16_t a16 = mmuPtr->read16(PC.read());
                PC += 2;
                uint8_t data = mmuPtr->read8(a16);
                load(AF.highByte(), data);
                return;
            }
            case 0xFE:
            {
                uint8_t n8 = mmuPtr->read8(PC++);
                alu->compare(n8);
                return;
            }
            default:
                exit(-1);
        }
    }

    void CPU::decodeExtendedInstructions()
    {
        currentOpcode = mmuPtr->read8(PC++);
        switch (currentOpcode)
        {
            case 0x19:
                alu->rr(BC.lowByte());
                return;
            case 0x1A:
                alu->rr(DE.highByte());
                return;
            case 0x37:
                alu->swap(AF.highByte());
                return;
            case 0x38:
                alu->srl(BC.highByte());
                return;
            case 0x3F:
                alu->srl(AF.highByte());
                return;
        }
        printf("%#02x -> INVALID INSTRUCTION!", currentOpcode);
        exit(-1);
    }
}