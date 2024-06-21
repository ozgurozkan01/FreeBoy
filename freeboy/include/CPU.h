//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_CPU_H
#define FREEBOY_CPU_H

#include <cstdint>
#include "InstructionSet.h"
#include "Registers.h"

namespace gameboy
{
    class GameBoy;
    class MMU;
    class ALU;
    class InterruptHandler;

    using namespace instruction;
    using namespace cpu_register;

    // The CPU flags are used to get information about the results of arithmetic and logical operations,
    // and to activate conditional branching
    enum class CPUFlags : uint8_t
    {
        z = 7, // Zero Flag
        n = 6, // Subtraction Flag
        h = 5, // Half-Carry Flag
        c = 4, // Carry Flag
    };

    class CPU {
        friend ALU;
    public:

        CPU(GameBoy* _gb, MMU* _mmu, InterruptHandler* _interruptHandler);
        ~CPU();
        void step();
        void emulateCycles(uint8_t cycleCount);

        bool init();

    private:
        uint8_t currentOpcode{};

        InstructionSet::Header* currentInstruction;
        InstructionSet* cpuProcess;

        GameBoy* gameBoyPtr;
        MMU* mmuPtr;
        InterruptHandler* interruptHandlerPtr;
        ALU* alu;

        /* Acuumulator and Flags */
        Register16 AF;
        /* General Purpose Registers */
        Register16 BC;
        Register16 DE;
        Register16 HL;
        /* Stack Pointer */
        // Register16 SP;
        /* Program Counter */
        Register16 PC;
        Register16 SP;

        bool isStopped{};
        bool isHalted{};

        void fetch();
        void execute();

        void decodeStandardInstructions();
        void decodeExtendedInstructions();

        void halt();

        void setFlag(const CPUFlags _flag);
        void resetFlag(const CPUFlags _flag);
        uint8_t readFlag(const CPUFlags _flag);
        bool checkFlag(const CPUFlags _flag);

        /* STANDARD INSTRUCTIONS */
        void load(Register16& _register, const uint16_t _value);
        void load(Register8& _register, const uint8_t _value);
        void load(Register8& _dstRegister, const Register8& _srcRegister);
        void load(const uint16_t _address, const Register8& _register);
        void load(const uint16_t _address, const uint16_t _value);

        void jump(const uint16_t _value);
        void jump(const Register16& _register);

        void relativeJump(const uint16_t _data);
        void call(const uint16_t _value);
        void di(); // Disable Interrupts
        void ret(); // Return from subroutine

        /* CB INSTRUCTIONS */
        void srl(Register8& _dstRegister); // Shift Right Logically
        void srl(uint8_t& _value);

        void rr(Register8& _dstRegister); // Rotate Register
        void rr(uint8_t& _value);

        void swap(Register8& _dstRegister); // Rotate Register
        void swap(uint8_t& _value);

        void rra(); // Rotate Register A
    };
}

#endif //FREEBOY_CPU_H
