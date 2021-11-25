
#ifndef GAMEBOY_OPCODES_HPP
#define GAMEBOY_OPCODES_HPP

#include <array>

namespace core {
    struct Context;
}

std::array<int (*)(core::Context&), 256> create_table();
std::array<int (*)(core::Context&), 256> create_cb_table();

//NOP
int opcode_0x00(core::Context& ctx);
//LD BC,d16
int opcode_0x01(core::Context& ctx);
//LD (BC),A
int opcode_0x02(core::Context& ctx);
//INC BC
int opcode_0x03(core::Context& ctx);
//INC B
int opcode_0x04(core::Context& ctx);
//DEC B
int opcode_0x05(core::Context& ctx);
//LD B,d8
int opcode_0x06(core::Context& ctx);
//RLCA
int opcode_0x07(core::Context& ctx);
//LD (a16),SP
int opcode_0x08(core::Context& ctx);
//ADD HL,BC
int opcode_0x09(core::Context& ctx);
//LD A,(BC)
int opcode_0x0a(core::Context& ctx);
//DEC BC
int opcode_0x0b(core::Context& ctx);
//INC C
int opcode_0x0c(core::Context& ctx);
//DEC C
int opcode_0x0d(core::Context& ctx);
//LD C,d8
int opcode_0x0e(core::Context& ctx);
//RRCA
int opcode_0x0f(core::Context& ctx);
//STOP 0
int opcode_0x10(core::Context& ctx);
//LD DE,d16
int opcode_0x11(core::Context& ctx);
//LD (DE),A
int opcode_0x12(core::Context& ctx);
//INC DE
int opcode_0x13(core::Context& ctx);
//INC D
int opcode_0x14(core::Context& ctx);
//DEC D
int opcode_0x15(core::Context& ctx);
//LD D,d8
int opcode_0x16(core::Context& ctx);
//RLA
int opcode_0x17(core::Context& ctx);
//JR r8
int opcode_0x18(core::Context& ctx);
//ADD HL,DE
int opcode_0x19(core::Context& ctx);
//LD A,(DE)
int opcode_0x1a(core::Context& ctx);
//DEC DE
int opcode_0x1b(core::Context& ctx);
//INC E
int opcode_0x1c(core::Context& ctx);
//DEC E
int opcode_0x1d(core::Context& ctx);
//LD E,d8
int opcode_0x1e(core::Context& ctx);
//RRA
int opcode_0x1f(core::Context& ctx);
//JR NZ,r8
int opcode_0x20(core::Context& ctx);
//LD HL,d16
int opcode_0x21(core::Context& ctx);
//LD (HL+),A
int opcode_0x22(core::Context& ctx);
//INC HL
int opcode_0x23(core::Context& ctx);
//INC H
int opcode_0x24(core::Context& ctx);
//DEC H
int opcode_0x25(core::Context& ctx);
//LD H,d8
int opcode_0x26(core::Context& ctx);
//DAA
int opcode_0x27(core::Context& ctx);
//JR Z,r8
int opcode_0x28(core::Context& ctx);
//ADD HL,HL
int opcode_0x29(core::Context& ctx);
//LD A,(HL+)
int opcode_0x2a(core::Context& ctx);
//DEC HL
int opcode_0x2b(core::Context& ctx);
//INC L
int opcode_0x2c(core::Context& ctx);
//DEC L
int opcode_0x2d(core::Context& ctx);
//LD L,d8
int opcode_0x2e(core::Context& ctx);
//CPL
int opcode_0x2f(core::Context& ctx);
//JR NC,r8
int opcode_0x30(core::Context& ctx);
//LD SP,d16
int opcode_0x31(core::Context& ctx);
//LD (HL-),A
int opcode_0x32(core::Context& ctx);
//INC SP
int opcode_0x33(core::Context& ctx);
//INC (HL)
int opcode_0x34(core::Context& ctx);
//DEC (HL)
int opcode_0x35(core::Context& ctx);
//LD (HL),d8
int opcode_0x36(core::Context& ctx);
//SCF
int opcode_0x37(core::Context& ctx);
//JR C,r8
int opcode_0x38(core::Context& ctx);
//ADD HL,SP
int opcode_0x39(core::Context& ctx);
//LD A,(HL-)
int opcode_0x3a(core::Context& ctx);
//DEC SP
int opcode_0x3b(core::Context& ctx);
//INC A
int opcode_0x3c(core::Context& ctx);
//DEC A
int opcode_0x3d(core::Context& ctx);
//LD A,d8
int opcode_0x3e(core::Context& ctx);
//CCF
int opcode_0x3f(core::Context& ctx);
//LD B,B
int opcode_0x40(core::Context& ctx);
//LD B,C
int opcode_0x41(core::Context& ctx);
//LD B,D
int opcode_0x42(core::Context& ctx);
//LD B,E
int opcode_0x43(core::Context& ctx);
//LD B,H
int opcode_0x44(core::Context& ctx);
//LD B,L
int opcode_0x45(core::Context& ctx);
//LD B,(HL)
int opcode_0x46(core::Context& ctx);
//LD B,A
int opcode_0x47(core::Context& ctx);
//LD C,B
int opcode_0x48(core::Context& ctx);
//LD C,C
int opcode_0x49(core::Context& ctx);
//LD C,D
int opcode_0x4a(core::Context& ctx);
//LD C,E
int opcode_0x4b(core::Context& ctx);
//LD C,H
int opcode_0x4c(core::Context& ctx);
//LD C,L
int opcode_0x4d(core::Context& ctx);
//LD C,(HL)
int opcode_0x4e(core::Context& ctx);
//LD C,A
int opcode_0x4f(core::Context& ctx);
//LD D,B
int opcode_0x50(core::Context& ctx);
//LD D,C
int opcode_0x51(core::Context& ctx);
//LD D,D
int opcode_0x52(core::Context& ctx);
//LD D,E
int opcode_0x53(core::Context& ctx);
//LD D,H
int opcode_0x54(core::Context& ctx);
//LD D,L
int opcode_0x55(core::Context& ctx);
//LD D,(HL)
int opcode_0x56(core::Context& ctx);
//LD D,A
int opcode_0x57(core::Context& ctx);
//LD E,B
int opcode_0x58(core::Context& ctx);
//LD E,C
int opcode_0x59(core::Context& ctx);
//LD E,D
int opcode_0x5a(core::Context& ctx);
//LD E,E
int opcode_0x5b(core::Context& ctx);
//LD E,H
int opcode_0x5c(core::Context& ctx);
//LD E,L
int opcode_0x5d(core::Context& ctx);
//LD E,(HL)
int opcode_0x5e(core::Context& ctx);
//LD E,A
int opcode_0x5f(core::Context& ctx);
//LD H,B
int opcode_0x60(core::Context& ctx);
//LD H,C
int opcode_0x61(core::Context& ctx);
//LD H,D
int opcode_0x62(core::Context& ctx);
//LD H,E
int opcode_0x63(core::Context& ctx);
//LD H,H
int opcode_0x64(core::Context& ctx);
//LD H,L
int opcode_0x65(core::Context& ctx);
//LD H,(HL)
int opcode_0x66(core::Context& ctx);
//LD H,A
int opcode_0x67(core::Context& ctx);
//LD L,B
int opcode_0x68(core::Context& ctx);
//LD L,C
int opcode_0x69(core::Context& ctx);
//LD L,D
int opcode_0x6a(core::Context& ctx);
//LD L,E
int opcode_0x6b(core::Context& ctx);
//LD L,H
int opcode_0x6c(core::Context& ctx);
//LD L,L
int opcode_0x6d(core::Context& ctx);
//LD L,(HL)
int opcode_0x6e(core::Context& ctx);
//LD L,A
int opcode_0x6f(core::Context& ctx);
//LD (HL),B
int opcode_0x70(core::Context& ctx);
//LD (HL),C
int opcode_0x71(core::Context& ctx);
//LD (HL),D
int opcode_0x72(core::Context& ctx);
//LD (HL),E
int opcode_0x73(core::Context& ctx);
//LD (HL),H
int opcode_0x74(core::Context& ctx);
//LD (HL),L
int opcode_0x75(core::Context& ctx);
//HALT
int opcode_0x76(core::Context& ctx);
//LD (HL),A
int opcode_0x77(core::Context& ctx);
//LD A,B
int opcode_0x78(core::Context& ctx);
//LD A,C
int opcode_0x79(core::Context& ctx);
//LD A,D
int opcode_0x7a(core::Context& ctx);
//LD A,E
int opcode_0x7b(core::Context& ctx);
//LD A,H
int opcode_0x7c(core::Context& ctx);
//LD A,L
int opcode_0x7d(core::Context& ctx);
//LD A,(HL)
int opcode_0x7e(core::Context& ctx);
//LD A,A
int opcode_0x7f(core::Context& ctx);
//ADD A,B
int opcode_0x80(core::Context& ctx);
//ADD A,C
int opcode_0x81(core::Context& ctx);
//ADD A,D
int opcode_0x82(core::Context& ctx);
//ADD A,E
int opcode_0x83(core::Context& ctx);
//ADD A,H
int opcode_0x84(core::Context& ctx);
//ADD A,L
int opcode_0x85(core::Context& ctx);
//ADD A,(HL)
int opcode_0x86(core::Context& ctx);
//ADD A,A
int opcode_0x87(core::Context& ctx);
//ADC A,B
int opcode_0x88(core::Context& ctx);
//ADC A,C
int opcode_0x89(core::Context& ctx);
//ADC A,D
int opcode_0x8a(core::Context& ctx);
//ADC A,E
int opcode_0x8b(core::Context& ctx);
//ADC A,H
int opcode_0x8c(core::Context& ctx);
//ADC A,L
int opcode_0x8d(core::Context& ctx);
//ADC A,(HL)
int opcode_0x8e(core::Context& ctx);
//ADC A,A
int opcode_0x8f(core::Context& ctx);
//SUB B
int opcode_0x90(core::Context& ctx);
//SUB C
int opcode_0x91(core::Context& ctx);
//SUB D
int opcode_0x92(core::Context& ctx);
//SUB E
int opcode_0x93(core::Context& ctx);
//SUB H
int opcode_0x94(core::Context& ctx);
//SUB L
int opcode_0x95(core::Context& ctx);
//SUB (HL)
int opcode_0x96(core::Context& ctx);
//SUB A
int opcode_0x97(core::Context& ctx);
//SBC A,B
int opcode_0x98(core::Context& ctx);
//SBC A,C
int opcode_0x99(core::Context& ctx);
//SBC A,D
int opcode_0x9a(core::Context& ctx);
//SBC A,E
int opcode_0x9b(core::Context& ctx);
//SBC A,H
int opcode_0x9c(core::Context& ctx);
//SBC A,L
int opcode_0x9d(core::Context& ctx);
//SBC A,(HL)
int opcode_0x9e(core::Context& ctx);
//SBC A,A
int opcode_0x9f(core::Context& ctx);
//AND B
int opcode_0xa0(core::Context& ctx);
//AND C
int opcode_0xa1(core::Context& ctx);
//AND D
int opcode_0xa2(core::Context& ctx);
//AND E
int opcode_0xa3(core::Context& ctx);
//AND H
int opcode_0xa4(core::Context& ctx);
//AND L
int opcode_0xa5(core::Context& ctx);
//AND (HL)
int opcode_0xa6(core::Context& ctx);
//AND A
int opcode_0xa7(core::Context& ctx);
//XOR B
int opcode_0xa8(core::Context& ctx);
//XOR C
int opcode_0xa9(core::Context& ctx);
//XOR D
int opcode_0xaa(core::Context& ctx);
//XOR E
int opcode_0xab(core::Context& ctx);
//XOR H
int opcode_0xac(core::Context& ctx);
//XOR L
int opcode_0xad(core::Context& ctx);
//XOR (HL)
int opcode_0xae(core::Context& ctx);
//XOR A
int opcode_0xaf(core::Context& ctx);
//OR B
int opcode_0xb0(core::Context& ctx);
//OR C
int opcode_0xb1(core::Context& ctx);
//OR D
int opcode_0xb2(core::Context& ctx);
//OR E
int opcode_0xb3(core::Context& ctx);
//OR H
int opcode_0xb4(core::Context& ctx);
//OR L
int opcode_0xb5(core::Context& ctx);
//OR (HL)
int opcode_0xb6(core::Context& ctx);
//OR A
int opcode_0xb7(core::Context& ctx);
//CP B
int opcode_0xb8(core::Context& ctx);
//CP C
int opcode_0xb9(core::Context& ctx);
//CP D
int opcode_0xba(core::Context& ctx);
//CP E
int opcode_0xbb(core::Context& ctx);
//CP H
int opcode_0xbc(core::Context& ctx);
//CP L
int opcode_0xbd(core::Context& ctx);
//CP (HL)
int opcode_0xbe(core::Context& ctx);
//CP A
int opcode_0xbf(core::Context& ctx);
//RET NZ
int opcode_0xc0(core::Context& ctx);
//POP BC
int opcode_0xc1(core::Context& ctx);
//JP NZ,a16
int opcode_0xc2(core::Context& ctx);
//JP a16
int opcode_0xc3(core::Context& ctx);
//CALL NZ,a16
int opcode_0xc4(core::Context& ctx);
//PUSH BC
int opcode_0xc5(core::Context& ctx);
//ADD A,d8
int opcode_0xc6(core::Context& ctx);
//RST 00H
int opcode_0xc7(core::Context& ctx);
//RET Z
int opcode_0xc8(core::Context& ctx);
//RET
int opcode_0xc9(core::Context& ctx);
//JP Z,a16
int opcode_0xca(core::Context& ctx);
//CALL Z,a16
int opcode_0xcc(core::Context& ctx);
//CALL a16
int opcode_0xcd(core::Context& ctx);
//ADC A,d8
int opcode_0xce(core::Context& ctx);
//RST 08H
int opcode_0xcf(core::Context& ctx);
//RET NC
int opcode_0xd0(core::Context& ctx);
//POP DE
int opcode_0xd1(core::Context& ctx);
//JP NC,a16
int opcode_0xd2(core::Context& ctx);
int opcode_0xd3(core::Context& ctx);
//CALL NC,a16
int opcode_0xd4(core::Context& ctx);
//PUSH DE
int opcode_0xd5(core::Context& ctx);
//SUB d8
int opcode_0xd6(core::Context& ctx);
//RST 10H
int opcode_0xd7(core::Context& ctx);
//RET C
int opcode_0xd8(core::Context& ctx);
//RETI
int opcode_0xd9(core::Context& ctx);
//JP C,a16
int opcode_0xda(core::Context& ctx);
int opcode_0xdb(core::Context& ctx);
//CALL C,a16
int opcode_0xdc(core::Context& ctx);
int opcode_0xdd(core::Context& ctx);
//SBC A,d8
int opcode_0xde(core::Context& ctx);
//RST 18H
int opcode_0xdf(core::Context& ctx);
//LDH (a8),A
int opcode_0xe0(core::Context& ctx);
//POP HL
int opcode_0xe1(core::Context& ctx);
//LD (C),A
int opcode_0xe2(core::Context& ctx);
int opcode_0xe3(core::Context& ctx);
int opcode_0xe4(core::Context& ctx);
//PUSH HL
int opcode_0xe5(core::Context& ctx);
//AND d8
int opcode_0xe6(core::Context& ctx);
//RST 20H
int opcode_0xe7(core::Context& ctx);
//ADD SP,r8
int opcode_0xe8(core::Context& ctx);
//JP (HL)
int opcode_0xe9(core::Context& ctx);
//LD (a16),A
int opcode_0xea(core::Context& ctx);
int opcode_0xeb(core::Context& ctx);
int opcode_0xec(core::Context& ctx);
int opcode_0xed(core::Context& ctx);
//XOR d8
int opcode_0xee(core::Context& ctx);
//RST 28H
int opcode_0xef(core::Context& ctx);
//LDH A,(a8)
int opcode_0xf0(core::Context& ctx);
//POP AF
int opcode_0xf1(core::Context& ctx);
//LD A,(C)
int opcode_0xf2(core::Context& ctx);
//DI
int opcode_0xf3(core::Context& ctx);
int opcode_0xf4(core::Context& ctx);
//PUSH AF
int opcode_0xf5(core::Context& ctx);
//OR d8
int opcode_0xf6(core::Context& ctx);
//RST 30H
int opcode_0xf7(core::Context& ctx);
//LD HL,SP+r8
int opcode_0xf8(core::Context& ctx);
//LD SP,HL
int opcode_0xf9(core::Context& ctx);
//LD A,(a16)
int opcode_0xfa(core::Context& ctx);
//EI
int opcode_0xfb(core::Context& ctx);
int opcode_0xfc(core::Context& ctx);
int opcode_0xfd(core::Context& ctx);
//CP d8
int opcode_0xfe(core::Context& ctx);
//RST 38H
int opcode_0xff(core::Context& ctx);
//RLC B
int opcode_0xcb00(core::Context& ctx);
//RLC C
int opcode_0xcb01(core::Context& ctx);
//RLC D
int opcode_0xcb02(core::Context& ctx);
//RLC E
int opcode_0xcb03(core::Context& ctx);
//RLC H
int opcode_0xcb04(core::Context& ctx);
//RLC L
int opcode_0xcb05(core::Context& ctx);
//RLC (HL)
int opcode_0xcb06(core::Context& ctx);
//RLC A
int opcode_0xcb07(core::Context& ctx);
//RRC B
int opcode_0xcb08(core::Context& ctx);
//RRC C
int opcode_0xcb09(core::Context& ctx);
//RRC D
int opcode_0xcb0a(core::Context& ctx);
//RRC E
int opcode_0xcb0b(core::Context& ctx);
//RRC H
int opcode_0xcb0c(core::Context& ctx);
//RRC L
int opcode_0xcb0d(core::Context& ctx);
//RRC (HL)
int opcode_0xcb0e(core::Context& ctx);
//RRC A
int opcode_0xcb0f(core::Context& ctx);
//RL B
int opcode_0xcb10(core::Context& ctx);
//RL C
int opcode_0xcb11(core::Context& ctx);
//RL D
int opcode_0xcb12(core::Context& ctx);
//RL E
int opcode_0xcb13(core::Context& ctx);
//RL H
int opcode_0xcb14(core::Context& ctx);
//RL L
int opcode_0xcb15(core::Context& ctx);
//RL (HL)
int opcode_0xcb16(core::Context& ctx);
//RL A
int opcode_0xcb17(core::Context& ctx);
//RR B
int opcode_0xcb18(core::Context& ctx);
//RR C
int opcode_0xcb19(core::Context& ctx);
//RR D
int opcode_0xcb1a(core::Context& ctx);
//RR E
int opcode_0xcb1b(core::Context& ctx);
//RR H
int opcode_0xcb1c(core::Context& ctx);
//RR L
int opcode_0xcb1d(core::Context& ctx);
//RR (HL)
int opcode_0xcb1e(core::Context& ctx);
//RR A
int opcode_0xcb1f(core::Context& ctx);
//SLA B
int opcode_0xcb20(core::Context& ctx);
//SLA C
int opcode_0xcb21(core::Context& ctx);
//SLA D
int opcode_0xcb22(core::Context& ctx);
//SLA E
int opcode_0xcb23(core::Context& ctx);
//SLA H
int opcode_0xcb24(core::Context& ctx);
//SLA L
int opcode_0xcb25(core::Context& ctx);
//SLA (HL)
int opcode_0xcb26(core::Context& ctx);
//SLA A
int opcode_0xcb27(core::Context& ctx);
//SRA B
int opcode_0xcb28(core::Context& ctx);
//SRA C
int opcode_0xcb29(core::Context& ctx);
//SRA D
int opcode_0xcb2a(core::Context& ctx);
//SRA E
int opcode_0xcb2b(core::Context& ctx);
//SRA H
int opcode_0xcb2c(core::Context& ctx);
//SRA L
int opcode_0xcb2d(core::Context& ctx);
//SRA (HL)
int opcode_0xcb2e(core::Context& ctx);
//SRA A
int opcode_0xcb2f(core::Context& ctx);
//SWAP B
int opcode_0xcb30(core::Context& ctx);
//SWAP C
int opcode_0xcb31(core::Context& ctx);
//SWAP D
int opcode_0xcb32(core::Context& ctx);
//SWAP E
int opcode_0xcb33(core::Context& ctx);
//SWAP H
int opcode_0xcb34(core::Context& ctx);
//SWAP L
int opcode_0xcb35(core::Context& ctx);
//SWAP (HL)
int opcode_0xcb36(core::Context& ctx);
//SWAP A
int opcode_0xcb37(core::Context& ctx);
//SRL B
int opcode_0xcb38(core::Context& ctx);
//SRL C
int opcode_0xcb39(core::Context& ctx);
//SRL D
int opcode_0xcb3a(core::Context& ctx);
//SRL E
int opcode_0xcb3b(core::Context& ctx);
//SRL H
int opcode_0xcb3c(core::Context& ctx);
//SRL L
int opcode_0xcb3d(core::Context& ctx);
//SRL (HL)
int opcode_0xcb3e(core::Context& ctx);
//SRL A
int opcode_0xcb3f(core::Context& ctx);
//BIT 0,B
int opcode_0xcb40(core::Context& ctx);
//BIT 0,C
int opcode_0xcb41(core::Context& ctx);
//BIT 0,D
int opcode_0xcb42(core::Context& ctx);
//BIT 0,E
int opcode_0xcb43(core::Context& ctx);
//BIT 0,H
int opcode_0xcb44(core::Context& ctx);
//BIT 0,L
int opcode_0xcb45(core::Context& ctx);
//BIT 0,(HL)
int opcode_0xcb46(core::Context& ctx);
//BIT 0,A
int opcode_0xcb47(core::Context& ctx);
//BIT 1,B
int opcode_0xcb48(core::Context& ctx);
//BIT 1,C
int opcode_0xcb49(core::Context& ctx);
//BIT 1,D
int opcode_0xcb4a(core::Context& ctx);
//BIT 1,E
int opcode_0xcb4b(core::Context& ctx);
//BIT 1,H
int opcode_0xcb4c(core::Context& ctx);
//BIT 1,L
int opcode_0xcb4d(core::Context& ctx);
//BIT 1,(HL)
int opcode_0xcb4e(core::Context& ctx);
//BIT 1,A
int opcode_0xcb4f(core::Context& ctx);
//BIT 2,B
int opcode_0xcb50(core::Context& ctx);
//BIT 2,C
int opcode_0xcb51(core::Context& ctx);
//BIT 2,D
int opcode_0xcb52(core::Context& ctx);
//BIT 2,E
int opcode_0xcb53(core::Context& ctx);
//BIT 2,H
int opcode_0xcb54(core::Context& ctx);
//BIT 2,L
int opcode_0xcb55(core::Context& ctx);
//BIT 2,(HL)
int opcode_0xcb56(core::Context& ctx);
//BIT 2,A
int opcode_0xcb57(core::Context& ctx);
//BIT 3,B
int opcode_0xcb58(core::Context& ctx);
//BIT 3,C
int opcode_0xcb59(core::Context& ctx);
//BIT 3,D
int opcode_0xcb5a(core::Context& ctx);
//BIT 3,E
int opcode_0xcb5b(core::Context& ctx);
//BIT 3,H
int opcode_0xcb5c(core::Context& ctx);
//BIT 3,L
int opcode_0xcb5d(core::Context& ctx);
//BIT 3,(HL)
int opcode_0xcb5e(core::Context& ctx);
//BIT 3,A
int opcode_0xcb5f(core::Context& ctx);
//BIT 4,B
int opcode_0xcb60(core::Context& ctx);
//BIT 4,C
int opcode_0xcb61(core::Context& ctx);
//BIT 4,D
int opcode_0xcb62(core::Context& ctx);
//BIT 4,E
int opcode_0xcb63(core::Context& ctx);
//BIT 4,H
int opcode_0xcb64(core::Context& ctx);
//BIT 4,L
int opcode_0xcb65(core::Context& ctx);
//BIT 4,(HL)
int opcode_0xcb66(core::Context& ctx);
//BIT 4,A
int opcode_0xcb67(core::Context& ctx);
//BIT 5,B
int opcode_0xcb68(core::Context& ctx);
//BIT 5,C
int opcode_0xcb69(core::Context& ctx);
//BIT 5,D
int opcode_0xcb6a(core::Context& ctx);
//BIT 5,E
int opcode_0xcb6b(core::Context& ctx);
//BIT 5,H
int opcode_0xcb6c(core::Context& ctx);
//BIT 5,L
int opcode_0xcb6d(core::Context& ctx);
//BIT 5,(HL)
int opcode_0xcb6e(core::Context& ctx);
//BIT 5,A
int opcode_0xcb6f(core::Context& ctx);
//BIT 6,B
int opcode_0xcb70(core::Context& ctx);
//BIT 6,C
int opcode_0xcb71(core::Context& ctx);
//BIT 6,D
int opcode_0xcb72(core::Context& ctx);
//BIT 6,E
int opcode_0xcb73(core::Context& ctx);
//BIT 6,H
int opcode_0xcb74(core::Context& ctx);
//BIT 6,L
int opcode_0xcb75(core::Context& ctx);
//BIT 6,(HL)
int opcode_0xcb76(core::Context& ctx);
//BIT 6,A
int opcode_0xcb77(core::Context& ctx);
//BIT 7,B
int opcode_0xcb78(core::Context& ctx);
//BIT 7,C
int opcode_0xcb79(core::Context& ctx);
//BIT 7,D
int opcode_0xcb7a(core::Context& ctx);
//BIT 7,E
int opcode_0xcb7b(core::Context& ctx);
//BIT 7,H
int opcode_0xcb7c(core::Context& ctx);
//BIT 7,L
int opcode_0xcb7d(core::Context& ctx);
//BIT 7,(HL)
int opcode_0xcb7e(core::Context& ctx);
//BIT 7,A
int opcode_0xcb7f(core::Context& ctx);
//RES 0,B
int opcode_0xcb80(core::Context& ctx);
//RES 0,C
int opcode_0xcb81(core::Context& ctx);
//RES 0,D
int opcode_0xcb82(core::Context& ctx);
//RES 0,E
int opcode_0xcb83(core::Context& ctx);
//RES 0,H
int opcode_0xcb84(core::Context& ctx);
//RES 0,L
int opcode_0xcb85(core::Context& ctx);
//RES 0,(HL)
int opcode_0xcb86(core::Context& ctx);
//RES 0,A
int opcode_0xcb87(core::Context& ctx);
//RES 1,B
int opcode_0xcb88(core::Context& ctx);
//RES 1,C
int opcode_0xcb89(core::Context& ctx);
//RES 1,D
int opcode_0xcb8a(core::Context& ctx);
//RES 1,E
int opcode_0xcb8b(core::Context& ctx);
//RES 1,H
int opcode_0xcb8c(core::Context& ctx);
//RES 1,L
int opcode_0xcb8d(core::Context& ctx);
//RES 1,(HL)
int opcode_0xcb8e(core::Context& ctx);
//RES 1,A
int opcode_0xcb8f(core::Context& ctx);
//RES 2,B
int opcode_0xcb90(core::Context& ctx);
//RES 2,C
int opcode_0xcb91(core::Context& ctx);
//RES 2,D
int opcode_0xcb92(core::Context& ctx);
//RES 2,E
int opcode_0xcb93(core::Context& ctx);
//RES 2,H
int opcode_0xcb94(core::Context& ctx);
//RES 2,L
int opcode_0xcb95(core::Context& ctx);
//RES 2,(HL)
int opcode_0xcb96(core::Context& ctx);
//RES 2,A
int opcode_0xcb97(core::Context& ctx);
//RES 3,B
int opcode_0xcb98(core::Context& ctx);
//RES 3,C
int opcode_0xcb99(core::Context& ctx);
//RES 3,D
int opcode_0xcb9a(core::Context& ctx);
//RES 3,E
int opcode_0xcb9b(core::Context& ctx);
//RES 3,H
int opcode_0xcb9c(core::Context& ctx);
//RES 3,L
int opcode_0xcb9d(core::Context& ctx);
//RES 3,(HL)
int opcode_0xcb9e(core::Context& ctx);
//RES 3,A
int opcode_0xcb9f(core::Context& ctx);
//RES 4,B
int opcode_0xcba0(core::Context& ctx);
//RES 4,C
int opcode_0xcba1(core::Context& ctx);
//RES 4,D
int opcode_0xcba2(core::Context& ctx);
//RES 4,E
int opcode_0xcba3(core::Context& ctx);
//RES 4,H
int opcode_0xcba4(core::Context& ctx);
//RES 4,L
int opcode_0xcba5(core::Context& ctx);
//RES 4,(HL)
int opcode_0xcba6(core::Context& ctx);
//RES 4,A
int opcode_0xcba7(core::Context& ctx);
//RES 5,B
int opcode_0xcba8(core::Context& ctx);
//RES 5,C
int opcode_0xcba9(core::Context& ctx);
//RES 5,D
int opcode_0xcbaa(core::Context& ctx);
//RES 5,E
int opcode_0xcbab(core::Context& ctx);
//RES 5,H
int opcode_0xcbac(core::Context& ctx);
//RES 5,L
int opcode_0xcbad(core::Context& ctx);
//RES 5,(HL)
int opcode_0xcbae(core::Context& ctx);
//RES 5,A
int opcode_0xcbaf(core::Context& ctx);
//RES 6,B
int opcode_0xcbb0(core::Context& ctx);
//RES 6,C
int opcode_0xcbb1(core::Context& ctx);
//RES 6,D
int opcode_0xcbb2(core::Context& ctx);
//RES 6,E
int opcode_0xcbb3(core::Context& ctx);
//RES 6,H
int opcode_0xcbb4(core::Context& ctx);
//RES 6,L
int opcode_0xcbb5(core::Context& ctx);
//RES 6,(HL)
int opcode_0xcbb6(core::Context& ctx);
//RES 6,A
int opcode_0xcbb7(core::Context& ctx);
//RES 7,B
int opcode_0xcbb8(core::Context& ctx);
//RES 7,C
int opcode_0xcbb9(core::Context& ctx);
//RES 7,D
int opcode_0xcbba(core::Context& ctx);
//RES 7,E
int opcode_0xcbbb(core::Context& ctx);
//RES 7,H
int opcode_0xcbbc(core::Context& ctx);
//RES 7,L
int opcode_0xcbbd(core::Context& ctx);
//RES 7,(HL)
int opcode_0xcbbe(core::Context& ctx);
//RES 7,A
int opcode_0xcbbf(core::Context& ctx);
//SET 0,B
int opcode_0xcbc0(core::Context& ctx);
//SET 0,C
int opcode_0xcbc1(core::Context& ctx);
//SET 0,D
int opcode_0xcbc2(core::Context& ctx);
//SET 0,E
int opcode_0xcbc3(core::Context& ctx);
//SET 0,H
int opcode_0xcbc4(core::Context& ctx);
//SET 0,L
int opcode_0xcbc5(core::Context& ctx);
//SET 0,(HL)
int opcode_0xcbc6(core::Context& ctx);
//SET 0,A
int opcode_0xcbc7(core::Context& ctx);
//SET 1,B
int opcode_0xcbc8(core::Context& ctx);
//SET 1,C
int opcode_0xcbc9(core::Context& ctx);
//SET 1,D
int opcode_0xcbca(core::Context& ctx);
//SET 1,E
int opcode_0xcbcb(core::Context& ctx);
//SET 1,H
int opcode_0xcbcc(core::Context& ctx);
//SET 1,L
int opcode_0xcbcd(core::Context& ctx);
//SET 1,(HL)
int opcode_0xcbce(core::Context& ctx);
//SET 1,A
int opcode_0xcbcf(core::Context& ctx);
//SET 2,B
int opcode_0xcbd0(core::Context& ctx);
//SET 2,C
int opcode_0xcbd1(core::Context& ctx);
//SET 2,D
int opcode_0xcbd2(core::Context& ctx);
//SET 2,E
int opcode_0xcbd3(core::Context& ctx);
//SET 2,H
int opcode_0xcbd4(core::Context& ctx);
//SET 2,L
int opcode_0xcbd5(core::Context& ctx);
//SET 2,(HL)
int opcode_0xcbd6(core::Context& ctx);
//SET 2,A
int opcode_0xcbd7(core::Context& ctx);
//SET 3,B
int opcode_0xcbd8(core::Context& ctx);
//SET 3,C
int opcode_0xcbd9(core::Context& ctx);
//SET 3,D
int opcode_0xcbda(core::Context& ctx);
//SET 3,E
int opcode_0xcbdb(core::Context& ctx);
//SET 3,H
int opcode_0xcbdc(core::Context& ctx);
//SET 3,L
int opcode_0xcbdd(core::Context& ctx);
//SET 3,(HL)
int opcode_0xcbde(core::Context& ctx);
//SET 3,A
int opcode_0xcbdf(core::Context& ctx);
//SET 4,B
int opcode_0xcbe0(core::Context& ctx);
//SET 4,C
int opcode_0xcbe1(core::Context& ctx);
//SET 4,D
int opcode_0xcbe2(core::Context& ctx);
//SET 4,E
int opcode_0xcbe3(core::Context& ctx);
//SET 4,H
int opcode_0xcbe4(core::Context& ctx);
//SET 4,L
int opcode_0xcbe5(core::Context& ctx);
//SET 4,(HL)
int opcode_0xcbe6(core::Context& ctx);
//SET 4,A
int opcode_0xcbe7(core::Context& ctx);
//SET 5,B
int opcode_0xcbe8(core::Context& ctx);
//SET 5,C
int opcode_0xcbe9(core::Context& ctx);
//SET 5,D
int opcode_0xcbea(core::Context& ctx);
//SET 5,E
int opcode_0xcbeb(core::Context& ctx);
//SET 5,H
int opcode_0xcbec(core::Context& ctx);
//SET 5,L
int opcode_0xcbed(core::Context& ctx);
//SET 5,(HL)
int opcode_0xcbee(core::Context& ctx);
//SET 5,A
int opcode_0xcbef(core::Context& ctx);
//SET 6,B
int opcode_0xcbf0(core::Context& ctx);
//SET 6,C
int opcode_0xcbf1(core::Context& ctx);
//SET 6,D
int opcode_0xcbf2(core::Context& ctx);
//SET 6,E
int opcode_0xcbf3(core::Context& ctx);
//SET 6,H
int opcode_0xcbf4(core::Context& ctx);
//SET 6,L
int opcode_0xcbf5(core::Context& ctx);
//SET 6,(HL)
int opcode_0xcbf6(core::Context& ctx);
//SET 6,A
int opcode_0xcbf7(core::Context& ctx);
//SET 7,B
int opcode_0xcbf8(core::Context& ctx);
//SET 7,C
int opcode_0xcbf9(core::Context& ctx);
//SET 7,D
int opcode_0xcbfa(core::Context& ctx);
//SET 7,E
int opcode_0xcbfb(core::Context& ctx);
//SET 7,H
int opcode_0xcbfc(core::Context& ctx);
//SET 7,L
int opcode_0xcbfd(core::Context& ctx);
//SET 7,(HL)
int opcode_0xcbfe(core::Context& ctx);
//SET 7,A
int opcode_0xcbff(core::Context& ctx);

#endif