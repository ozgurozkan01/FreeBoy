//
// Created by ozgur on 12/04/2024.
//

#include "../include/CPU.h"
#include "../include/Bus.h"

namespace gameboy
{
    CPU::CPU(Bus* _bus) : busRef(_bus) {}

    bool CPU::init()
    {
        return true;
    }

    void CPU::step()
    {
    }

    void CPU::nop() {/* Do Nothing */}

    [[maybe_unused]] void CPU::ld(registers::Register8 &_register, uint8_t _data)
    {
    }

    [[maybe_unused]] void CPU::ld(registers::Register8& r_target, const registers::Register8& r_source)
    {
    }

    [[maybe_unused]] void CPU::ld(registers::Register16 &_register, uint16_t _data)
    {
    }

    [[maybe_unused]] void ld(registers::Register16& r_target, const registers::Register16& r_source)
    {
    }
}
