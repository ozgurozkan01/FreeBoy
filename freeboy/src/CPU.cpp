//
// Created by ozgur on 12/04/2024.
//

#include <thread>
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
            af(0x01B0), /* Acuumulator and Flags */
            bc(0x0013), de(0x00D8), hl(0x014D), /* General Purpose Registers */
            sp(0xFFFE), /* Stack Pointer */
            pc(0x0100)  /* Program Counter */
    {
        alu = new ALU(this);
    }

    CPU::~CPU()
    {
        if (cpuProcess != nullptr) { delete cpuProcess; }
        if (alu != nullptr) { delete alu; }
    }

    void CPU::step()
    {
        if (!isHalted)
        {
            fetch();
            execute();
        }

        else if (interruptHandlerPtr->getIF().read())
        {
            isHalted = false;
        }

        if (interruptHandlerPtr->getIME())
        {
            interruptHandlerPtr->requestInterrupt(this, mmuPtr);
        }
    }

    void CPU::fetch()
    {
        currentOpcode = mmuPtr->read8(pc++);
        currentInstruction = &cpuProcess->standardInstructions[currentOpcode];
        printf("\n");

        char flags[16];
        sprintf(flags, "%c%c%c%c",
                af.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::z)) ? 'Z' : '-',
                af.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::n)) ? 'N' : '-',
                af.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::h)) ? 'H' : '-',
                af.lowByte().read() & (1 << static_cast<uint8_t>(CPUFlags::c)) ? 'C' : '-'
        );

        printf("%08lX - PC : %02X, Opcode : (%02X %02X %02X) : (A : %02X - F : %s - BC : %04X - DE : %04X - HL : %04X - SP : %04X) - %s\n",
               gameBoyPtr->ticks,
               pc.read() - 1,
               currentOpcode,
               mmuPtr->read8(pc.read()),
               mmuPtr->read8(pc.read() + 1),
               af.highByte().read(),
               flags,
               bc.read(),
               de.read(),
               hl.read(),
               sp.read(),
               currentInstruction->nmenomic.c_str());
    }

    void CPU::execute()
    {
        emulateCycles(currentInstruction->mCycles);

        if (currentOpcode != 0xCB)  { decodeStandardInstructions(); }
        else                        { decodeExtendedInstructions(); }
    }

    void CPU::emulateCycles(uint8_t cycleCount) { gameBoyPtr->emulateCycles(cycleCount); }

    void CPU::load(Register16 &_register, const uint16_t _value)
    {
        //printf("\nBefore : %02X\n", _register.read());
        _register = _value;
        //printf("After : %02X\n", _register.read());
    }
    void CPU::load(const uint16_t _address, const Register8 &_register)
    {
        //printf("\nBefore : %02X\n", mmuPtr->read8(_address));
        mmuPtr->write8(_address, _register.read());
        //printf("After : %02X\n", mmuPtr->read8(_address));
    }
    void CPU::load(Register8 &_register, const uint8_t _value)
    {
        //printf("\nBefore : %02X\n", _register.read());
        _register = _value;
        //printf("After : %02X\n", _register.read());
    }
    void CPU::load(Register8 &_dstRegister, const Register8 &_srcRegister)
    {
        //printf("\nBefore : %02X\n", _dstRegister.read());
        _dstRegister = _srcRegister;
        //printf("After : %02X\n", _dstRegister.read());
    }
    void CPU::load(const uint16_t _address, const uint16_t _value)
    {
        //printf("\nBefore : %02X\n", mmuPtr->read16(_address));
        mmuPtr->write16(_address, _value);
        //printf("After : %02X\n", mmuPtr->read16(_address));
    }

    void CPU::setFlag(const CPUFlags _flag) { af.lowByte() |= (1 << static_cast<uint8_t>(_flag)); }
    void CPU::resetFlag(const CPUFlags _flag) { af.lowByte() &= ~(1 << static_cast<uint8_t>(_flag)); }
    void CPU::toggleFlag(const CPUFlags _flag) { af.lowByte() ^= (1 << static_cast<uint8_t>(_flag)); };
    uint8_t CPU::readFlag(const CPUFlags _flag) { return (af.lowByte().read() >> static_cast<uint8_t>(_flag)) & 0x1; }
    bool CPU::checkFlag(const CPUFlags _flag) { return (af.lowByte().read() >> static_cast<uint8_t>(_flag)) & 0x1; }

    void CPU::jump(const uint16_t _value)
    {
        //printf("\nBefore : %02X\n", PC.read());
        pc = _value;
        //printf("After : %02X\n", PC.read());
    }

    void CPU::jump(const Register16 &_register)
    {
        //printf("\nBefore : %02X\n", PC.read());
        pc = _register;
        //printf("After : %02X\n", PC.read());
    }

    void CPU::relativeJump(const uint16_t _data)
    {
        //printf("\nBefore : %02X\n", PC.read());
        pc = pc.read() + _data;
        //printf("After : %02X\n", PC.read());
    }

    void CPU::call(const uint16_t _value)
    {
        //printf("\nBefore : %02X\n", PC.read());
        mmuPtr->push(sp, pc);
        pc = _value;
        //printf("After : %02X\n", PC.read());
    }

    void CPU::rst(const uint8_t vector)
    {
        mmuPtr->push(sp, pc);
        pc = vector;
    }

    void CPU::di() { interruptHandlerPtr->setIME(false); }
    void CPU::ei() { interruptHandlerPtr->setIME(true); }

    void CPU::ret()
    {
        //printf("\nBefore : %02X\n", PC.read());
        pc = mmuPtr->pop(sp);
        //printf("After : %02X\n", PC.read());
    }

    void CPU::reti()
    {
        ret();
        ei();
    }

    void CPU::halt() { isHalted = true; }

    void CPU::decodeStandardInstructions()
    {
        switch (currentOpcode)
        {
            case 0x00: // NOP - 1/4t
                return;
            case 0x01: // LD BC, n16
            {
                uint16_t n16 = mmuPtr->read16(pc.read());
                pc += 2;
                load(bc, n16);
                return;
            }
            case 0x02:
            {
                uint16_t address = bc.read();
                load(address, af.highByte());
                return;
            }
            case 0x03:
                alu->increment(bc);
                return;
            case 0x04:
                alu->increment(bc.highByte());
                return;
            case 0x05:
                alu->decrement(bc.highByte());
                return;
            case 0x06:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                load(bc.highByte(), n8);
                return;
            }
            case 0x07:
                alu->rlca();
                return;
            case 0x08:
            {
                uint16_t a16 = mmuPtr->read16(pc.read());
                pc += 2;
                load(a16, sp.read());
                return;
            }
            case 0x09:
                alu->add(bc);
                return;
            case 0x0A:
            {
                uint16_t address = bc.read();
                uint8_t data = mmuPtr->read8(address);
                load(af.highByte(), data);
                return;
            }
            case 0x0B:
                alu->decrement(bc);
                return;
            case 0x0C:
                alu->increment(bc.lowByte());
                return;
            case 0x0D:
                alu->decrement(bc.lowByte());
                return;
            case 0x0E:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                load(bc.lowByte(), n8);
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
                uint16_t n16 = mmuPtr->read16(pc.read());
                pc += 2;
                load(de, n16);
                return;
            }
            case 0x12:
            {
                uint16_t address = de.read();
                load(address, af.highByte());
                return;
            }
            case 0x13:
                alu->increment(de);
                return;
            case 0x14:
                alu->increment(de.highByte());
                return;
            case 0x15:
                alu->decrement(de.highByte());
            case 0x16:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                load(de.highByte(), n8);
                return;
            }
            case 0x17:
                alu->rla();
                return;
            case 0x18: // JR e8 - 2/12t
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(pc++));
                relativeJump(e8);
                return;
            }
            case 0x19:
                alu->add(de);
                return;
            case 0x1A:
            {
                uint16_t address = de.read();
                uint8_t data = mmuPtr->read16(address);
                load(af.highByte(), data);
                return;
            }
            case 0x1B:
                alu->decrement(de);
                return;
            case 0x1C:
                alu->increment(de.lowByte());
                return;
            case 0x1D:
                alu->decrement(de.lowByte());
                return;
            case 0x1E:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                load(de.lowByte(), n8);
                return;
            }
            case 0x1F:
                alu->rra();
                return;
            case 0x20:
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(pc++));
                if (!checkFlag(CPUFlags::z))
                {
                    relativeJump(e8);
                    emulateCycles(1);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0x21: // LD HL,n16 - 3/12t
            {
                uint16_t address = pc.read();
                uint16_t data = mmuPtr->read16(address);
                pc += 2;
                load(hl, data);
                return;
            }
            case 0x22:
            {
                uint16_t address = hl++;
                load(address, af.highByte());
                return;
            }
            case 0x23:
                alu->increment(hl);
                return;
            case 0x24:
                alu->increment(hl.highByte());
                return;
            case 0x25:
                alu->decrement(hl.highByte());
                return;
            case 0x26:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                load(hl.highByte(), n8);
                return;
            }
            case 0x27:
                alu->daa();
                return;
            case 0x28:
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(pc++));
                if (checkFlag(CPUFlags::z))
                {
                    relativeJump(e8);
                    emulateCycles(1);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0x29:
                alu->add(hl);
                return;
            case 0x2A: // LD A, [HL+]
            {
                load(af.highByte(), mmuPtr->read8(hl++));
                return;
            }
            case 0x2B:
                alu->decrement(hl);
                return;
            case 0x2C:
                alu->increment(hl.lowByte());
                return;
            case 0x2D:
                alu->decrement(hl.lowByte());
                return;
            case 0x2E:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                load(hl.lowByte(), n8);
                return;
            }
            case 0x2F:
            {
                alu->complement();
                return;
            }
            case 0x30:
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(pc++));
                if (!checkFlag(CPUFlags::c))
                {
                    relativeJump(e8);
                    emulateCycles(1);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0x31: // LD SP, n16 - 3/12t
            {
                uint16_t address = pc.read();
                load(sp, mmuPtr->read16(address));
                pc += 2;
                return;
            }
            case 0x32:
            {
                uint16_t address = hl--;
                load(address, af.highByte());
                return;
            }
            case 0x33:
                alu->increment(sp);
                return;
            case 0x34:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->increment(data);
                mmuPtr->write8(address, data);
            }
            case 0x35:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->decrement(data);
                mmuPtr->write8(address, data);
                return;
            }
            case 0x36:
            {
                uint16_t address = hl.read();
                uint8_t n8 = mmuPtr->read8(pc++);
                load(address, n8);
                return;
            }
            case 0x37:
            {
                setFlag(CPUFlags::c);
                resetFlag(CPUFlags::h);
                resetFlag(CPUFlags::n);
                return;
            }
            case 0x38:
            {
                int8_t e8 = static_cast<int8_t>(mmuPtr->read8(pc++));
                if (checkFlag(CPUFlags::c))
                {
                    relativeJump(e8);
                    emulateCycles(1);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0x39:
                alu->add(sp);
                return;
            case 0x3A:
                load(af.highByte(), mmuPtr->read8(hl--));
                return;
            case 0x3B:
                alu->decrement(sp);
                return;
            case 0x3C:
                alu->increment(af.highByte());
                return;
            case 0x3D:
                alu->decrement(af.highByte());
                return;
            case 0x3F:
                alu->complementCarryFlag();
                return;
            case 0x3E: // LD A,n8 - 2/8t
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                load(af.highByte(), n8);
                return;
            }
            case 0x40:
                load(bc.highByte(), bc.highByte());
                return;
            case 0x41:
                load(bc.highByte(), bc.lowByte());
                return;
            case 0x42:
                load(bc.highByte(), de.highByte());
                return;
            case 0x43:
                load(bc.highByte(), de.lowByte());
                return;
            case 0x44:
                load(bc.highByte(), hl.highByte());
                return;
            case 0x45:
                load(bc.highByte(), hl.lowByte());
                return;
            case 0x46:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                load(bc.highByte(), data);
                return;
            }
            case 0x47:
                load(bc.highByte(), af.highByte());
                return;
            case 0x48:
                load(bc.lowByte(), bc.highByte());
                return;
            case 0x49:
                load(bc.lowByte(), bc.lowByte());
                return;
            case 0x4A:
                load(bc.lowByte(), de.highByte());
                return;
            case 0x4B:
                load(bc.lowByte(), de.lowByte());
                return;
            case 0x4C:
                load(bc.lowByte(), hl.highByte());
                return;
            case 0x4D:
                load(bc.lowByte(), hl.lowByte());
                return;
            case 0x4E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                load(bc.lowByte(), data);
                return;
            }
            case 0x4F:
                load(bc.lowByte(), af.highByte());
                return;
            case 0x50:
                load(de.highByte(), bc.highByte());
                return;
            case 0x51:
                load(de.highByte(), bc.lowByte());
                return;
            case 0x52:
                load(de.highByte(), de.highByte());
                return;
            case 0x53:
                load(de.highByte(), de.lowByte());
                return;
            case 0x54:
                load(de.highByte(), hl.highByte());
                return;
            case 0x55:
                load(de.highByte(), hl.lowByte());
                return;
            case 0x56:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                load(de.highByte(), data);
                return;
            }
            case 0x57:
                load(de.highByte(), af.highByte());
                return;
            case 0x58:
                load(de.lowByte(), bc.highByte());
                return;
            case 0x59:
                load(de.lowByte(), bc.lowByte());
                return;
            case 0x5A:
                load(de.lowByte(), de.highByte());
                return;
            case 0x5B:
                load(de.lowByte(), de.lowByte());
                return;
            case 0x5C:
                load(de.lowByte(), hl.highByte());
                return;
            case 0x5D:
                load(de.lowByte(), hl.lowByte());
                return;
            case 0x5E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                load(de.lowByte(), data);
                return;
            }
            case 0x5F:
                load(de.lowByte(), af.highByte());
                return;
            case 0x60:
                load(hl.highByte(), bc.highByte());
                return;
            case 0x61:
                load(hl.highByte(), bc.lowByte());
                return;
            case 0x62:
                load(hl.highByte(), de.highByte());
                return;
            case 0x63:
                load(hl.highByte(), de.lowByte());
                return;
            case 0x64:
                load(hl.highByte(), hl.highByte());
                return;
            case 0x65:
                load(hl.highByte(), hl.lowByte());
                return;
            case 0x66:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                load(hl.highByte(), data);
                return;
            }
            case 0x67:
                load(hl.highByte(), af.highByte());
                return;
            case 0x68:
                load(hl.lowByte(), bc.highByte());
                return;
            case 0x69:
                load(hl.lowByte(), bc.lowByte());
                return;
            case 0x6A:
                load(hl.lowByte(), de.highByte());
                return;
            case 0x6B:
                load(hl.lowByte(), de.lowByte());
                return;
            case 0x6C:
                load(hl.lowByte(), hl.highByte());
                return;
            case 0x6D:
                load(hl.lowByte(), hl.lowByte());
                return;
            case 0x6E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                load(hl.lowByte(), data);
                return;
            }
            case 0x6F:
                load(hl.lowByte(), af.highByte());
                return;
            case 0x70:
            {
                uint16_t address = hl.read();
                load(address, bc.highByte());
                return;
            }
            case 0x71:
            {
                uint16_t address = hl.read();
                load(address, bc.lowByte());
                return;
            }
            case 0x72:
            {
                uint16_t address = hl.read();
                load(address, de.highByte());
                return;
            }
            case 0x73:
            {
                uint16_t address = hl.read();
                load(address, de.lowByte());
                return;
            }
            case 0x74:
            {
                uint16_t address = hl.read();
                load(address, hl.highByte());
                return;
            }
            case 0x75:
            {
                uint16_t address = hl.read();
                load(address, hl.lowByte());
                return;
            }
            case 0x76:
                halt();
                return;
            case 0x77:
            {
                uint16_t address = hl.read();
                load(address, af.highByte());
                return;
            }
            case 0x78:
                load(af.highByte(), bc.highByte());
                return;
            case 0x79:
                load(af.highByte(), bc.lowByte());
                return;
            case 0x7A:
                load(af.highByte(), de.highByte());
                return;
            case 0x7B:
                load(af.highByte(), de.lowByte());
                return;
            case 0x7C: // LD A, H - 1/4t
                load(af.highByte(), hl.highByte());
                return;
            case 0x7D: // LD A, L - 1/4t
                load(af.highByte(), hl.lowByte());
                return;
            case 0x7E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                load(af.highByte(), data);
                return;
            }
            case 0x7F:
                load(af.highByte(), af.highByte());
                return;
            case 0x80:
                alu->add(bc.highByte());
                return;
            case 0x81:
                alu->add(bc.lowByte());
                return;
            case 0x82:
                alu->add(de.highByte());
                return;
            case 0x83:
                alu->add(de.lowByte());
                return;
            case 0x84:
                alu->add(hl.highByte());
                return;
            case 0x85:
                alu->add(hl.lowByte());
                return;
            case 0x86:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->add(data);
                return;
            }
            case 0x87:
                alu->add(af.highByte());
                return;
            case 0x88:
                alu->adc(bc.highByte());
                return;
            case 0x89:
                alu->adc(bc.lowByte());
                return;
            case 0x8A:
                alu->adc(de.highByte());
                return;
            case 0x8B:
                alu->adc(de.lowByte());
                return;
            case 0x8C:
                alu->adc(hl.highByte());
                return;
            case 0x8D:
                alu->adc(hl.lowByte());
                return;
            case 0x8E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->adc(data);
                return;
            }
            case 0x8F:
                alu->adc(af.highByte());
                return;
            case 0x90:
                alu->subtract(bc.highByte());
                return;
            case 0x91:
                alu->subtract(bc.lowByte());
                return;
            case 0x92:
                alu->subtract(de.highByte());
                return;
            case 0x93:
                alu->subtract(de.lowByte());
                return;
            case 0x94:
                alu->subtract(hl.highByte());
                return;
            case 0x95:
                alu->subtract(hl.lowByte());
                return;
            case 0x96:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->subtract(data);
                return;
            }
            case 0x97:
                alu->subtract(af.highByte());
                return;
            case 0x98:
                alu->subtractCarry(bc.highByte());
                return;
            case 0x99:
                alu->subtractCarry(bc.lowByte());
                return;
            case 0x9A:
                alu->subtractCarry(de.highByte());
                return;
            case 0x9B:
                alu->subtractCarry(de.lowByte());
                return;
            case 0x9C:
                alu->subtractCarry(hl.highByte());
                return;
            case 0x9D:
                alu->subtractCarry(hl.lowByte());
                return;
            case 0x9E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->subtractCarry(data);
                return;
            }
            case 0x9F:
                alu->subtractCarry(af.highByte());
                return;
            case 0xA0:
                alu->and_(bc.highByte());
                return;
            case 0xA1:
                alu->and_(bc.lowByte());
                return;
            case 0xA2:
                alu->and_(de.highByte());
                return;
            case 0xA3:
                alu->and_(de.lowByte());
                return;
            case 0xA4:
                alu->and_(hl.highByte());
                return;
            case 0xA5:
                alu->and_(hl.lowByte());
                return;
            case 0xA6:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->and_(data);
                return;
            }
            case 0xA7:
                alu->and_(af.highByte());
                return;
            case 0xA8:
                alu->xor_(bc.highByte());
                return;
            case 0xA9:
                alu->xor_(bc.lowByte());
                return;
            case 0xAA:
                alu->xor_(de.highByte());
                return;
            case 0xAB:
                alu->xor_(de.lowByte());
                return;
            case 0xAC:
                alu->xor_(hl.highByte());
                return;
            case 0xAD:
                alu->xor_(hl.lowByte());
                return;
            case 0xAE:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->xor_(data);
                return;
            }
            case 0xAF:
                alu->xor_(af.highByte());
                return;
            case 0xB0:
                alu->or_(bc.highByte());
                return;
            case 0xB1:
                alu->or_(bc.lowByte());
                return;
            case 0xB2:
                alu->or_(de.highByte());
                return;
            case 0xB3:
                alu->or_(de.lowByte());
                return;
            case 0xB4:
                alu->or_(hl.highByte());
                return;
            case 0xB5:
                alu->or_(hl.lowByte());
                return;
            case 0xB6:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->or_(data);
                return;
            }
            case 0xB7:
                alu->or_(af.highByte());
                return;
            case 0xB8:
                alu->compare(bc.highByte());
                return;
            case 0xB9:
                alu->compare(bc.lowByte());
                return;
            case 0xBA:
                alu->compare(de.highByte());
                return;
            case 0xBB:
                alu->compare(de.lowByte());
                return;
            case 0xBC:
                alu->compare(hl.highByte());
                return;
            case 0xBD:
                alu->compare(hl.lowByte());
                return;
            case 0xBE:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->compare(data);
                return;
            }
            case 0xBF:
                alu->compare(af.highByte());
                return;
            case 0xC0:
                if (!checkFlag(CPUFlags::z))
                {
                    ret();
                    emulateCycles(3);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            case 0xC1:
                //printf("\nBefore : %02X\n", BC.read());
                bc = mmuPtr->pop(sp);
                //printf("After : %02X\n", BC.read());
                return;
            case 0xC2:
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                if (!checkFlag(CPUFlags::z))
                {
                    jump(a16);
                    emulateCycles(1);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0xC3: // JP a16 - 3/16t
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                jump(a16);
                return;
            }
            case 0xC4:
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                if (!checkFlag(CPUFlags::z))
                {
                    call(a16);
                    emulateCycles(3);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0xC5:
                //printf("\nBC : %02x\n", BC.read());
                mmuPtr->push(sp, bc);
                return;
            case 0xC6:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
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
                    emulateCycles(3);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            case 0xC9: // RET - 1/16t
                ret();
                return;
            case 0xCA:
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                if (checkFlag(CPUFlags::z))
                {
                    jump(a16);
                    emulateCycles(1);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0xCC:
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                if (checkFlag(CPUFlags::z))
                {
                    call(a16);
                    emulateCycles(3);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0xCD: // CALL a16 - 3/24t
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                call(a16);
                return;
            }
            case 0xCE:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
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
                    emulateCycles(3);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            case 0xD1:
                //printf("\nBefore : %02X\n", DE.read());
                de = mmuPtr->pop(sp);
                //printf("After : %02X\n", DE.read());
                return;
            case 0xD2:
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                if (!checkFlag(CPUFlags::c))
                {
                    jump(a16);
                    emulateCycles(1);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0xD4:
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                if (!checkFlag(CPUFlags::c))
                {
                    call(a16);
                    emulateCycles(3);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0xD5:
                //printf("\nDE : %02x\n", DE.read());
                mmuPtr->push(sp, de);
                return;
            case 0xD6:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
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
                    emulateCycles(3);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            case 0xD9:
                reti();
                return;
            case 0xDA:
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                if (checkFlag(CPUFlags::c))
                {
                    jump(a16);
                    emulateCycles(1);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0xDC:
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                if (checkFlag(CPUFlags::c))
                {
                    call(a16);
                    emulateCycles(3);
                }
/*                else
                {
                    printf("\nCONDITION IS FALSE!!!!!!\n");
                }*/
                return;
            }
            case 0xDE:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                alu->subtractCarry(n8);
                return;
            }
            case 0xDF:
                rst(0x18);
                return;
            case 0xE0: // LD ($FF00+a8),A
            {
                uint8_t a8 = mmuPtr->read8(pc++);
                uint16_t address = 0xFF00 | a8;
                //printf("\nFF00 | DATA : %02X -- A: %02x\n", address, AF.highByte().read());
                load(address, af.highByte());
                return;
            }
            case 0xE1: // POP HL
                //printf("\nBefore : %02X\n", HL.read());
                hl = mmuPtr->pop(sp);
                //printf("After : %02X\n", HL.read());
                return;
            case 0xE2:
            {
                uint8_t C = bc.lowByte().read();
                uint16_t address = 0xFF00 | C;
                load(address, af.highByte());
                return;
            }
            case 0xE5: // PUSH HL
                //printf("\nHL : %02x\n", HL.read());
                mmuPtr->push(sp, hl);
                return;
            case 0xE6:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                alu->and_(n8);
                return;
            }
            case 0xE7:
                rst(0x20);
                return;
            case 0xE8:
            {
                uint16_t data =  mmuPtr->read8(pc++);
                alu->addToStack(data);
                return;
            }
            case 0xE9:
                jump(hl);
                return;
            case 0xEA: // LD [a16], A - 3/16t
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                load(a16, af.highByte());
                return;
            }
            case 0xEE:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                alu->xor_(n8);
                return;
            }
            case 0xEF:
                rst(0x28);
                return;
            case 0xF0: // LD A, ($FF00+a8)
            {
                uint8_t a8 = mmuPtr->read8(pc++);
                uint16_t address = 0xFF00 | a8;
                uint8_t data = mmuPtr->read8(address);
                load(af.highByte(), data);
                return;
            }
            case 0xF1:
                //printf("\nBefore : %02X\n", AF.read());
                af = mmuPtr->pop(sp) & 0xFFF0;
                //printf("After : %02X\n", AF.read());
                return;
            case 0xF2:
            {
                uint8_t C = bc.lowByte().read();
                uint16_t address = 0xFF00 | C;
                uint8_t data = mmuPtr->read8(address);
                load(af.highByte(), data);
                return;
            }
            case 0xF3: // DI - 1/4t
                di();
                return;
            case 0xF5: // PUSH AF
                //printf("\nAF : %02x\n", AF.read());
                mmuPtr->push(sp, af);
                return;
            case 0xF6:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                alu->or_(n8);
                return;
            }
            case 0xF7:
                rst(0x30);
                return;
            case 0xF8:
            {
                uint16_t e8 = mmuPtr->read8(pc++);
                uint16_t sp_e8 = sp.read() + static_cast<int8_t>(e8);

                //printf("\nSP+e8 : %04x\n", (SP.read() & 0x0F) + (e8 & 0x0F));
                (sp.read() & 0x0F) + (e8 & 0x0F) > 0x0F ? setFlag(CPUFlags::h) : resetFlag(CPUFlags::h);
                //printf("\nSP+e8 : %04x\n", (SP.read() & 0xFF) + (e8 & 0xFF));
                (sp.read() & 0xFF) + (e8 & 0xFF) > 0xFF ? setFlag(CPUFlags::c) : resetFlag(CPUFlags::c);

                load(hl, sp_e8);

                resetFlag(CPUFlags::z);
                resetFlag(CPUFlags::n);
                return;
            }
            case 0xF9:
                load(sp, hl.read());
                return;
            case 0xFA:
            {
                uint16_t address = pc.read();
                uint16_t a16 = mmuPtr->read16(address);
                pc += 2;
                uint8_t data = mmuPtr->read8(a16);
                load(af.highByte(), data);
                return;
            }
            case 0xFB:
                ei();
                return;
            case 0xFE:
            {
                uint8_t n8 = mmuPtr->read8(pc++);
                alu->compare(n8);
                return;
            }
            case 0xFF:
                rst(0x38);
                return;
            default:
                printf("UNSUPPORTED INSTRUCTION : %#02x\n", currentOpcode);
                exit(-1);
        }
    }

    void CPU::decodeExtendedInstructions()
    {
        currentOpcode = mmuPtr->read8(pc++);
        switch (currentOpcode)
        {
            case 0x00:
                alu->rlc(bc.highByte());
                break;
            case 0x01:
                alu->rlc(bc.lowByte());
                break;
            case 0x02:
                alu->rlc(de.highByte());
                break;
            case 0x03:
                alu->rlc(de.lowByte());
                break;
            case 0x04:
                alu->rlc(hl.highByte());
                break;
            case 0x05:
                alu->rlc(hl.lowByte());
                break;
            case 0x06:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->rlc(data);
                mmuPtr->write8(address, data);
                break;
            }
            case 0x07:
                alu->rlc(af.highByte());
                break;
            case 0x08:
                alu->rrc(bc.highByte());
                return;
            case 0x09:
                alu->rrc(bc.lowByte());
                break;
            case 0x0A:
                alu->rrc(de.highByte());
                return;
            case 0x0B:
                alu->rrc(de.lowByte());
                return;
            case 0x0C:
                alu->rrc(hl.highByte());
                return;
            case 0x0D:
                alu->rrc(hl.lowByte());
                return;
            case 0x0E: {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->rrc(data);
                mmuPtr->write8(address, data);
                break;
            }
            case 0x0F:
                alu->rrc(af.highByte());
                return;

            case 0x10:
                alu->rl(bc.highByte());
                break;
            case 0x11:
                alu->rl(bc.lowByte());
                break;
            case 0x12:
                alu->rl(de.highByte());
                break;
            case 0x13:
                alu->rl(de.lowByte());
                break;
            case 0x14:
                alu->rl(hl.highByte());
                break;
            case 0x15:
                alu->rl(hl.lowByte());
                break;
            case 0x16:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->rl(data);
                mmuPtr->write8(address, data);
                break;
            }
            case 0x17:
                alu->rl(af.highByte());
                break;
            case 0x18:
                alu->rr(bc.highByte());
                return;
            case 0x19:
                alu->rr(bc.lowByte());
                break;
            case 0x1A:
                alu->rr(de.highByte());
                return;
            case 0x1B:
                alu->rr(de.lowByte());
                return;
            case 0x1C:
                alu->rr(hl.highByte());
                return;
            case 0x1D:
                alu->rr(hl.lowByte());
                return;
            case 0x1E: {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->rr(data);
                mmuPtr->write8(address, data);
                break;
            }
            case 0x1F:
                alu->rr(af.highByte());
                return;
            case 0x20:
                alu->sla(bc.highByte());
                return;
            case 0x21:
                alu->sla(bc.lowByte());
                return;
            case 0x22:
                alu->sla(de.highByte());
                return;
            case 0x23:
                alu->sla(de.lowByte());
                return;
            case 0x24:
                alu->sla(hl.highByte());
                return;
            case 0x25:
                alu->sla(hl.lowByte());
                return;
            case 0x26:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->sla(data);
                mmuPtr->write8(address, data);
            }
            case 0x27:
                alu->sla(af.highByte());
                return;
            case 0x28:
                alu->sra(bc.highByte());
                return;
            case 0x29:
                alu->sra(bc.lowByte());
                return;
            case 0x2A:
                alu->sra(de.highByte());
                return;
            case 0x2B:
                alu->sra(de.lowByte());
                return;
            case 0x2C:
                alu->sra(hl.highByte());
                return;
            case 0x2D:
                alu->sra(hl.lowByte());
                return;
            case 0x2E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->sra(data);
                mmuPtr->write8(address, data);
            }
            case 0x2F:
                alu->sra(af.highByte());
                return;
            case 0x30:
                alu->swap(bc.highByte());
                return;
            case 0x31:
                alu->swap(bc.lowByte());
                return;
            case 0x32:
                alu->swap(de.highByte());
                return;
            case 0x33:
                alu->swap(de.lowByte());
                return;
            case 0x34:
                alu->swap(hl.highByte());
                return;
            case 0x35:
                alu->swap(hl.lowByte());
                return;
            case 0x36:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->swap(data);
                mmuPtr->write8(address, data);
            }
            case 0x37:
                alu->swap(af.highByte());
                return;
            case 0x38:
                alu->srl(bc.highByte());
                return;
            case 0x39:
                alu->srl(bc.lowByte());
                return;
            case 0x3A:
                alu->srl(de.highByte());
                return;
            case 0x3B:
                alu->srl(de.lowByte());
                return;
            case 0x3C:
                alu->srl(hl.highByte());
                return;
            case 0x3D:
                alu->srl(hl.lowByte());
                return;
            case 0x3E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->srl(data);
                mmuPtr->write8(address, data);
            }
            case 0x3F:
                alu->srl(af.highByte());
                return;
            case 0x40:
                alu->bit(bc.highByte(), 0);
                return;
            case 0x41:
                alu->bit(bc.lowByte(), 0);
                return;
            case 0x42:
                alu->bit(de.highByte(), 0);
                return;
            case 0x43:
                alu->bit(de.lowByte(), 0);
                return;
            case 0x44:
                alu->bit(hl.highByte(), 0);
                return;
            case 0x45:
                alu->bit(hl.lowByte(), 0);
                return;
            case 0x46:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 0);
            }
            case 0x47:
                alu->bit(af.highByte(), 0);
                return;
            case 0x48:
                alu->bit(bc.highByte(), 1);
                return;
            case 0x49:
                alu->bit(bc.lowByte(), 1);
                return;
            case 0x4A:
                alu->bit(de.highByte(), 1);
                return;
            case 0x4B:
                alu->bit(de.lowByte(), 1);
                return;
            case 0x4C:
                alu->bit(hl.highByte(), 1);
                return;
            case 0x4D:
                alu->bit(hl.lowByte(), 1);
                return;
            case 0x4E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 1);
            }
            case 0x4F:
                alu->bit(af.highByte(), 1);
                return;

            case 0x50:
                alu->bit(bc.highByte(), 2);
                return;
            case 0x51:
                alu->bit(bc.lowByte(), 2);
                return;
            case 0x52:
                alu->bit(de.highByte(), 2);
                return;
            case 0x53:
                alu->bit(de.lowByte(), 2);
                return;
            case 0x54:
                alu->bit(hl.highByte(), 2);
                return;
            case 0x55:
                alu->bit(hl.lowByte(), 2);
                return;
            case 0x56:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 2);
            }
            case 0x57:
                alu->bit(af.highByte(), 2);
                return;
            case 0x58:
                alu->bit(bc.highByte(), 3);
                return;
            case 0x59:
                alu->bit(bc.lowByte(), 3);
                return;
            case 0x5A:
                alu->bit(de.highByte(), 3);
                return;
            case 0x5B:
                alu->bit(de.lowByte(), 3);
                return;
            case 0x5C:
                alu->bit(hl.highByte(), 3);
                return;
            case 0x5D:
                alu->bit(hl.lowByte(), 3);
                return;
            case 0x5E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 3);
            }
            case 0x5F:
                alu->bit(af.highByte(), 3);
                return;


            case 0x60:
                alu->bit(bc.highByte(), 4);
                return;
            case 0x61:
                alu->bit(bc.lowByte(), 4);
                return;
            case 0x62:
                alu->bit(de.highByte(), 4);
                return;
            case 0x63:
                alu->bit(de.lowByte(), 4);
                return;
            case 0x64:
                alu->bit(hl.highByte(), 4);
                return;
            case 0x65:
                alu->bit(hl.lowByte(), 4);
                return;
            case 0x66:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 4);
            }
            case 0x67:
                alu->bit(af.highByte(), 4);
                return;
            case 0x68:
                alu->bit(bc.highByte(), 5);
                return;
            case 0x69:
                alu->bit(bc.lowByte(), 5);
                return;
            case 0x6A:
                alu->bit(de.highByte(), 5);
                return;
            case 0x6B:
                alu->bit(de.lowByte(), 5);
                return;
            case 0x6C:
                alu->bit(hl.highByte(), 5);
                return;
            case 0x6D:
                alu->bit(hl.lowByte(), 5);
                return;
            case 0x6E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 5);
            }
            case 0x6F:
                alu->bit(af.highByte(), 5);
                return;

            case 0x70:
                alu->bit(bc.highByte(), 6);
                return;
            case 0x71:
                alu->bit(bc.lowByte(), 6);
                return;
            case 0x72:
                alu->bit(de.highByte(), 6);
                return;
            case 0x73:
                alu->bit(de.lowByte(), 6);
                return;
            case 0x74:
                alu->bit(hl.highByte(), 6);
                return;
            case 0x75:
                alu->bit(hl.lowByte(), 6);
                return;
            case 0x76:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 6);
            }
            case 0x77:
                alu->bit(af.highByte(), 6);
                return;
            case 0x78:
                alu->bit(bc.highByte(), 7);
                return;
            case 0x79:
                alu->bit(bc.lowByte(), 7);
                return;
            case 0x7A:
                alu->bit(de.highByte(), 7);
                return;
            case 0x7B:
                alu->bit(de.lowByte(), 7);
                return;
            case 0x7C:
                alu->bit(hl.highByte(), 7);
                return;
            case 0x7D:
                alu->bit(hl.lowByte(), 7);
                return;
            case 0x7E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->bit(data, 7);
            }
            case 0x7F:
                alu->bit(af.highByte(), 7);
                return;


            case 0x80:
                alu->res(bc.highByte(), 0);
                return;
            case 0x81:
                alu->res(bc.lowByte(), 0);
                return;
            case 0x82:
                alu->res(de.highByte(), 0);
                return;
            case 0x83:
                alu->res(de.lowByte(), 0);
                return;
            case 0x84:
                alu->res(hl.highByte(), 0);
                return;
            case 0x85:
                alu->res(hl.lowByte(), 0);
                return;
            case 0x86:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 0);
                mmuPtr->write8(address, data);
            }
            case 0x87:
                alu->res(af.highByte(), 0);
                return;
            case 0x88:
                alu->res(bc.highByte(), 1);
                return;
            case 0x89:
                alu->res(bc.lowByte(), 1);
                return;
            case 0x8A:
                alu->res(de.highByte(), 1);
                return;
            case 0x8B:
                alu->res(de.lowByte(), 1);
                return;
            case 0x8C:
                alu->res(hl.highByte(), 1);
                return;
            case 0x8D:
                alu->res(hl.lowByte(), 1);
                return;
            case 0x8E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 1);
                mmuPtr->write8(address, data);
            }
            case 0x8F:
                alu->res(af.highByte(), 1);
                return;


            case 0x90:
                alu->res(bc.highByte(), 2);
                return;
            case 0x91:
                alu->res(bc.lowByte(), 2);
                return;
            case 0x92:
                alu->res(de.highByte(), 2);
                return;
            case 0x93:
                alu->res(de.lowByte(), 2);
                return;
            case 0x94:
                alu->res(hl.highByte(), 2);
                return;
            case 0x95:
                alu->res(hl.lowByte(), 2);
                return;
            case 0x96:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 2);
                mmuPtr->write8(address, data);
            }
            case 0x97:
                alu->res(af.highByte(), 2);
                return;
            case 0x98:
                alu->res(bc.highByte(), 3);
                return;
            case 0x99:
                alu->res(bc.lowByte(), 3);
                return;
            case 0x9A:
                alu->res(de.highByte(), 3);
                return;
            case 0x9B:
                alu->res(de.lowByte(), 3);
                return;
            case 0x9C:
                alu->res(hl.highByte(), 3);
                return;
            case 0x9D:
                alu->res(hl.lowByte(), 3);
                return;
            case 0x9E:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 3);
                mmuPtr->write8(address, data);
            }
            case 0x9F:
                alu->res(af.highByte(), 3);
                return;


            case 0xA0:
                alu->res(bc.highByte(), 4);
                return;
            case 0xA1:
                alu->res(bc.lowByte(), 4);
                return;
            case 0xA2:
                alu->res(de.highByte(), 4);
                return;
            case 0xA3:
                alu->res(de.lowByte(), 4);
                return;
            case 0xA4:
                alu->res(hl.highByte(), 4);
                return;
            case 0xA5:
                alu->res(hl.lowByte(), 4);
                return;
            case 0xA6:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 4);
                mmuPtr->write8(address, data);
            }
            case 0xA7:
                alu->res(af.highByte(), 4);
                return;
            case 0xA8:
                alu->res(bc.highByte(), 5);
                return;
            case 0xA9:
                alu->res(bc.lowByte(), 5);
                return;
            case 0xAA:
                alu->res(de.highByte(), 5);
                return;
            case 0xAB:
                alu->res(de.lowByte(), 5);
                return;
            case 0xAC:
                alu->res(hl.highByte(), 5);
                return;
            case 0xAD:
                alu->res(hl.lowByte(), 5);
                return;
            case 0xAE:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 5);
                mmuPtr->write8(address, data);
            }
            case 0xAF:
                alu->res(af.highByte(), 5);
                return;
            case 0xB0:
                alu->res(bc.highByte(), 6);
                return;
            case 0xB1:
                alu->res(bc.lowByte(), 6);
                return;
            case 0xB2:
                alu->res(de.highByte(), 6);
                return;
            case 0xB3:
                alu->res(de.lowByte(), 6);
                return;
            case 0xB4:
                alu->res(hl.highByte(), 6);
                return;
            case 0xB5:
                alu->res(hl.lowByte(), 6);
                return;
            case 0xB6:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 6);
                mmuPtr->write8(address, data);
            }
            case 0xB7:
                alu->res(af.highByte(), 6);
                return;
            case 0xB8:
                alu->res(bc.highByte(), 7);
                return;
            case 0xB9:
                alu->res(bc.lowByte(), 7);
                return;
            case 0xBA:
                alu->res(de.highByte(), 7);
                return;
            case 0xBB:
                alu->res(de.lowByte(), 7);
                return;
            case 0xBC:
                alu->res(hl.highByte(), 7);
                return;
            case 0xBD:
                alu->res(hl.lowByte(), 7);
                return;
            case 0xBE:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->res(data, 7);
                mmuPtr->write8(address, data);
            }
            case 0xBF:
                alu->res(af.highByte(), 7);
                return;

            case 0xC0:
                alu->set(bc.highByte(), 0);
                return;
            case 0xC1:
                alu->set(bc.lowByte(), 0);
                return;
            case 0xC2:
                alu->set(de.highByte(), 0);
                return;
            case 0xC3:
                alu->set(de.lowByte(), 0);
                return;
            case 0xC4:
                alu->set(hl.highByte(), 0);
                return;
            case 0xC5:
                alu->set(hl.lowByte(), 0);
                return;
            case 0xC6:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 0);
                mmuPtr->write8(address, data);
            }
            case 0xC7:
                alu->set(af.highByte(), 0);
                return;
            case 0xC8:
                alu->set(bc.highByte(), 1);
                return;
            case 0xC9:
                alu->set(bc.lowByte(), 1);
                return;
            case 0xCA:
                alu->set(de.highByte(), 1);
                return;
            case 0xCB:
                alu->set(de.lowByte(), 1);
                return;
            case 0xCC:
                alu->set(hl.highByte(), 1);
                return;
            case 0xCD:
                alu->set(hl.lowByte(), 1);
                return;
            case 0xCE:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 1);
                mmuPtr->write8(address, data);
            }
            case 0xCF:
                alu->set(af.highByte(), 1);
                return;


            case 0xD0:
                alu->set(bc.highByte(), 2);
                return;
            case 0xD1:
                alu->set(bc.lowByte(), 2);
                return;
            case 0xD2:
                alu->set(de.highByte(), 2);
                return;
            case 0xD3:
                alu->set(de.lowByte(), 2);
                return;
            case 0xD4:
                alu->set(hl.highByte(), 2);
                return;
            case 0xD5:
                alu->set(hl.lowByte(), 2);
                return;
            case 0xD6:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 2);
                mmuPtr->write8(address, data);
            }
            case 0xD7:
                alu->set(af.highByte(), 2);
                return;
            case 0xD8:
                alu->set(bc.highByte(), 3);
                return;
            case 0xD9:
                alu->set(bc.lowByte(), 3);
                return;
            case 0xDA:
                alu->set(de.highByte(), 3);
                return;
            case 0xDB:
                alu->set(de.lowByte(), 3);
                return;
            case 0xDC:
                alu->set(hl.highByte(), 3);
                return;
            case 0xDD:
                alu->set(hl.lowByte(), 3);
                return;
            case 0xDE:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 3);
                mmuPtr->write8(address, data);
            }
            case 0xDF:
                alu->set(af.highByte(), 3);
                return;


            case 0xE0:
                alu->set(bc.highByte(), 4);
                return;
            case 0xE1:
                alu->set(bc.lowByte(), 4);
                return;
            case 0xE2:
                alu->set(de.highByte(), 4);
                return;
            case 0xE3:
                alu->set(de.lowByte(), 4);
                return;
            case 0xE4:
                alu->set(hl.highByte(), 4);
                return;
            case 0xE5:
                alu->set(hl.lowByte(), 4);
                return;
            case 0xE6:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 4);
                mmuPtr->write8(address, data);
            }
            case 0xE7:
                alu->set(af.highByte(), 4);
                return;
            case 0xE8:
                alu->set(bc.highByte(), 5);
                return;
            case 0xE9:
                alu->set(bc.lowByte(), 5);
                return;
            case 0xEA:
                alu->set(de.highByte(), 5);
                return;
            case 0xEB:
                alu->set(de.lowByte(), 5);
                return;
            case 0xEC:
                alu->set(hl.highByte(), 5);
                return;
            case 0xED:
                alu->set(hl.lowByte(), 5);
                return;
            case 0xEE:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 5);
                mmuPtr->write8(address, data);
            }
            case 0xEF:
                alu->set(af.highByte(), 5);
                return;
            case 0xF0:
                alu->set(bc.highByte(), 6);
                return;
            case 0xF1:
                alu->set(bc.lowByte(), 6);
                return;
            case 0xF2:
                alu->set(de.highByte(), 6);
                return;
            case 0xF3:
                alu->set(de.lowByte(), 6);
                return;
            case 0xF4:
                alu->set(hl.highByte(), 6);
                return;
            case 0xF5:
                alu->set(hl.lowByte(), 6);
                return;
            case 0xF6:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 6);
                mmuPtr->write8(address, data);
            }
            case 0xF7:
                alu->set(af.highByte(), 6);
                return;
            case 0xF8:
                alu->set(bc.highByte(), 7);
                return;
            case 0xF9:
                alu->set(bc.lowByte(), 7);
                return;
            case 0xFA:
                alu->set(de.highByte(), 7);
                return;
            case 0xFB:
                alu->set(de.lowByte(), 7);
                return;
            case 0xFC:
                alu->set(hl.highByte(), 7);
                return;
            case 0xFD:
                alu->set(hl.lowByte(), 7);
                return;
            case 0xFE:
            {
                uint16_t address = hl.read();
                uint8_t data = mmuPtr->read8(address);
                alu->set(data, 7);
                mmuPtr->write8(address, data);
            }
            case 0xFF:
                alu->set(af.highByte(), 7);
                return;
            default:
                printf("%#02x -> INVALID INSTRUCTION!", currentOpcode);
                exit(-1);
        }
    }

    void CPU::run()
    {
        while (gameBoyPtr->currentState == EmulatorState::running)
        {
            step();
        }
    }
}