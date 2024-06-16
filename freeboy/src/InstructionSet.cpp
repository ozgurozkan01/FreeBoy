//
// Created by ozgur on 12/04/2024.
//

#include "../include/InstructionSet.h"

namespace gameboy
{
    namespace instruction
    {
        InstructionSet::InstructionSet()
        {
            standardInstructions =
                    {
                            Header{"NOP", 1, 1},
                            Header{"LD BC,n16", 3, 3},
                            Header{"LD (BC),A", 1, 2},
                            Header{"INC BC", 1, 2},
                            Header{"INC B", 1, 1},
                            Header{"DEC B", 1, 1},
                            Header{"LD B,n8", 2, 2},
                            Header{"RLCA", 1, 1},
                            Header{"LD (a16),SP", 3, 5},
                            Header{"ADD HL,BC", 1, 2},
                            Header{"LD A,(BC)", 1, 2},
                            Header{"DEC BC", 1, 2},
                            Header{"INC C", 1, 1},
                            Header{"DEC C", 1, 1},
                            Header{"LD C,n8", 2, 2},
                            Header{"RRCA", 1, 1},

                            Header{"STOP 0", 2, 1},
                            Header{"LD DE,n16", 3, 3},
                            Header{"LD (DE),A", 1, 2},
                            Header{"INC DE", 1, 2},
                            Header{"INC D", 1, 1},
                            Header{"DEC D", 1, 1},
                            Header{"LD D,n8", 2, 2},
                            Header{"RLA", 1, 1},
                            Header{"JR e8", 2, 3},
                            Header{"ADD HL,DE", 1, 2},
                            Header{"LD A,(DE)", 1, 2},
                            Header{"DEC DE", 1, 2},
                            Header{"INC E", 1, 1},
                            Header{"DEC E", 1, 1},
                            Header{"LD E,n8", 2, 2},
                            Header{"RRA", 1, 1},

                            Header{"JR NZ,e8", 2, 3},
                            Header{"LD HL,n16", 3, 3},
                            Header{"LD (HL+),A", 1, 2},
                            Header{"INC HL", 1, 2},
                            Header{"INC H", 1, 1},
                            Header{"DEC H", 1, 1},
                            Header{"LD H,n8", 2, 2},
                            Header{"DAA", 1, 1},
                            Header{"JR Z,e8", 2, 3},
                            Header{"ADD HL,HL", 1, 2},
                            Header{"LD A,(HL+)", 1, 2},
                            Header{"DEC HL", 1, 2},
                            Header{"INC L", 1, 1},
                            Header{"DEC L", 1, 1},
                            Header{"LD L,n8", 2, 2},
                            Header{"CPL", 1, 1},

                            Header{"JR NC,e8", 2, 3},
                            Header{"LD SP,n16", 3, 3},
                            Header{"LD (HL-),A", 1, 2},
                            Header{"INC SP", 1, 2},
                            Header{"INC (HL)", 1, 3},
                            Header{"DEC (HL)", 1, 3},
                            Header{"LD (HL),n8", 2, 3},
                            Header{"SCF", 1, 1},
                            Header{"JR C,e8", 2, 3},
                            Header{"ADD HL,SP", 1, 2},
                            Header{"LD A,(HL-)", 1, 2},
                            Header{"DEC SP", 1, 2},
                            Header{"INC A", 1, 1},
                            Header{"DEC A", 1, 1},
                            Header{"LD A,n8", 2, 2},
                            Header{"CCF", 1, 1},

                            Header{"LD B,B", 1, 1},
                            Header{"LD B,C", 1, 1},
                            Header{"LD B,D", 1, 1},
                            Header{"LD B,E", 1, 1},
                            Header{"LD B,H", 1, 1},
                            Header{"LD B,L", 1, 1},
                            Header{"LD B,(HL)", 1, 2},
                            Header{"LD B,A", 1, 1},
                            Header{"LD C,B", 1, 1},
                            Header{"LD C,C", 1, 1},
                            Header{"LD C,D", 1, 1},
                            Header{"LD C,E", 1, 1},
                            Header{"LD C,H", 1, 1},
                            Header{"LD C,L", 1, 1},
                            Header{"LD C,(HL)", 1, 2},
                            Header{"LD C,A", 1, 1},

                            Header{"LD D,B", 1, 1},
                            Header{"LD D,C", 1, 1},
                            Header{"LD D,D", 1, 1},
                            Header{"LD D,E", 1, 1},
                            Header{"LD D,H", 1, 1},
                            Header{"LD D,L", 1, 1},
                            Header{"LD D,(HL)", 1, 2},
                            Header{"LD D,A", 1, 1},
                            Header{"LD E,B", 1, 1},
                            Header{"LD E,C", 1, 1},
                            Header{"LD E,D", 1, 1},
                            Header{"LD E,E", 1, 1},
                            Header{"LD E,H", 1, 1},
                            Header{"LD E,L", 1, 1},
                            Header{"LD E,(HL)", 1, 2},
                            Header{"LD E,A", 1, 1},

                            Header{"LD H,B", 1, 1},
                            Header{"LD H,C", 1, 1},
                            Header{"LD H,D", 1, 1},
                            Header{"LD H,E", 1, 1},
                            Header{"LD H,H", 1, 1},
                            Header{"LD H,L", 1, 1},
                            Header{"LD H,(HL)", 1, 2},
                            Header{"LD H,A", 1, 1},
                            Header{"LD L,B", 1, 1},
                            Header{"LD L,C", 1, 1},
                            Header{"LD L,D", 1, 1},
                            Header{"LD L,E", 1, 1},
                            Header{"LD L,H", 1, 1},
                            Header{"LD L,L", 1, 1},
                            Header{"LD L,(HL)", 1, 2},
                            Header{"LD L,A", 1, 1},

                            Header{"LD (HL),B", 1, 2},
                            Header{"LD (HL),C", 1, 2},
                            Header{"LD (HL),D", 1, 2},
                            Header{"LD (HL),E", 1, 2},
                            Header{"LD (HL),H", 1, 2},
                            Header{"LD (HL),L", 1, 2},
                            Header{"HALT", 1, 1},
                            Header{"LD (HL),A", 1, 2},
                            Header{"LD A,B", 1, 1},
                            Header{"LD A,C", 1, 1},
                            Header{"LD A,D", 1, 1},
                            Header{"LD A,E", 1, 1},
                            Header{"LD A,H", 1, 1},
                            Header{"LD A,L", 1, 1},
                            Header{"LD A,(HL)", 1, 2},
                            Header{"LD A,A", 1, 1},

                            Header{"ADD A,B", 1, 1},
                            Header{"ADD A,C", 1, 1},
                            Header{"ADD A,D", 1, 1},
                            Header{"ADD A,E", 1, 1},
                            Header{"ADD A,H", 1, 1},
                            Header{"ADD A,L", 1, 1},
                            Header{"ADD A,(HL)", 1, 2},
                            Header{"ADD A,A", 1, 1},
                            Header{"ADC A,B", 1, 1},
                            Header{"ADC A,C", 1, 1},
                            Header{"ADC A,D", 1, 1},
                            Header{"ADC A,E", 1, 1},
                            Header{"ADC A,H", 1, 1},
                            Header{"ADC A,L", 1, 1},
                            Header{"ADC A,(HL)", 1, 2},
                            Header{"ADC A,A", 1, 1},

                            Header{"SUB A,B", 1, 1},
                            Header{"SUB A,C", 1, 1},
                            Header{"SUB A,D", 1, 1},
                            Header{"SUB A,E", 1, 1},
                            Header{"SUB A,H", 1, 1},
                            Header{"SUB A,L", 1, 2},
                            Header{"SUB A,(HL)", 1, 1},
                            Header{"SUB A,A", 1, 1},
                            Header{"SBC A,B", 1, 1},
                            Header{"SBC A,C", 1, 1},
                            Header{"SBC A,D", 1, 1},
                            Header{"SBC A,E", 1, 1},
                            Header{"SBC A,H", 1, 1},
                            Header{"SBC A,L", 1, 2},
                            Header{"SBC A,(HL)", 1, 1},
                            Header{"SBC A,A", 1, 1},

                            Header{"AND A,B", 1, 1},
                            Header{"AND A,C", 1, 1},
                            Header{"AND A,D", 1, 1},
                            Header{"AND A,E", 1, 1},
                            Header{"AND A,H", 1, 1},
                            Header{"AND A,L", 1, 2},
                            Header{"AND A,(HL)", 1, 1},
                            Header{"AND A,A", 1, 1},
                            Header{"XOR A,B", 1, 1},
                            Header{"XOR A,C", 1, 1},
                            Header{"XOR A,D", 1, 1},
                            Header{"XOR A,E", 1, 1},
                            Header{"XOR A,H", 1, 1},
                            Header{"XOR A,L", 1, 2},
                            Header{"XOR A,(HL)", 1, 1},
                            Header{"XOR A,A", 1, 1},

                            Header{"OR A,B", 1, 1},
                            Header{"OR A,C", 1, 1},
                            Header{"OR A,D", 1, 1},
                            Header{"OR A,E", 1, 1},
                            Header{"OR A,H", 1, 1},
                            Header{"OR A,L", 1, 2},
                            Header{"OR A,(HL)", 1, 1},
                            Header{"OR A,A", 1, 1},
                            Header{"CP A,B", 1, 1},
                            Header{"CP A,C", 1, 1},
                            Header{"CP A,D", 1, 1},
                            Header{"CP A,E", 1, 1},
                            Header{"CP A,H", 1, 1},
                            Header{"CP A,L", 1, 2},
                            Header{"CP A,(HL)", 1, 1},
                            Header{"CP A,A", 1, 1},

                            Header{"RET NZ", 1, 5},
                            Header{"POP BC", 1, 3},
                            Header{"JP NZ,a16", 3, 4},
                            Header{"JP a16", 3, 4},
                            Header{"CALL NZ,a16", 3, 6},
                            Header{"PUSH BC", 1, 4},
                            Header{"ADD A,n8", 2, 2},
                            Header{"RST 0x00", 1, 4},
                            Header{"RET Z", 1, 5},
                            Header{"RET", 1, 4},
                            Header{"JP Z,a16", 3, 4},
                            Header{"PREFIX", 1, 1},
                            Header{"CALL Z,a16", 3, 6},
                            Header{"CALL a16", 3, 6},
                            Header{"ADC A,n8", 2, 2},
                            Header{"RST 0x08", 1, 4},

                            Header{"RET NC", 1, 5},
                            Header{"POP DE", 1, 3},
                            Header{"JP NC,a16", 3, 4},
                            Header{"invalid", 0, 0},
                            Header{"CALL NC,a16", 3, 6},
                            Header{"PUSH DE", 1, 4},
                            Header{"SUB A,n8", 2, 2},
                            Header{"RST 0x10", 1, 4},
                            Header{"RET C", 1, 5},
                            Header{"RETI", 1, 4},
                            Header{"JP C,a16", 3, 4},
                            Header{"invalid", 0, 0},
                            Header{"CALL C,a16", 3, 6},
                            Header{"invalid", 0, 0},
                            Header{"SBC A,n8", 2, 2},
                            Header{"RST 0x18", 1, 4},

                            Header{"LD ($FF00+a8),A", 2, 3},
                            Header{"POP HL", 1, 3},
                            Header{"LD ($FF00+C),A", 1, 2},
                            Header{"invalid", 0, 0},
                            Header{"invalid", 0, 0},
                            Header{"PUSH HL", 1, 4},
                            Header{"AND A, n8", 2, 2},
                            Header{"RST 0x20", 1, 4},
                            Header{"ADD SP, e8", 2, 4},
                            Header{"JP HL", 1, 1},
                            Header{"LD (a16),A", 3, 4},
                            Header{"invalid", 0, 0},
                            Header{"invalid", 0, 0},
                            Header{"invalid", 0, 0},
                            Header{"XOR A, n8", 2, 2},
                            Header{"RST 0x28", 1, 4},

                            Header{"LD A,($FF00+a8)", 2, 3},
                            Header{"POP AF", 1, 3},
                            Header{"LD A,($FF00+C)", 1, 2},
                            Header{"DI", 1, 1},
                            Header{"invalid", 0, 0},
                            Header{"PUSH AF", 1, 4},
                            Header{"OR A, n8", 2, 2},
                            Header{"RST 0x30", 1, 4},
                            Header{"LD HL, SP+e8", 2, 3},
                            Header{"LD SP,HL", 1, 2},
                            Header{"LD A,(a16)", 3, 4},
                            Header{"EI", 1, 1},
                            Header{"invalid", 0, 0},
                            Header{"invalid", 0, 0},
                            Header{"CP A, {:#04x}", 2, 2},
                            Header{"RST 0x38", 1, 4}
                    };

            cbInstructions =
                    {
                            Header{"RLC B", 1, 2},
                            Header{"RLC C", 1, 2},
                            Header{"RLC D", 1, 2},
                            Header{"RLC E", 1, 2},
                            Header{"RLC H", 1, 2},
                            Header{"RLC L", 1, 2},
                            Header{"RLC (HL)", 1, 4},
                            Header{"RLC A", 1, 2},
                            Header{"RRC B", 1, 2},
                            Header{"RRC C", 1, 2},
                            Header{"RRC D", 1, 2},
                            Header{"RRC E", 1, 2},
                            Header{"RRC H", 1, 2},
                            Header{"RRC L", 1, 2},
                            Header{"RRC (HL)", 1, 4},
                            Header{"RRC A", 1, 2},

                            Header{"RL B", 1, 2},
                            Header{"RL C", 1, 2},
                            Header{"RL D", 1, 2},
                            Header{"RL E", 1, 2},
                            Header{"RL H", 1, 2},
                            Header{"RL L", 1, 2},
                            Header{"RL (HL)", 1, 4},
                            Header{"RL A", 1, 2},
                            Header{"RR B", 1, 2},
                            Header{"RR C", 1, 2},
                            Header{"RR D", 1, 2},
                            Header{"RR E", 1, 2},
                            Header{"RR H", 1, 2},
                            Header{"RR L", 1, 2},
                            Header{"RR (HL)", 1, 4},
                            Header{"RR A", 1, 2},

                            Header{"SLA B", 1, 2},
                            Header{"SLA C", 1, 2},
                            Header{"SLA D", 1, 2},
                            Header{"SLA E", 1, 2},
                            Header{"SLA H", 1, 2},
                            Header{"SLA L", 1, 2},
                            Header{"SLA (HL)", 1, 4},
                            Header{"SLA A", 1, 2},
                            Header{"SRA B", 1, 2},
                            Header{"SRA C", 1, 2},
                            Header{"SRA D", 1, 2},
                            Header{"SRA E", 1, 2},
                            Header{"SRA H", 1, 2},
                            Header{"SRA L", 1, 2},
                            Header{"SRA (HL)", 1, 4},
                            Header{"SRA A", 1, 2},

                            Header{"SWAP B", 1, 2},
                            Header{"SWAP C", 1, 2},
                            Header{"SWAP D", 1, 2},
                            Header{"SWAP E", 1, 2},
                            Header{"SWAP H", 1, 2},
                            Header{"SWAP L", 1, 2},
                            Header{"SWAP (HL)", 1, 4},
                            Header{"SWAP A", 1, 2},
                            Header{"SRL B", 1, 2},
                            Header{"SRL C", 1, 2},
                            Header{"SRL D", 1, 2},
                            Header{"SRL E", 1, 2},
                            Header{"SRL H", 1, 2},
                            Header{"SRL L", 1, 2},
                            Header{"SRL (HL)", 1, 4},
                            Header{"SRL A", 1, 2},

                            Header{"BIT 0,B", 1, 2},
                            Header{"BIT 0,C", 1, 2},
                            Header{"BIT 0,D", 1, 2},
                            Header{"BIT 0,E", 1, 2},
                            Header{"BIT 0,H", 1, 2},
                            Header{"BIT 0,L", 1, 2},
                            Header{"BIT 0,(HL)", 1, 3},
                            Header{"BIT 0,A", 1, 2},
                            Header{"BIT 1,B", 1, 2},
                            Header{"BIT 1,C", 1, 2},
                            Header{"BIT 1,D", 1, 2},
                            Header{"BIT 1,E", 1, 2},
                            Header{"BIT 1,H", 1, 2},
                            Header{"BIT 1,L", 1, 2},
                            Header{"BIT 1,(HL)", 1, 3},
                            Header{"BIT 1,A", 1, 2},

                            Header{"BIT 2,B", 1, 2},
                            Header{"BIT 2,C", 1, 2},
                            Header{"BIT 2,D", 1, 2},
                            Header{"BIT 2,E", 1, 2},
                            Header{"BIT 2,H", 1, 2},
                            Header{"BIT 2,L", 1, 2},
                            Header{"BIT 2,(HL)", 1, 3},
                            Header{"BIT 2,A", 1, 2},
                            Header{"BIT 3,B", 1, 2},
                            Header{"BIT 3,C", 1, 2},
                            Header{"BIT 3,D", 1, 2},
                            Header{"BIT 3,E", 1, 2},
                            Header{"BIT 3,H", 1, 2},
                            Header{"BIT 3,L", 1, 2},
                            Header{"BIT 3,(HL)", 1, 3},
                            Header{"BIT 3,A", 1, 2},

                            Header{"BIT 4,B", 1, 2},
                            Header{"BIT 4,C", 1, 2},
                            Header{"BIT 4,D", 1, 2},
                            Header{"BIT 4,E", 1, 2},
                            Header{"BIT 4,H", 1, 2},
                            Header{"BIT 4,L", 1, 2},
                            Header{"BIT 4,(HL)", 1, 3},
                            Header{"BIT 4,A", 1, 2},
                            Header{"BIT 5,B", 1, 2},
                            Header{"BIT 5,C", 1, 2},
                            Header{"BIT 5,D", 1, 2},
                            Header{"BIT 5,E", 1, 2},
                            Header{"BIT 5,H", 1, 2},
                            Header{"BIT 5,L", 1, 2},
                            Header{"BIT 5,(HL)", 1, 3},
                            Header{"BIT 5,A", 1, 2},

                            Header{"BIT 6,B", 1, 2},
                            Header{"BIT 6,C", 1, 2},
                            Header{"BIT 6,D", 1, 2},
                            Header{"BIT 6,E", 1, 2},
                            Header{"BIT 6,H", 1, 2},
                            Header{"BIT 6,L", 1, 2},
                            Header{"BIT 6,(HL)", 1, 3},
                            Header{"BIT 6,A", 1, 2},
                            Header{"BIT 7,B", 1, 2},
                            Header{"BIT 7,C", 1, 2},
                            Header{"BIT 7,D", 1, 2},
                            Header{"BIT 7,E", 1, 2},
                            Header{"BIT 7,H", 1, 2},
                            Header{"BIT 7,L", 1, 2},
                            Header{"BIT 7,(HL)", 1, 3},
                            Header{"BIT 7,A", 1, 2},

                            Header{"RES 0,B", 1, 2},
                            Header{"RES 0,C", 1, 2},
                            Header{"RES 0,D", 1, 2},
                            Header{"RES 0,E", 1, 2},
                            Header{"RES 0,H", 1, 2},
                            Header{"RES 0,L", 1, 2},
                            Header{"RES 0,(HL)", 1, 4},
                            Header{"RES 0,A", 1, 2},
                            Header{"RES 1,B", 1, 2},
                            Header{"RES 1,C", 1, 2},
                            Header{"RES 1,D", 1, 2},
                            Header{"RES 1,E", 1, 2},
                            Header{"RES 1,H", 1, 2},
                            Header{"RES 1,L", 1, 2},
                            Header{"RES 1,(HL)", 1, 4},
                            Header{"RES 1,A", 1, 2},

                            Header{"RES 2,B", 1, 2},
                            Header{"RES 2,C", 1, 2},
                            Header{"RES 2,D", 1, 2},
                            Header{"RES 2,E", 1, 2},
                            Header{"RES 2,H", 1, 2},
                            Header{"RES 2,L", 1, 2},
                            Header{"RES 2,(HL)", 1, 4},
                            Header{"RES 2,A", 1, 2},
                            Header{"RES 3,B", 1, 2},
                            Header{"RES 3,C", 1, 2},
                            Header{"RES 3,D", 1, 2},
                            Header{"RES 3,E", 1, 2},
                            Header{"RES 3,H", 1, 2},
                            Header{"RES 3,L", 1, 2},
                            Header{"RES 3,(HL)", 1, 4},
                            Header{"RES 3,A", 1, 2},

                            Header{"RES 4,B", 1, 2},
                            Header{"RES 4,C", 1, 2},
                            Header{"RES 4,D", 1, 2},
                            Header{"RES 4,E", 1, 2},
                            Header{"RES 4,H", 1, 2},
                            Header{"RES 4,L", 1, 2},
                            Header{"RES 4,(HL)", 1, 4},
                            Header{"RES 4,A", 1, 2},
                            Header{"RES 5,B", 1, 2},
                            Header{"RES 5,C", 1, 2},
                            Header{"RES 5,D", 1, 2},
                            Header{"RES 5,E", 1, 2},
                            Header{"RES 5,H", 1, 2},
                            Header{"RES 5,L", 1, 2},
                            Header{"RES 5,(HL)", 1, 4},
                            Header{"RES 5,A", 1, 2},

                            Header{"RES 6,B", 1, 2},
                            Header{"RES 6,C", 1, 2},
                            Header{"RES 6,D", 1, 2},
                            Header{"RES 6,E", 1, 2},
                            Header{"RES 6,H", 1, 2},
                            Header{"RES 6,L", 1, 2},
                            Header{"RES 6,(HL)", 1, 4},
                            Header{"RES 6,A", 1, 2},
                            Header{"RES 7,B", 1, 2},
                            Header{"RES 7,C", 1, 2},
                            Header{"RES 7,D", 1, 2},
                            Header{"RES 7,E", 1, 2},
                            Header{"RES 7,H", 1, 2},
                            Header{"RES 7,L", 1, 2},
                            Header{"RES 7,(HL)", 1, 4},
                            Header{"RES 7,A", 1, 2},

                            Header{"SET 0,B", 1, 2},
                            Header{"SET 0,C", 1, 2},
                            Header{"SET 0,D", 1, 2},
                            Header{"SET 0,E", 1, 2},
                            Header{"SET 0,H", 1, 2},
                            Header{"SET 0,L", 1, 2},
                            Header{"SET 0,(HL)", 1, 4},
                            Header{"SET 0,A", 1, 2},
                            Header{"SET 1,B", 1, 2},
                            Header{"SET 1,C", 1, 2},
                            Header{"SET 1,D", 1, 2},
                            Header{"SET 1,E", 1, 2},
                            Header{"SET 1,H", 1, 2},
                            Header{"SET 1,L", 1, 2},
                            Header{"SET 1,(HL)", 1, 4},
                            Header{"SET 1,A", 1, 2},

                            Header{"SET 2,B", 1, 2},
                            Header{"SET 2,C", 1, 2},
                            Header{"SET 2,D", 1, 2},
                            Header{"SET 2,E", 1, 2},
                            Header{"SET 2,H", 1, 2},
                            Header{"SET 2,L", 1, 2},
                            Header{"SET 2,(HL)", 1, 4},
                            Header{"SET 2,A", 1, 2},
                            Header{"SET 3,B", 1, 2},
                            Header{"SET 3,C", 1, 2},
                            Header{"SET 3,D", 1, 2},
                            Header{"SET 3,E", 1, 2},
                            Header{"SET 3,H", 1, 2},
                            Header{"SET 3,L", 1, 2},
                            Header{"SET 3,(HL)", 1, 4},
                            Header{"SET 3,A", 1, 2},

                            Header{"SET 4,B", 1, 2},
                            Header{"SET 4,C", 1, 2},
                            Header{"SET 4,D", 1, 2},
                            Header{"SET 4,E", 1, 2},
                            Header{"SET 4,H", 1, 2},
                            Header{"SET 4,L", 1, 2},
                            Header{"SET 4,(HL)", 1, 4},
                            Header{"SET 4,A", 1, 2},
                            Header{"SET 5,B", 1, 2},
                            Header{"SET 5,C", 1, 2},
                            Header{"SET 5,D", 1, 2},
                            Header{"SET 5,E", 1, 2},
                            Header{"SET 5,H", 1, 2},
                            Header{"SET 5,L", 1, 2},
                            Header{"SET 5,(HL)", 1, 4},
                            Header{"SET 5,A", 1, 2},

                            Header{"SET 6,B", 1, 2},
                            Header{"SET 6,C", 1, 2},
                            Header{"SET 6,D", 1, 2},
                            Header{"SET 6,E", 1, 2},
                            Header{"SET 6,H", 1, 2},
                            Header{"SET 6,L", 1, 2},
                            Header{"SET 6,(HL)", 1, 4},
                            Header{"SET 6,A", 1, 2},
                            Header{"SET 7,B", 1, 2},
                            Header{"SET 7,C", 1, 2},
                            Header{"SET 7,D", 1, 2},
                            Header{"SET 7,E", 1, 2},
                            Header{"SET 7,H", 1, 2},
                            Header{"SET 7,L", 1, 2},
                            Header{"SET 7,(HL)", 1, 4},
                            Header{"SET 7,A", 1, 2}
                    };
        }
    }
}