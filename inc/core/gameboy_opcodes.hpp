
#ifndef GAMEBOY_OPCODES_HPP
#define GAMEBOY_OPCODES_HPP

#include <array>

struct Context;

std::array<int (*)(Context&), 256> create_table();
std::array<int (*)(Context&), 256> create_cb_table();

//NOP
int opcode_0x00(Context& ctx);
//LD BC,d16
int opcode_0x01(Context& ctx);
//LD (BC),A
int opcode_0x02(Context& ctx);
//INC BC
int opcode_0x03(Context& ctx);
//INC B
int opcode_0x04(Context& ctx);
//DEC B
int opcode_0x05(Context& ctx);
//LD B,d8
int opcode_0x06(Context& ctx);
//RLCA
int opcode_0x07(Context& ctx);
//LD (a16),SP
int opcode_0x08(Context& ctx);
//ADD HL,BC
int opcode_0x09(Context& ctx);
//LD A,(BC)
int opcode_0x0a(Context& ctx);
//DEC BC
int opcode_0x0b(Context& ctx);
//INC C
int opcode_0x0c(Context& ctx);
//DEC C
int opcode_0x0d(Context& ctx);
//LD C,d8
int opcode_0x0e(Context& ctx);
//RRCA
int opcode_0x0f(Context& ctx);
//STOP 0
int opcode_0x10(Context& ctx);
//LD DE,d16
int opcode_0x11(Context& ctx);
//LD (DE),A
int opcode_0x12(Context& ctx);
//INC DE
int opcode_0x13(Context& ctx);
//INC D
int opcode_0x14(Context& ctx);
//DEC D
int opcode_0x15(Context& ctx);
//LD D,d8
int opcode_0x16(Context& ctx);
//RLA
int opcode_0x17(Context& ctx);
//JR r8
int opcode_0x18(Context& ctx);
//ADD HL,DE
int opcode_0x19(Context& ctx);
//LD A,(DE)
int opcode_0x1a(Context& ctx);
//DEC DE
int opcode_0x1b(Context& ctx);
//INC E
int opcode_0x1c(Context& ctx);
//DEC E
int opcode_0x1d(Context& ctx);
//LD E,d8
int opcode_0x1e(Context& ctx);
//RRA
int opcode_0x1f(Context& ctx);
//JR NZ,r8
int opcode_0x20(Context& ctx);
//LD HL,d16
int opcode_0x21(Context& ctx);
//LD (HL+),A
int opcode_0x22(Context& ctx);
//INC HL
int opcode_0x23(Context& ctx);
//INC H
int opcode_0x24(Context& ctx);
//DEC H
int opcode_0x25(Context& ctx);
//LD H,d8
int opcode_0x26(Context& ctx);
//DAA
int opcode_0x27(Context& ctx);
//JR Z,r8
int opcode_0x28(Context& ctx);
//ADD HL,HL
int opcode_0x29(Context& ctx);
//LD A,(HL+)
int opcode_0x2a(Context& ctx);
//DEC HL
int opcode_0x2b(Context& ctx);
//INC L
int opcode_0x2c(Context& ctx);
//DEC L
int opcode_0x2d(Context& ctx);
//LD L,d8
int opcode_0x2e(Context& ctx);
//CPL
int opcode_0x2f(Context& ctx);
//JR NC,r8
int opcode_0x30(Context& ctx);
//LD SP,d16
int opcode_0x31(Context& ctx);
//LD (HL-),A
int opcode_0x32(Context& ctx);
//INC SP
int opcode_0x33(Context& ctx);
//INC (HL)
int opcode_0x34(Context& ctx);
//DEC (HL)
int opcode_0x35(Context& ctx);
//LD (HL),d8
int opcode_0x36(Context& ctx);
//SCF
int opcode_0x37(Context& ctx);
//JR C,r8
int opcode_0x38(Context& ctx);
//ADD HL,SP
int opcode_0x39(Context& ctx);
//LD A,(HL-)
int opcode_0x3a(Context& ctx);
//DEC SP
int opcode_0x3b(Context& ctx);
//INC A
int opcode_0x3c(Context& ctx);
//DEC A
int opcode_0x3d(Context& ctx);
//LD A,d8
int opcode_0x3e(Context& ctx);
//CCF
int opcode_0x3f(Context& ctx);
//LD B,B
int opcode_0x40(Context& ctx);
//LD B,C
int opcode_0x41(Context& ctx);
//LD B,D
int opcode_0x42(Context& ctx);
//LD B,E
int opcode_0x43(Context& ctx);
//LD B,H
int opcode_0x44(Context& ctx);
//LD B,L
int opcode_0x45(Context& ctx);
//LD B,(HL)
int opcode_0x46(Context& ctx);
//LD B,A
int opcode_0x47(Context& ctx);
//LD C,B
int opcode_0x48(Context& ctx);
//LD C,C
int opcode_0x49(Context& ctx);
//LD C,D
int opcode_0x4a(Context& ctx);
//LD C,E
int opcode_0x4b(Context& ctx);
//LD C,H
int opcode_0x4c(Context& ctx);
//LD C,L
int opcode_0x4d(Context& ctx);
//LD C,(HL)
int opcode_0x4e(Context& ctx);
//LD C,A
int opcode_0x4f(Context& ctx);
//LD D,B
int opcode_0x50(Context& ctx);
//LD D,C
int opcode_0x51(Context& ctx);
//LD D,D
int opcode_0x52(Context& ctx);
//LD D,E
int opcode_0x53(Context& ctx);
//LD D,H
int opcode_0x54(Context& ctx);
//LD D,L
int opcode_0x55(Context& ctx);
//LD D,(HL)
int opcode_0x56(Context& ctx);
//LD D,A
int opcode_0x57(Context& ctx);
//LD E,B
int opcode_0x58(Context& ctx);
//LD E,C
int opcode_0x59(Context& ctx);
//LD E,D
int opcode_0x5a(Context& ctx);
//LD E,E
int opcode_0x5b(Context& ctx);
//LD E,H
int opcode_0x5c(Context& ctx);
//LD E,L
int opcode_0x5d(Context& ctx);
//LD E,(HL)
int opcode_0x5e(Context& ctx);
//LD E,A
int opcode_0x5f(Context& ctx);
//LD H,B
int opcode_0x60(Context& ctx);
//LD H,C
int opcode_0x61(Context& ctx);
//LD H,D
int opcode_0x62(Context& ctx);
//LD H,E
int opcode_0x63(Context& ctx);
//LD H,H
int opcode_0x64(Context& ctx);
//LD H,L
int opcode_0x65(Context& ctx);
//LD H,(HL)
int opcode_0x66(Context& ctx);
//LD H,A
int opcode_0x67(Context& ctx);
//LD L,B
int opcode_0x68(Context& ctx);
//LD L,C
int opcode_0x69(Context& ctx);
//LD L,D
int opcode_0x6a(Context& ctx);
//LD L,E
int opcode_0x6b(Context& ctx);
//LD L,H
int opcode_0x6c(Context& ctx);
//LD L,L
int opcode_0x6d(Context& ctx);
//LD L,(HL)
int opcode_0x6e(Context& ctx);
//LD L,A
int opcode_0x6f(Context& ctx);
//LD (HL),B
int opcode_0x70(Context& ctx);
//LD (HL),C
int opcode_0x71(Context& ctx);
//LD (HL),D
int opcode_0x72(Context& ctx);
//LD (HL),E
int opcode_0x73(Context& ctx);
//LD (HL),H
int opcode_0x74(Context& ctx);
//LD (HL),L
int opcode_0x75(Context& ctx);
//HALT
int opcode_0x76(Context& ctx);
//LD (HL),A
int opcode_0x77(Context& ctx);
//LD A,B
int opcode_0x78(Context& ctx);
//LD A,C
int opcode_0x79(Context& ctx);
//LD A,D
int opcode_0x7a(Context& ctx);
//LD A,E
int opcode_0x7b(Context& ctx);
//LD A,H
int opcode_0x7c(Context& ctx);
//LD A,L
int opcode_0x7d(Context& ctx);
//LD A,(HL)
int opcode_0x7e(Context& ctx);
//LD A,A
int opcode_0x7f(Context& ctx);
//ADD A,B
int opcode_0x80(Context& ctx);
//ADD A,C
int opcode_0x81(Context& ctx);
//ADD A,D
int opcode_0x82(Context& ctx);
//ADD A,E
int opcode_0x83(Context& ctx);
//ADD A,H
int opcode_0x84(Context& ctx);
//ADD A,L
int opcode_0x85(Context& ctx);
//ADD A,(HL)
int opcode_0x86(Context& ctx);
//ADD A,A
int opcode_0x87(Context& ctx);
//ADC A,B
int opcode_0x88(Context& ctx);
//ADC A,C
int opcode_0x89(Context& ctx);
//ADC A,D
int opcode_0x8a(Context& ctx);
//ADC A,E
int opcode_0x8b(Context& ctx);
//ADC A,H
int opcode_0x8c(Context& ctx);
//ADC A,L
int opcode_0x8d(Context& ctx);
//ADC A,(HL)
int opcode_0x8e(Context& ctx);
//ADC A,A
int opcode_0x8f(Context& ctx);
//SUB B
int opcode_0x90(Context& ctx);
//SUB C
int opcode_0x91(Context& ctx);
//SUB D
int opcode_0x92(Context& ctx);
//SUB E
int opcode_0x93(Context& ctx);
//SUB H
int opcode_0x94(Context& ctx);
//SUB L
int opcode_0x95(Context& ctx);
//SUB (HL)
int opcode_0x96(Context& ctx);
//SUB A
int opcode_0x97(Context& ctx);
//SBC A,B
int opcode_0x98(Context& ctx);
//SBC A,C
int opcode_0x99(Context& ctx);
//SBC A,D
int opcode_0x9a(Context& ctx);
//SBC A,E
int opcode_0x9b(Context& ctx);
//SBC A,H
int opcode_0x9c(Context& ctx);
//SBC A,L
int opcode_0x9d(Context& ctx);
//SBC A,(HL)
int opcode_0x9e(Context& ctx);
//SBC A,A
int opcode_0x9f(Context& ctx);
//AND B
int opcode_0xa0(Context& ctx);
//AND C
int opcode_0xa1(Context& ctx);
//AND D
int opcode_0xa2(Context& ctx);
//AND E
int opcode_0xa3(Context& ctx);
//AND H
int opcode_0xa4(Context& ctx);
//AND L
int opcode_0xa5(Context& ctx);
//AND (HL)
int opcode_0xa6(Context& ctx);
//AND A
int opcode_0xa7(Context& ctx);
//XOR B
int opcode_0xa8(Context& ctx);
//XOR C
int opcode_0xa9(Context& ctx);
//XOR D
int opcode_0xaa(Context& ctx);
//XOR E
int opcode_0xab(Context& ctx);
//XOR H
int opcode_0xac(Context& ctx);
//XOR L
int opcode_0xad(Context& ctx);
//XOR (HL)
int opcode_0xae(Context& ctx);
//XOR A
int opcode_0xaf(Context& ctx);
//OR B
int opcode_0xb0(Context& ctx);
//OR C
int opcode_0xb1(Context& ctx);
//OR D
int opcode_0xb2(Context& ctx);
//OR E
int opcode_0xb3(Context& ctx);
//OR H
int opcode_0xb4(Context& ctx);
//OR L
int opcode_0xb5(Context& ctx);
//OR (HL)
int opcode_0xb6(Context& ctx);
//OR A
int opcode_0xb7(Context& ctx);
//CP B
int opcode_0xb8(Context& ctx);
//CP C
int opcode_0xb9(Context& ctx);
//CP D
int opcode_0xba(Context& ctx);
//CP E
int opcode_0xbb(Context& ctx);
//CP H
int opcode_0xbc(Context& ctx);
//CP L
int opcode_0xbd(Context& ctx);
//CP (HL)
int opcode_0xbe(Context& ctx);
//CP A
int opcode_0xbf(Context& ctx);
//RET NZ
int opcode_0xc0(Context& ctx);
//POP BC
int opcode_0xc1(Context& ctx);
//JP NZ,a16
int opcode_0xc2(Context& ctx);
//JP a16
int opcode_0xc3(Context& ctx);
//CALL NZ,a16
int opcode_0xc4(Context& ctx);
//PUSH BC
int opcode_0xc5(Context& ctx);
//ADD A,d8
int opcode_0xc6(Context& ctx);
//RST 00H
int opcode_0xc7(Context& ctx);
//RET Z
int opcode_0xc8(Context& ctx);
//RET
int opcode_0xc9(Context& ctx);
//JP Z,a16
int opcode_0xca(Context& ctx);
//CALL Z,a16
int opcode_0xcc(Context& ctx);
//CALL a16
int opcode_0xcd(Context& ctx);
//ADC A,d8
int opcode_0xce(Context& ctx);
//RST 08H
int opcode_0xcf(Context& ctx);
//RET NC
int opcode_0xd0(Context& ctx);
//POP DE
int opcode_0xd1(Context& ctx);
//JP NC,a16
int opcode_0xd2(Context& ctx);
int opcode_0xd3(Context& ctx);
//CALL NC,a16
int opcode_0xd4(Context& ctx);
//PUSH DE
int opcode_0xd5(Context& ctx);
//SUB d8
int opcode_0xd6(Context& ctx);
//RST 10H
int opcode_0xd7(Context& ctx);
//RET C
int opcode_0xd8(Context& ctx);
//RETI
int opcode_0xd9(Context& ctx);
//JP C,a16
int opcode_0xda(Context& ctx);
int opcode_0xdb(Context& ctx);
//CALL C,a16
int opcode_0xdc(Context& ctx);
int opcode_0xdd(Context& ctx);
//SBC A,d8
int opcode_0xde(Context& ctx);
//RST 18H
int opcode_0xdf(Context& ctx);
//LDH (a8),A
int opcode_0xe0(Context& ctx);
//POP HL
int opcode_0xe1(Context& ctx);
//LD (C),A
int opcode_0xe2(Context& ctx);
int opcode_0xe3(Context& ctx);
int opcode_0xe4(Context& ctx);
//PUSH HL
int opcode_0xe5(Context& ctx);
//AND d8
int opcode_0xe6(Context& ctx);
//RST 20H
int opcode_0xe7(Context& ctx);
//ADD SP,r8
int opcode_0xe8(Context& ctx);
//JP (HL)
int opcode_0xe9(Context& ctx);
//LD (a16),A
int opcode_0xea(Context& ctx);
int opcode_0xeb(Context& ctx);
int opcode_0xec(Context& ctx);
int opcode_0xed(Context& ctx);
//XOR d8
int opcode_0xee(Context& ctx);
//RST 28H
int opcode_0xef(Context& ctx);
//LDH A,(a8)
int opcode_0xf0(Context& ctx);
//POP AF
int opcode_0xf1(Context& ctx);
//LD A,(C)
int opcode_0xf2(Context& ctx);
//DI
int opcode_0xf3(Context& ctx);
int opcode_0xf4(Context& ctx);
//PUSH AF
int opcode_0xf5(Context& ctx);
//OR d8
int opcode_0xf6(Context& ctx);
//RST 30H
int opcode_0xf7(Context& ctx);
//LD HL,SP+r8
int opcode_0xf8(Context& ctx);
//LD SP,HL
int opcode_0xf9(Context& ctx);
//LD A,(a16)
int opcode_0xfa(Context& ctx);
//EI
int opcode_0xfb(Context& ctx);
int opcode_0xfc(Context& ctx);
int opcode_0xfd(Context& ctx);
//CP d8
int opcode_0xfe(Context& ctx);
//RST 38H
int opcode_0xff(Context& ctx);
//RLC B
int opcode_0xcb00(Context& ctx);
//RLC C
int opcode_0xcb01(Context& ctx);
//RLC D
int opcode_0xcb02(Context& ctx);
//RLC E
int opcode_0xcb03(Context& ctx);
//RLC H
int opcode_0xcb04(Context& ctx);
//RLC L
int opcode_0xcb05(Context& ctx);
//RLC (HL)
int opcode_0xcb06(Context& ctx);
//RLC A
int opcode_0xcb07(Context& ctx);
//RRC B
int opcode_0xcb08(Context& ctx);
//RRC C
int opcode_0xcb09(Context& ctx);
//RRC D
int opcode_0xcb0a(Context& ctx);
//RRC E
int opcode_0xcb0b(Context& ctx);
//RRC H
int opcode_0xcb0c(Context& ctx);
//RRC L
int opcode_0xcb0d(Context& ctx);
//RRC (HL)
int opcode_0xcb0e(Context& ctx);
//RRC A
int opcode_0xcb0f(Context& ctx);
//RL B
int opcode_0xcb10(Context& ctx);
//RL C
int opcode_0xcb11(Context& ctx);
//RL D
int opcode_0xcb12(Context& ctx);
//RL E
int opcode_0xcb13(Context& ctx);
//RL H
int opcode_0xcb14(Context& ctx);
//RL L
int opcode_0xcb15(Context& ctx);
//RL (HL)
int opcode_0xcb16(Context& ctx);
//RL A
int opcode_0xcb17(Context& ctx);
//RR B
int opcode_0xcb18(Context& ctx);
//RR C
int opcode_0xcb19(Context& ctx);
//RR D
int opcode_0xcb1a(Context& ctx);
//RR E
int opcode_0xcb1b(Context& ctx);
//RR H
int opcode_0xcb1c(Context& ctx);
//RR L
int opcode_0xcb1d(Context& ctx);
//RR (HL)
int opcode_0xcb1e(Context& ctx);
//RR A
int opcode_0xcb1f(Context& ctx);
//SLA B
int opcode_0xcb20(Context& ctx);
//SLA C
int opcode_0xcb21(Context& ctx);
//SLA D
int opcode_0xcb22(Context& ctx);
//SLA E
int opcode_0xcb23(Context& ctx);
//SLA H
int opcode_0xcb24(Context& ctx);
//SLA L
int opcode_0xcb25(Context& ctx);
//SLA (HL)
int opcode_0xcb26(Context& ctx);
//SLA A
int opcode_0xcb27(Context& ctx);
//SRA B
int opcode_0xcb28(Context& ctx);
//SRA C
int opcode_0xcb29(Context& ctx);
//SRA D
int opcode_0xcb2a(Context& ctx);
//SRA E
int opcode_0xcb2b(Context& ctx);
//SRA H
int opcode_0xcb2c(Context& ctx);
//SRA L
int opcode_0xcb2d(Context& ctx);
//SRA (HL)
int opcode_0xcb2e(Context& ctx);
//SRA A
int opcode_0xcb2f(Context& ctx);
//SWAP B
int opcode_0xcb30(Context& ctx);
//SWAP C
int opcode_0xcb31(Context& ctx);
//SWAP D
int opcode_0xcb32(Context& ctx);
//SWAP E
int opcode_0xcb33(Context& ctx);
//SWAP H
int opcode_0xcb34(Context& ctx);
//SWAP L
int opcode_0xcb35(Context& ctx);
//SWAP (HL)
int opcode_0xcb36(Context& ctx);
//SWAP A
int opcode_0xcb37(Context& ctx);
//SRL B
int opcode_0xcb38(Context& ctx);
//SRL C
int opcode_0xcb39(Context& ctx);
//SRL D
int opcode_0xcb3a(Context& ctx);
//SRL E
int opcode_0xcb3b(Context& ctx);
//SRL H
int opcode_0xcb3c(Context& ctx);
//SRL L
int opcode_0xcb3d(Context& ctx);
//SRL (HL)
int opcode_0xcb3e(Context& ctx);
//SRL A
int opcode_0xcb3f(Context& ctx);
//BIT 0,B
int opcode_0xcb40(Context& ctx);
//BIT 0,C
int opcode_0xcb41(Context& ctx);
//BIT 0,D
int opcode_0xcb42(Context& ctx);
//BIT 0,E
int opcode_0xcb43(Context& ctx);
//BIT 0,H
int opcode_0xcb44(Context& ctx);
//BIT 0,L
int opcode_0xcb45(Context& ctx);
//BIT 0,(HL)
int opcode_0xcb46(Context& ctx);
//BIT 0,A
int opcode_0xcb47(Context& ctx);
//BIT 1,B
int opcode_0xcb48(Context& ctx);
//BIT 1,C
int opcode_0xcb49(Context& ctx);
//BIT 1,D
int opcode_0xcb4a(Context& ctx);
//BIT 1,E
int opcode_0xcb4b(Context& ctx);
//BIT 1,H
int opcode_0xcb4c(Context& ctx);
//BIT 1,L
int opcode_0xcb4d(Context& ctx);
//BIT 1,(HL)
int opcode_0xcb4e(Context& ctx);
//BIT 1,A
int opcode_0xcb4f(Context& ctx);
//BIT 2,B
int opcode_0xcb50(Context& ctx);
//BIT 2,C
int opcode_0xcb51(Context& ctx);
//BIT 2,D
int opcode_0xcb52(Context& ctx);
//BIT 2,E
int opcode_0xcb53(Context& ctx);
//BIT 2,H
int opcode_0xcb54(Context& ctx);
//BIT 2,L
int opcode_0xcb55(Context& ctx);
//BIT 2,(HL)
int opcode_0xcb56(Context& ctx);
//BIT 2,A
int opcode_0xcb57(Context& ctx);
//BIT 3,B
int opcode_0xcb58(Context& ctx);
//BIT 3,C
int opcode_0xcb59(Context& ctx);
//BIT 3,D
int opcode_0xcb5a(Context& ctx);
//BIT 3,E
int opcode_0xcb5b(Context& ctx);
//BIT 3,H
int opcode_0xcb5c(Context& ctx);
//BIT 3,L
int opcode_0xcb5d(Context& ctx);
//BIT 3,(HL)
int opcode_0xcb5e(Context& ctx);
//BIT 3,A
int opcode_0xcb5f(Context& ctx);
//BIT 4,B
int opcode_0xcb60(Context& ctx);
//BIT 4,C
int opcode_0xcb61(Context& ctx);
//BIT 4,D
int opcode_0xcb62(Context& ctx);
//BIT 4,E
int opcode_0xcb63(Context& ctx);
//BIT 4,H
int opcode_0xcb64(Context& ctx);
//BIT 4,L
int opcode_0xcb65(Context& ctx);
//BIT 4,(HL)
int opcode_0xcb66(Context& ctx);
//BIT 4,A
int opcode_0xcb67(Context& ctx);
//BIT 5,B
int opcode_0xcb68(Context& ctx);
//BIT 5,C
int opcode_0xcb69(Context& ctx);
//BIT 5,D
int opcode_0xcb6a(Context& ctx);
//BIT 5,E
int opcode_0xcb6b(Context& ctx);
//BIT 5,H
int opcode_0xcb6c(Context& ctx);
//BIT 5,L
int opcode_0xcb6d(Context& ctx);
//BIT 5,(HL)
int opcode_0xcb6e(Context& ctx);
//BIT 5,A
int opcode_0xcb6f(Context& ctx);
//BIT 6,B
int opcode_0xcb70(Context& ctx);
//BIT 6,C
int opcode_0xcb71(Context& ctx);
//BIT 6,D
int opcode_0xcb72(Context& ctx);
//BIT 6,E
int opcode_0xcb73(Context& ctx);
//BIT 6,H
int opcode_0xcb74(Context& ctx);
//BIT 6,L
int opcode_0xcb75(Context& ctx);
//BIT 6,(HL)
int opcode_0xcb76(Context& ctx);
//BIT 6,A
int opcode_0xcb77(Context& ctx);
//BIT 7,B
int opcode_0xcb78(Context& ctx);
//BIT 7,C
int opcode_0xcb79(Context& ctx);
//BIT 7,D
int opcode_0xcb7a(Context& ctx);
//BIT 7,E
int opcode_0xcb7b(Context& ctx);
//BIT 7,H
int opcode_0xcb7c(Context& ctx);
//BIT 7,L
int opcode_0xcb7d(Context& ctx);
//BIT 7,(HL)
int opcode_0xcb7e(Context& ctx);
//BIT 7,A
int opcode_0xcb7f(Context& ctx);
//RES 0,B
int opcode_0xcb80(Context& ctx);
//RES 0,C
int opcode_0xcb81(Context& ctx);
//RES 0,D
int opcode_0xcb82(Context& ctx);
//RES 0,E
int opcode_0xcb83(Context& ctx);
//RES 0,H
int opcode_0xcb84(Context& ctx);
//RES 0,L
int opcode_0xcb85(Context& ctx);
//RES 0,(HL)
int opcode_0xcb86(Context& ctx);
//RES 0,A
int opcode_0xcb87(Context& ctx);
//RES 1,B
int opcode_0xcb88(Context& ctx);
//RES 1,C
int opcode_0xcb89(Context& ctx);
//RES 1,D
int opcode_0xcb8a(Context& ctx);
//RES 1,E
int opcode_0xcb8b(Context& ctx);
//RES 1,H
int opcode_0xcb8c(Context& ctx);
//RES 1,L
int opcode_0xcb8d(Context& ctx);
//RES 1,(HL)
int opcode_0xcb8e(Context& ctx);
//RES 1,A
int opcode_0xcb8f(Context& ctx);
//RES 2,B
int opcode_0xcb90(Context& ctx);
//RES 2,C
int opcode_0xcb91(Context& ctx);
//RES 2,D
int opcode_0xcb92(Context& ctx);
//RES 2,E
int opcode_0xcb93(Context& ctx);
//RES 2,H
int opcode_0xcb94(Context& ctx);
//RES 2,L
int opcode_0xcb95(Context& ctx);
//RES 2,(HL)
int opcode_0xcb96(Context& ctx);
//RES 2,A
int opcode_0xcb97(Context& ctx);
//RES 3,B
int opcode_0xcb98(Context& ctx);
//RES 3,C
int opcode_0xcb99(Context& ctx);
//RES 3,D
int opcode_0xcb9a(Context& ctx);
//RES 3,E
int opcode_0xcb9b(Context& ctx);
//RES 3,H
int opcode_0xcb9c(Context& ctx);
//RES 3,L
int opcode_0xcb9d(Context& ctx);
//RES 3,(HL)
int opcode_0xcb9e(Context& ctx);
//RES 3,A
int opcode_0xcb9f(Context& ctx);
//RES 4,B
int opcode_0xcba0(Context& ctx);
//RES 4,C
int opcode_0xcba1(Context& ctx);
//RES 4,D
int opcode_0xcba2(Context& ctx);
//RES 4,E
int opcode_0xcba3(Context& ctx);
//RES 4,H
int opcode_0xcba4(Context& ctx);
//RES 4,L
int opcode_0xcba5(Context& ctx);
//RES 4,(HL)
int opcode_0xcba6(Context& ctx);
//RES 4,A
int opcode_0xcba7(Context& ctx);
//RES 5,B
int opcode_0xcba8(Context& ctx);
//RES 5,C
int opcode_0xcba9(Context& ctx);
//RES 5,D
int opcode_0xcbaa(Context& ctx);
//RES 5,E
int opcode_0xcbab(Context& ctx);
//RES 5,H
int opcode_0xcbac(Context& ctx);
//RES 5,L
int opcode_0xcbad(Context& ctx);
//RES 5,(HL)
int opcode_0xcbae(Context& ctx);
//RES 5,A
int opcode_0xcbaf(Context& ctx);
//RES 6,B
int opcode_0xcbb0(Context& ctx);
//RES 6,C
int opcode_0xcbb1(Context& ctx);
//RES 6,D
int opcode_0xcbb2(Context& ctx);
//RES 6,E
int opcode_0xcbb3(Context& ctx);
//RES 6,H
int opcode_0xcbb4(Context& ctx);
//RES 6,L
int opcode_0xcbb5(Context& ctx);
//RES 6,(HL)
int opcode_0xcbb6(Context& ctx);
//RES 6,A
int opcode_0xcbb7(Context& ctx);
//RES 7,B
int opcode_0xcbb8(Context& ctx);
//RES 7,C
int opcode_0xcbb9(Context& ctx);
//RES 7,D
int opcode_0xcbba(Context& ctx);
//RES 7,E
int opcode_0xcbbb(Context& ctx);
//RES 7,H
int opcode_0xcbbc(Context& ctx);
//RES 7,L
int opcode_0xcbbd(Context& ctx);
//RES 7,(HL)
int opcode_0xcbbe(Context& ctx);
//RES 7,A
int opcode_0xcbbf(Context& ctx);
//SET 0,B
int opcode_0xcbc0(Context& ctx);
//SET 0,C
int opcode_0xcbc1(Context& ctx);
//SET 0,D
int opcode_0xcbc2(Context& ctx);
//SET 0,E
int opcode_0xcbc3(Context& ctx);
//SET 0,H
int opcode_0xcbc4(Context& ctx);
//SET 0,L
int opcode_0xcbc5(Context& ctx);
//SET 0,(HL)
int opcode_0xcbc6(Context& ctx);
//SET 0,A
int opcode_0xcbc7(Context& ctx);
//SET 1,B
int opcode_0xcbc8(Context& ctx);
//SET 1,C
int opcode_0xcbc9(Context& ctx);
//SET 1,D
int opcode_0xcbca(Context& ctx);
//SET 1,E
int opcode_0xcbcb(Context& ctx);
//SET 1,H
int opcode_0xcbcc(Context& ctx);
//SET 1,L
int opcode_0xcbcd(Context& ctx);
//SET 1,(HL)
int opcode_0xcbce(Context& ctx);
//SET 1,A
int opcode_0xcbcf(Context& ctx);
//SET 2,B
int opcode_0xcbd0(Context& ctx);
//SET 2,C
int opcode_0xcbd1(Context& ctx);
//SET 2,D
int opcode_0xcbd2(Context& ctx);
//SET 2,E
int opcode_0xcbd3(Context& ctx);
//SET 2,H
int opcode_0xcbd4(Context& ctx);
//SET 2,L
int opcode_0xcbd5(Context& ctx);
//SET 2,(HL)
int opcode_0xcbd6(Context& ctx);
//SET 2,A
int opcode_0xcbd7(Context& ctx);
//SET 3,B
int opcode_0xcbd8(Context& ctx);
//SET 3,C
int opcode_0xcbd9(Context& ctx);
//SET 3,D
int opcode_0xcbda(Context& ctx);
//SET 3,E
int opcode_0xcbdb(Context& ctx);
//SET 3,H
int opcode_0xcbdc(Context& ctx);
//SET 3,L
int opcode_0xcbdd(Context& ctx);
//SET 3,(HL)
int opcode_0xcbde(Context& ctx);
//SET 3,A
int opcode_0xcbdf(Context& ctx);
//SET 4,B
int opcode_0xcbe0(Context& ctx);
//SET 4,C
int opcode_0xcbe1(Context& ctx);
//SET 4,D
int opcode_0xcbe2(Context& ctx);
//SET 4,E
int opcode_0xcbe3(Context& ctx);
//SET 4,H
int opcode_0xcbe4(Context& ctx);
//SET 4,L
int opcode_0xcbe5(Context& ctx);
//SET 4,(HL)
int opcode_0xcbe6(Context& ctx);
//SET 4,A
int opcode_0xcbe7(Context& ctx);
//SET 5,B
int opcode_0xcbe8(Context& ctx);
//SET 5,C
int opcode_0xcbe9(Context& ctx);
//SET 5,D
int opcode_0xcbea(Context& ctx);
//SET 5,E
int opcode_0xcbeb(Context& ctx);
//SET 5,H
int opcode_0xcbec(Context& ctx);
//SET 5,L
int opcode_0xcbed(Context& ctx);
//SET 5,(HL)
int opcode_0xcbee(Context& ctx);
//SET 5,A
int opcode_0xcbef(Context& ctx);
//SET 6,B
int opcode_0xcbf0(Context& ctx);
//SET 6,C
int opcode_0xcbf1(Context& ctx);
//SET 6,D
int opcode_0xcbf2(Context& ctx);
//SET 6,E
int opcode_0xcbf3(Context& ctx);
//SET 6,H
int opcode_0xcbf4(Context& ctx);
//SET 6,L
int opcode_0xcbf5(Context& ctx);
//SET 6,(HL)
int opcode_0xcbf6(Context& ctx);
//SET 6,A
int opcode_0xcbf7(Context& ctx);
//SET 7,B
int opcode_0xcbf8(Context& ctx);
//SET 7,C
int opcode_0xcbf9(Context& ctx);
//SET 7,D
int opcode_0xcbfa(Context& ctx);
//SET 7,E
int opcode_0xcbfb(Context& ctx);
//SET 7,H
int opcode_0xcbfc(Context& ctx);
//SET 7,L
int opcode_0xcbfd(Context& ctx);
//SET 7,(HL)
int opcode_0xcbfe(Context& ctx);
//SET 7,A
int opcode_0xcbff(Context& ctx);

#endif