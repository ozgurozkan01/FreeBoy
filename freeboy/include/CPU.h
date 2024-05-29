//
// Created by ozgur on 12/04/2024.
//

#ifndef FREEBOY_CPU_H
#define FREEBOY_CPU_H

#include <cstdint>
#include "Instructions.h"

namespace gameboy
{
    class Bus;
    class GameBoy;

    using namespace instruction;

    namespace instruction
    {
        class InterruptHandler;
    }

    // The CPU flags are used to get information about the results of arithmetic and logical operations,
    // and to activate conditional branching
    enum FlagType
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
        uint8_t A = 0; // Accumulator Register -> Holds result of logical, arithmetic op and data
        uint8_t F = 0; // Flag-Status Register -> ZNHC0000
        // General Purpose Register
        uint8_t B = 0;
        uint8_t C = 0;
        uint8_t D = 0;
        uint8_t E = 0;
        uint8_t H = 0;
        uint8_t L = 0;
        uint16_t PC = 0; // Program Counter -> Holds address of the next instruction
        uint16_t SP = 0; // Stack Pointer -> Holds address of the last added element into the stack

    };

    class CPU {
        friend instruction::Instructions;
    public:

        CPU(Bus* _bus, GameBoy* _gb);

        void step();
        void emulateCycles(uint8_t cycleCount);

        bool init();
        
        bool getflag(FlagType _flag); // Get CPU Flags -> Z, N, C, H
        void setFlag(FlagType _flag, bool _state); // Set just one specific flag
        void setFlags(bool _zState, bool _nState, bool _hState, bool _cState); // Set whole flags at a time

        [[nodiscard]] inline uint16_t getFetchedData() const { return fetchedData; };

    private:
        uint8_t currentOpcode{}; // 8-bit Instruction
        uint16_t fetchedData{}; //
        uint16_t memoryDestination{}; // Memory address is used when isDestMem is true

        Instructions::Info* currentInstruction;
        CoreRegisters coreRegisters;

        Bus* busRef;
        GameBoy* gameBoyRef;
        Instructions* cpuProcess;
        InterruptHandler* interruptHandler;

        bool isStopped{};
        bool isHalted{};
        bool isDestMem{};

        void fetch();
        void decode();

        bool isConditionPassed();
        /* ALU Functions */
        void alu_adc(); void alu_add(); void alu_and(); void alu_cp();
        void alu_sub(); void alu_sbc(); void alu_or();  void alu_xor();

        /* STANDARD Functions */
        void call();    void cb();      void ccf();
        void cpl();     void daa();     void dec();
        void di();      void ei();      void halt();
        void inc();     void jp();      void jr();
        void ld();      void ldh();     void nop();
        void pop();     void push();    void ret();
        void reti();    void rlca();    void rla();
        void rra();     void rrca();    void rst();
        void scf();     void stop();

        /* CB-Prefixed Functions */
        void cb_bit();  void cb_res();  void cb_rl();   void cb_rlc();   void cb_rr();
        void cb_rrc();  void cb_set();  void cb_sla();  void cb_sra();   void cb_srl();
        void cb_swap();

        void addr_IMP();    // No Implementation
        void addr_R();      // Operation on just one Register (does not matter 8 or 16 bit)
        void addr_R_D8();   // Operation which is getting the 8 bit data and store in the register
        void addr_D16();    // Operation which is storing 16 bit value into PC
        void addr_R_D16();  // Operation which is getting the 16 bit data and store in the register
        void addr_R_R();    // Operation which is getting the data from register and store in the another register
        void addr_MR_R();   //
        void addr_R_MR();   //
        void addr_R_HLI();  // Operation which is getting the HL value and store in the register. Then increase HL
        void addr_R_HLD();  // Operation which is getting the HL value and store in the register. Then decrease HL
        void addr_HLI_R();  // Operation which is getting the data from register and store in the HL. then increase HL
        void addr_HLD_R();  // Operation which is getting the data from register and store in the HL. then decrease HL
        void addr_R_A8();   // Operation which is getting the data from 8 Bit address in memory and store on the register
        void addr_A8_R();   // Operation which is getting the data from register and store in the 8 bit address in memory
        void addr_HL_SPR(); //
        void addr_D8();     // Operation which is storing 8 bit value into PC
        void addr_D16_R();  // Operation which is getting the data from register and use as 16 bit data
        void addr_MR_D8();  //
        void addr_MR();     //
        void addr_A16_R();  // Operation which is getting the data from register and store 16 bit address in memory
        void addr_R_A16();  // Operation which is getting the data from 16 address in memory and store in the register


        uint16_t readRegister(RegisterType _register) const; // Read value of core register
        void writeRegister(RegisterType _register, uint16_t _value); // Read value into core register
    };
}

#endif //FREEBOY_CPU_H
