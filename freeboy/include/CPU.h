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
        friend InterruptHandler;
    public:

        CPU(GameBoy* _gb, MMU* _mmu, InterruptHandler* _interruptHandler);
        ~CPU();
        void run();
        void emulateCycles(uint8_t cycleCount);
    private:
        uint8_t currentOpcode{};

        InstructionSet::Header* currentInstruction;
        InstructionSet* cpuProcess;

        GameBoy* gameBoyPtr;
        MMU* mmuPtr;
        InterruptHandler* interruptHandlerPtr;
        ALU* alu;

        /* Acuumulator and Flags */
        Register16 af;
        /* General Purpose Registers */
        Register16 bc;
        Register16 de;
        Register16 hl;

        Register16 pc; /* Program Counter */
        Register16 sp; /* Stack Pointer */

        bool isStopped;
        bool isHalted;

        char dbg_msg[1024] = {0};
        int msg_size = 0;

        void debugUpdate();
        void debugPrint();

        void fetch();
        void execute();
        void step();

        void decodeStandardInstructions();
        void decodeExtendedInstructions();

        void setFlag(const CPUFlags _flag);
        void resetFlag(const CPUFlags _flag);
        void toggleFlag(const CPUFlags _flag);
        uint8_t readFlag(const CPUFlags _flag);
        bool checkFlag(const CPUFlags _flag);

        void load(Register16& _register, const uint16_t _value);
        void load(Register8& _register, const uint8_t _value);
        void load(Register8& _dstRegister, const Register8& _srcRegister);
        void load(const uint16_t _address, const Register8& _register);
        void load(const uint16_t _address, const uint16_t _value);

        void jump(const uint16_t _value);
        void jump(const Register16& _register);

        void relativeJump(const uint16_t _data);
        void call(const uint16_t _value);
        void rst(const uint8_t vector);
        void di();
        void ei();
        void ret();
        void reti();
        void halt();
    };
}

#endif //FREEBOY_CPU_H
