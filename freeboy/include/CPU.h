//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_CPU_H
#define FREEBOY_CPU_H

#include <cstdint>
#include "Instruction.h"

namespace gameboy
{
    class Bus;
    class GameBoy;

    // The CPU flags are used to get information about the results of arithmetic and logical operations,
    // and to activate conditional branching
    enum class FlagType : uint8_t
    {
        // LOWER 8 BITS OF AF
        NONE = 0x0,
        ZERO_FLAG = 1 << 7,
        SUBTRACT_FLAG = 1 << 6, // (BCD)
        HALF_CARRY_FLAG = 1 << 5, // (BCD)
        CARRY_FLAG = 1 << 4
    };

    struct CoreRegisters
    {
        uint8_t A; // Accumulator Register -> Holds result of logical, arithmetic op and data
        uint8_t F; // Flag-Status Register -> ZNHC0000
        // General Purpose Register
        uint8_t B;
        uint8_t C;
        uint8_t D;
        uint8_t E;
        uint8_t H;
        uint8_t L;
        uint16_t PC; // Program Counter -> Holds address of the next instruction
        uint16_t SP; // Stack Pointer -> Holds address of the last added element into the stack
    };

    class CPU {
    public:

        CPU(Bus* _bus, GameBoy* _gb);
        bool init();
        void step();

    private:
        uint8_t currentOpcode{}; // 8-bit Instruction
        uint16_t fetchedData{}; //
        uint16_t memoryDestination{}; //

        CoreRegisters coreRegisters{};
        instruction::Instruction currentInstraction;

        Bus* busRef;
        GameBoy* gameBoyRef;

        bool isStopped{};
        bool isHalted{};

        void fetch();
        void execute();

        // Switching from register enum to register variable
        uint16_t readRegister(instruction::RegisterType _register) const;
        // Set specific register current value
        void writeRegister(instruction::RegisterType _register, uint16_t _value);
    };
}

#endif //FREEBOY_CPU_H
