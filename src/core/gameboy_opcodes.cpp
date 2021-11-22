#include "core/gameboy_opcodes.hpp"
#include "core/gameboy.hpp"

#define OP_INLINE __attribute__((always_inline))

void OP_INLINE LD(Registers& r, RAM& m, auto& first, auto second) { first = second; }
void OP_INLINE ADD(Registers& r, RAM& m, auto& first, auto second) {}
void OP_INLINE SUB(Registers& r, RAM& m, auto first) {}
void OP_INLINE ADC(Registers& r, RAM& m, auto& first, auto second) {}
void OP_INLINE SBC(Registers& r, RAM& m, auto& first, auto second) {}
void OP_INLINE XOR(Registers& r, RAM& m, auto first) {
    r.A ^= first;
    if (r.A == 0) {
        r.ZERO_FLAG = 1;
    }
}
void OP_INLINE OR(Registers& r, RAM& m, auto first) {
    r.A |= first;
    if (r.A == 0) {
        r.ZERO_FLAG = 1;
    }
}
void OP_INLINE INC(Registers& r, RAM& m, auto& first) {}
void OP_INLINE DEC(Registers& r, RAM& m, auto& first) {}
void OP_INLINE RLC(Registers& r, RAM& m, auto& first) {}
void OP_INLINE RRC(Registers& r, RAM& m, auto& first) {}
void OP_INLINE RL(Registers& r, RAM& m, auto& first) {}
void OP_INLINE RR(Registers& r, RAM& m, auto& first) {}
void OP_INLINE SLA(Registers& r, RAM& m, auto& first) {}
void OP_INLINE SRA(Registers& r, RAM& m, auto& first) {}
void OP_INLINE SWAP(Registers& r, RAM& m, auto& first) {}
void OP_INLINE SRL(Registers& r, RAM& m, auto& first) {}
void OP_INLINE BIT(Registers& r, RAM& m, auto first, auto second) {}
void OP_INLINE RES(Registers& r, RAM& m, auto first, auto second) {}
void OP_INLINE SET(Registers& r, RAM& m, auto first, auto second) {}
void OP_INLINE RST(Registers& r, RAM& m, auto first) {}
void OP_INLINE RLCA(Registers& r, RAM& m) {}
void OP_INLINE RRCA(Registers& r, RAM& m) {}
void OP_INLINE RLA(Registers& r, RAM& m) {}
void OP_INLINE RRA(Registers& r, RAM& m) {}
void OP_INLINE DAA(Registers& r, RAM& m) {}
void OP_INLINE SCF(Registers& r, RAM& m) {}
void OP_INLINE CPL(Registers& r, RAM& m) {}
void OP_INLINE CCF(Registers& r, RAM& m) {}
void OP_INLINE PUSH(Registers& r, RAM& m, auto first) {}
void OP_INLINE POP(Registers& r, RAM& m, auto first) {}
void OP_INLINE CP(Registers& r, RAM& m, auto first) {}
void OP_INLINE EI(Registers& r, RAM& m) {}
void OP_INLINE RETI(Registers& r, RAM& m) {}
void OP_INLINE DI(Registers& r, RAM& m) {}
void OP_INLINE STOP(Registers& r, RAM& m) {}
void OP_INLINE RET(Registers& r, RAM& m) {}
int OP_INLINE  RET(Registers& r, RAM& m, auto bit, auto T, auto F) {}
void OP_INLINE JR(Registers& r, RAM& m, auto first) {}
int OP_INLINE  JR(Registers& r, RAM& m, auto first, auto bit, auto T, auto F) {}
void OP_INLINE JP(Registers& r, RAM& m, auto first) {}
int OP_INLINE  JP(Registers& r, RAM& m, auto first, auto bit, auto T, auto F) {}
void OP_INLINE CALL(Registers& r, RAM& m, auto first) {}
int OP_INLINE  CALL(Registers& r, RAM& m, auto first, auto bit, auto T, auto F) {}

// create table
std::array<int (*)(Registers&, RAM&), 256> create_table() {
    std::array<int (*)(Registers&, RAM&), 256> ret;

    ret[0]   = opcode_0x00;
    ret[1]   = opcode_0x01;
    ret[2]   = opcode_0x02;
    ret[3]   = opcode_0x03;
    ret[4]   = opcode_0x04;
    ret[5]   = opcode_0x05;
    ret[6]   = opcode_0x06;
    ret[7]   = opcode_0x07;
    ret[8]   = opcode_0x08;
    ret[9]   = opcode_0x09;
    ret[10]  = opcode_0x0a;
    ret[11]  = opcode_0x0b;
    ret[12]  = opcode_0x0c;
    ret[13]  = opcode_0x0d;
    ret[14]  = opcode_0x0e;
    ret[15]  = opcode_0x0f;
    ret[16]  = opcode_0x10;
    ret[17]  = opcode_0x11;
    ret[18]  = opcode_0x12;
    ret[19]  = opcode_0x13;
    ret[20]  = opcode_0x14;
    ret[21]  = opcode_0x15;
    ret[22]  = opcode_0x16;
    ret[23]  = opcode_0x17;
    ret[24]  = opcode_0x18;
    ret[25]  = opcode_0x19;
    ret[26]  = opcode_0x1a;
    ret[27]  = opcode_0x1b;
    ret[28]  = opcode_0x1c;
    ret[29]  = opcode_0x1d;
    ret[30]  = opcode_0x1e;
    ret[31]  = opcode_0x1f;
    ret[32]  = opcode_0x20;
    ret[33]  = opcode_0x21;
    ret[34]  = opcode_0x22;
    ret[35]  = opcode_0x23;
    ret[36]  = opcode_0x24;
    ret[37]  = opcode_0x25;
    ret[38]  = opcode_0x26;
    ret[39]  = opcode_0x27;
    ret[40]  = opcode_0x28;
    ret[41]  = opcode_0x29;
    ret[42]  = opcode_0x2a;
    ret[43]  = opcode_0x2b;
    ret[44]  = opcode_0x2c;
    ret[45]  = opcode_0x2d;
    ret[46]  = opcode_0x2e;
    ret[47]  = opcode_0x2f;
    ret[48]  = opcode_0x30;
    ret[49]  = opcode_0x31;
    ret[50]  = opcode_0x32;
    ret[51]  = opcode_0x33;
    ret[52]  = opcode_0x34;
    ret[53]  = opcode_0x35;
    ret[54]  = opcode_0x36;
    ret[55]  = opcode_0x37;
    ret[56]  = opcode_0x38;
    ret[57]  = opcode_0x39;
    ret[58]  = opcode_0x3a;
    ret[59]  = opcode_0x3b;
    ret[60]  = opcode_0x3c;
    ret[61]  = opcode_0x3d;
    ret[62]  = opcode_0x3e;
    ret[63]  = opcode_0x3f;
    ret[64]  = opcode_0x40;
    ret[65]  = opcode_0x41;
    ret[66]  = opcode_0x42;
    ret[67]  = opcode_0x43;
    ret[68]  = opcode_0x44;
    ret[69]  = opcode_0x45;
    ret[70]  = opcode_0x46;
    ret[71]  = opcode_0x47;
    ret[72]  = opcode_0x48;
    ret[73]  = opcode_0x49;
    ret[74]  = opcode_0x4a;
    ret[75]  = opcode_0x4b;
    ret[76]  = opcode_0x4c;
    ret[77]  = opcode_0x4d;
    ret[78]  = opcode_0x4e;
    ret[79]  = opcode_0x4f;
    ret[80]  = opcode_0x50;
    ret[81]  = opcode_0x51;
    ret[82]  = opcode_0x52;
    ret[83]  = opcode_0x53;
    ret[84]  = opcode_0x54;
    ret[85]  = opcode_0x55;
    ret[86]  = opcode_0x56;
    ret[87]  = opcode_0x57;
    ret[88]  = opcode_0x58;
    ret[89]  = opcode_0x59;
    ret[90]  = opcode_0x5a;
    ret[91]  = opcode_0x5b;
    ret[92]  = opcode_0x5c;
    ret[93]  = opcode_0x5d;
    ret[94]  = opcode_0x5e;
    ret[95]  = opcode_0x5f;
    ret[96]  = opcode_0x60;
    ret[97]  = opcode_0x61;
    ret[98]  = opcode_0x62;
    ret[99]  = opcode_0x63;
    ret[100] = opcode_0x64;
    ret[101] = opcode_0x65;
    ret[102] = opcode_0x66;
    ret[103] = opcode_0x67;
    ret[104] = opcode_0x68;
    ret[105] = opcode_0x69;
    ret[106] = opcode_0x6a;
    ret[107] = opcode_0x6b;
    ret[108] = opcode_0x6c;
    ret[109] = opcode_0x6d;
    ret[110] = opcode_0x6e;
    ret[111] = opcode_0x6f;
    ret[112] = opcode_0x70;
    ret[113] = opcode_0x71;
    ret[114] = opcode_0x72;
    ret[115] = opcode_0x73;
    ret[116] = opcode_0x74;
    ret[117] = opcode_0x75;
    ret[118] = opcode_0x76;
    ret[119] = opcode_0x77;
    ret[120] = opcode_0x78;
    ret[121] = opcode_0x79;
    ret[122] = opcode_0x7a;
    ret[123] = opcode_0x7b;
    ret[124] = opcode_0x7c;
    ret[125] = opcode_0x7d;
    ret[126] = opcode_0x7e;
    ret[127] = opcode_0x7f;
    ret[128] = opcode_0x80;
    ret[129] = opcode_0x81;
    ret[130] = opcode_0x82;
    ret[131] = opcode_0x83;
    ret[132] = opcode_0x84;
    ret[133] = opcode_0x85;
    ret[134] = opcode_0x86;
    ret[135] = opcode_0x87;
    ret[136] = opcode_0x88;
    ret[137] = opcode_0x89;
    ret[138] = opcode_0x8a;
    ret[139] = opcode_0x8b;
    ret[140] = opcode_0x8c;
    ret[141] = opcode_0x8d;
    ret[142] = opcode_0x8e;
    ret[143] = opcode_0x8f;
    ret[144] = opcode_0x90;
    ret[145] = opcode_0x91;
    ret[146] = opcode_0x92;
    ret[147] = opcode_0x93;
    ret[148] = opcode_0x94;
    ret[149] = opcode_0x95;
    ret[150] = opcode_0x96;
    ret[151] = opcode_0x97;
    ret[152] = opcode_0x98;
    ret[153] = opcode_0x99;
    ret[154] = opcode_0x9a;
    ret[155] = opcode_0x9b;
    ret[156] = opcode_0x9c;
    ret[157] = opcode_0x9d;
    ret[158] = opcode_0x9e;
    ret[159] = opcode_0x9f;
    ret[160] = opcode_0xa0;
    ret[161] = opcode_0xa1;
    ret[162] = opcode_0xa2;
    ret[163] = opcode_0xa3;
    ret[164] = opcode_0xa4;
    ret[165] = opcode_0xa5;
    ret[166] = opcode_0xa6;
    ret[167] = opcode_0xa7;
    ret[168] = opcode_0xa8;
    ret[169] = opcode_0xa9;
    ret[170] = opcode_0xaa;
    ret[171] = opcode_0xab;
    ret[172] = opcode_0xac;
    ret[173] = opcode_0xad;
    ret[174] = opcode_0xae;
    ret[175] = opcode_0xaf;
    ret[176] = opcode_0xb0;
    ret[177] = opcode_0xb1;
    ret[178] = opcode_0xb2;
    ret[179] = opcode_0xb3;
    ret[180] = opcode_0xb4;
    ret[181] = opcode_0xb5;
    ret[182] = opcode_0xb6;
    ret[183] = opcode_0xb7;
    ret[184] = opcode_0xb8;
    ret[185] = opcode_0xb9;
    ret[186] = opcode_0xba;
    ret[187] = opcode_0xbb;
    ret[188] = opcode_0xbc;
    ret[189] = opcode_0xbd;
    ret[190] = opcode_0xbe;
    ret[191] = opcode_0xbf;
    ret[192] = opcode_0xc0;
    ret[193] = opcode_0xc1;
    ret[194] = opcode_0xc2;
    ret[195] = opcode_0xc3;
    ret[196] = opcode_0xc4;
    ret[197] = opcode_0xc5;
    ret[198] = opcode_0xc6;
    ret[199] = opcode_0xc7;
    ret[200] = opcode_0xc8;
    ret[201] = opcode_0xc9;
    ret[202] = opcode_0xca;
    ret[204] = opcode_0xcc;
    ret[205] = opcode_0xcd;
    ret[206] = opcode_0xce;
    ret[207] = opcode_0xcf;
    ret[208] = opcode_0xd0;
    ret[209] = opcode_0xd1;
    ret[210] = opcode_0xd2;
    ret[211] = opcode_0xd3;
    ret[212] = opcode_0xd4;
    ret[213] = opcode_0xd5;
    ret[214] = opcode_0xd6;
    ret[215] = opcode_0xd7;
    ret[216] = opcode_0xd8;
    ret[217] = opcode_0xd9;
    ret[218] = opcode_0xda;
    ret[219] = opcode_0xdb;
    ret[220] = opcode_0xdc;
    ret[221] = opcode_0xdd;
    ret[222] = opcode_0xde;
    ret[223] = opcode_0xdf;
    ret[224] = opcode_0xe0;
    ret[225] = opcode_0xe1;
    ret[226] = opcode_0xe2;
    ret[227] = opcode_0xe3;
    ret[228] = opcode_0xe4;
    ret[229] = opcode_0xe5;
    ret[230] = opcode_0xe6;
    ret[231] = opcode_0xe7;
    ret[232] = opcode_0xe8;
    ret[233] = opcode_0xe9;
    ret[234] = opcode_0xea;
    ret[235] = opcode_0xeb;
    ret[236] = opcode_0xec;
    ret[237] = opcode_0xed;
    ret[238] = opcode_0xee;
    ret[239] = opcode_0xef;
    ret[240] = opcode_0xf0;
    ret[241] = opcode_0xf1;
    ret[242] = opcode_0xf2;
    ret[243] = opcode_0xf3;
    ret[244] = opcode_0xf4;
    ret[245] = opcode_0xf5;
    ret[246] = opcode_0xf6;
    ret[247] = opcode_0xf7;
    ret[248] = opcode_0xf8;
    ret[249] = opcode_0xf9;
    ret[250] = opcode_0xfa;
    ret[251] = opcode_0xfb;
    ret[252] = opcode_0xfc;
    ret[253] = opcode_0xfd;
    ret[254] = opcode_0xfe;
    ret[255] = opcode_0xff;
    return ret;
}

std::array<int (*)(Registers&, RAM&), 256> create_cb_table() {
    std::array<int (*)(Registers&, RAM&), 256> ret;

    ret[0]   = opcode_0xcb00;
    ret[1]   = opcode_0xcb01;
    ret[2]   = opcode_0xcb02;
    ret[3]   = opcode_0xcb03;
    ret[4]   = opcode_0xcb04;
    ret[5]   = opcode_0xcb05;
    ret[6]   = opcode_0xcb06;
    ret[7]   = opcode_0xcb07;
    ret[8]   = opcode_0xcb08;
    ret[9]   = opcode_0xcb09;
    ret[10]  = opcode_0xcb0a;
    ret[11]  = opcode_0xcb0b;
    ret[12]  = opcode_0xcb0c;
    ret[13]  = opcode_0xcb0d;
    ret[14]  = opcode_0xcb0e;
    ret[15]  = opcode_0xcb0f;
    ret[16]  = opcode_0xcb10;
    ret[17]  = opcode_0xcb11;
    ret[18]  = opcode_0xcb12;
    ret[19]  = opcode_0xcb13;
    ret[20]  = opcode_0xcb14;
    ret[21]  = opcode_0xcb15;
    ret[22]  = opcode_0xcb16;
    ret[23]  = opcode_0xcb17;
    ret[24]  = opcode_0xcb18;
    ret[25]  = opcode_0xcb19;
    ret[26]  = opcode_0xcb1a;
    ret[27]  = opcode_0xcb1b;
    ret[28]  = opcode_0xcb1c;
    ret[29]  = opcode_0xcb1d;
    ret[30]  = opcode_0xcb1e;
    ret[31]  = opcode_0xcb1f;
    ret[32]  = opcode_0xcb20;
    ret[33]  = opcode_0xcb21;
    ret[34]  = opcode_0xcb22;
    ret[35]  = opcode_0xcb23;
    ret[36]  = opcode_0xcb24;
    ret[37]  = opcode_0xcb25;
    ret[38]  = opcode_0xcb26;
    ret[39]  = opcode_0xcb27;
    ret[40]  = opcode_0xcb28;
    ret[41]  = opcode_0xcb29;
    ret[42]  = opcode_0xcb2a;
    ret[43]  = opcode_0xcb2b;
    ret[44]  = opcode_0xcb2c;
    ret[45]  = opcode_0xcb2d;
    ret[46]  = opcode_0xcb2e;
    ret[47]  = opcode_0xcb2f;
    ret[48]  = opcode_0xcb30;
    ret[49]  = opcode_0xcb31;
    ret[50]  = opcode_0xcb32;
    ret[51]  = opcode_0xcb33;
    ret[52]  = opcode_0xcb34;
    ret[53]  = opcode_0xcb35;
    ret[54]  = opcode_0xcb36;
    ret[55]  = opcode_0xcb37;
    ret[56]  = opcode_0xcb38;
    ret[57]  = opcode_0xcb39;
    ret[58]  = opcode_0xcb3a;
    ret[59]  = opcode_0xcb3b;
    ret[60]  = opcode_0xcb3c;
    ret[61]  = opcode_0xcb3d;
    ret[62]  = opcode_0xcb3e;
    ret[63]  = opcode_0xcb3f;
    ret[64]  = opcode_0xcb40;
    ret[65]  = opcode_0xcb41;
    ret[66]  = opcode_0xcb42;
    ret[67]  = opcode_0xcb43;
    ret[68]  = opcode_0xcb44;
    ret[69]  = opcode_0xcb45;
    ret[70]  = opcode_0xcb46;
    ret[71]  = opcode_0xcb47;
    ret[72]  = opcode_0xcb48;
    ret[73]  = opcode_0xcb49;
    ret[74]  = opcode_0xcb4a;
    ret[75]  = opcode_0xcb4b;
    ret[76]  = opcode_0xcb4c;
    ret[77]  = opcode_0xcb4d;
    ret[78]  = opcode_0xcb4e;
    ret[79]  = opcode_0xcb4f;
    ret[80]  = opcode_0xcb50;
    ret[81]  = opcode_0xcb51;
    ret[82]  = opcode_0xcb52;
    ret[83]  = opcode_0xcb53;
    ret[84]  = opcode_0xcb54;
    ret[85]  = opcode_0xcb55;
    ret[86]  = opcode_0xcb56;
    ret[87]  = opcode_0xcb57;
    ret[88]  = opcode_0xcb58;
    ret[89]  = opcode_0xcb59;
    ret[90]  = opcode_0xcb5a;
    ret[91]  = opcode_0xcb5b;
    ret[92]  = opcode_0xcb5c;
    ret[93]  = opcode_0xcb5d;
    ret[94]  = opcode_0xcb5e;
    ret[95]  = opcode_0xcb5f;
    ret[96]  = opcode_0xcb60;
    ret[97]  = opcode_0xcb61;
    ret[98]  = opcode_0xcb62;
    ret[99]  = opcode_0xcb63;
    ret[100] = opcode_0xcb64;
    ret[101] = opcode_0xcb65;
    ret[102] = opcode_0xcb66;
    ret[103] = opcode_0xcb67;
    ret[104] = opcode_0xcb68;
    ret[105] = opcode_0xcb69;
    ret[106] = opcode_0xcb6a;
    ret[107] = opcode_0xcb6b;
    ret[108] = opcode_0xcb6c;
    ret[109] = opcode_0xcb6d;
    ret[110] = opcode_0xcb6e;
    ret[111] = opcode_0xcb6f;
    ret[112] = opcode_0xcb70;
    ret[113] = opcode_0xcb71;
    ret[114] = opcode_0xcb72;
    ret[115] = opcode_0xcb73;
    ret[116] = opcode_0xcb74;
    ret[117] = opcode_0xcb75;
    ret[118] = opcode_0xcb76;
    ret[119] = opcode_0xcb77;
    ret[120] = opcode_0xcb78;
    ret[121] = opcode_0xcb79;
    ret[122] = opcode_0xcb7a;
    ret[123] = opcode_0xcb7b;
    ret[124] = opcode_0xcb7c;
    ret[125] = opcode_0xcb7d;
    ret[126] = opcode_0xcb7e;
    ret[127] = opcode_0xcb7f;
    ret[128] = opcode_0xcb80;
    ret[129] = opcode_0xcb81;
    ret[130] = opcode_0xcb82;
    ret[131] = opcode_0xcb83;
    ret[132] = opcode_0xcb84;
    ret[133] = opcode_0xcb85;
    ret[134] = opcode_0xcb86;
    ret[135] = opcode_0xcb87;
    ret[136] = opcode_0xcb88;
    ret[137] = opcode_0xcb89;
    ret[138] = opcode_0xcb8a;
    ret[139] = opcode_0xcb8b;
    ret[140] = opcode_0xcb8c;
    ret[141] = opcode_0xcb8d;
    ret[142] = opcode_0xcb8e;
    ret[143] = opcode_0xcb8f;
    ret[144] = opcode_0xcb90;
    ret[145] = opcode_0xcb91;
    ret[146] = opcode_0xcb92;
    ret[147] = opcode_0xcb93;
    ret[148] = opcode_0xcb94;
    ret[149] = opcode_0xcb95;
    ret[150] = opcode_0xcb96;
    ret[151] = opcode_0xcb97;
    ret[152] = opcode_0xcb98;
    ret[153] = opcode_0xcb99;
    ret[154] = opcode_0xcb9a;
    ret[155] = opcode_0xcb9b;
    ret[156] = opcode_0xcb9c;
    ret[157] = opcode_0xcb9d;
    ret[158] = opcode_0xcb9e;
    ret[159] = opcode_0xcb9f;
    ret[160] = opcode_0xcba0;
    ret[161] = opcode_0xcba1;
    ret[162] = opcode_0xcba2;
    ret[163] = opcode_0xcba3;
    ret[164] = opcode_0xcba4;
    ret[165] = opcode_0xcba5;
    ret[166] = opcode_0xcba6;
    ret[167] = opcode_0xcba7;
    ret[168] = opcode_0xcba8;
    ret[169] = opcode_0xcba9;
    ret[170] = opcode_0xcbaa;
    ret[171] = opcode_0xcbab;
    ret[172] = opcode_0xcbac;
    ret[173] = opcode_0xcbad;
    ret[174] = opcode_0xcbae;
    ret[175] = opcode_0xcbaf;
    ret[176] = opcode_0xcbb0;
    ret[177] = opcode_0xcbb1;
    ret[178] = opcode_0xcbb2;
    ret[179] = opcode_0xcbb3;
    ret[180] = opcode_0xcbb4;
    ret[181] = opcode_0xcbb5;
    ret[182] = opcode_0xcbb6;
    ret[183] = opcode_0xcbb7;
    ret[184] = opcode_0xcbb8;
    ret[185] = opcode_0xcbb9;
    ret[186] = opcode_0xcbba;
    ret[187] = opcode_0xcbbb;
    ret[188] = opcode_0xcbbc;
    ret[189] = opcode_0xcbbd;
    ret[190] = opcode_0xcbbe;
    ret[191] = opcode_0xcbbf;
    ret[192] = opcode_0xcbc0;
    ret[193] = opcode_0xcbc1;
    ret[194] = opcode_0xcbc2;
    ret[195] = opcode_0xcbc3;
    ret[196] = opcode_0xcbc4;
    ret[197] = opcode_0xcbc5;
    ret[198] = opcode_0xcbc6;
    ret[199] = opcode_0xcbc7;
    ret[200] = opcode_0xcbc8;
    ret[201] = opcode_0xcbc9;
    ret[202] = opcode_0xcbca;
    ret[203] = opcode_0xcbcb;
    ret[204] = opcode_0xcbcc;
    ret[205] = opcode_0xcbcd;
    ret[206] = opcode_0xcbce;
    ret[207] = opcode_0xcbcf;
    ret[208] = opcode_0xcbd0;
    ret[209] = opcode_0xcbd1;
    ret[210] = opcode_0xcbd2;
    ret[211] = opcode_0xcbd3;
    ret[212] = opcode_0xcbd4;
    ret[213] = opcode_0xcbd5;
    ret[214] = opcode_0xcbd6;
    ret[215] = opcode_0xcbd7;
    ret[216] = opcode_0xcbd8;
    ret[217] = opcode_0xcbd9;
    ret[218] = opcode_0xcbda;
    ret[219] = opcode_0xcbdb;
    ret[220] = opcode_0xcbdc;
    ret[221] = opcode_0xcbdd;
    ret[222] = opcode_0xcbde;
    ret[223] = opcode_0xcbdf;
    ret[224] = opcode_0xcbe0;
    ret[225] = opcode_0xcbe1;
    ret[226] = opcode_0xcbe2;
    ret[227] = opcode_0xcbe3;
    ret[228] = opcode_0xcbe4;
    ret[229] = opcode_0xcbe5;
    ret[230] = opcode_0xcbe6;
    ret[231] = opcode_0xcbe7;
    ret[232] = opcode_0xcbe8;
    ret[233] = opcode_0xcbe9;
    ret[234] = opcode_0xcbea;
    ret[235] = opcode_0xcbeb;
    ret[236] = opcode_0xcbec;
    ret[237] = opcode_0xcbed;
    ret[238] = opcode_0xcbee;
    ret[239] = opcode_0xcbef;
    ret[240] = opcode_0xcbf0;
    ret[241] = opcode_0xcbf1;
    ret[242] = opcode_0xcbf2;
    ret[243] = opcode_0xcbf3;
    ret[244] = opcode_0xcbf4;
    ret[245] = opcode_0xcbf5;
    ret[246] = opcode_0xcbf6;
    ret[247] = opcode_0xcbf7;
    ret[248] = opcode_0xcbf8;
    ret[249] = opcode_0xcbf9;
    ret[250] = opcode_0xcbfa;
    ret[251] = opcode_0xcbfb;
    ret[252] = opcode_0xcbfc;
    ret[253] = opcode_0xcbfd;
    ret[254] = opcode_0xcbfe;
    ret[255] = opcode_0xcbff;
    return ret;
}

int opcode_0x00(Registers& r, RAM& m) {
    /*  mnemonic: NOP
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    return 4;
}
int opcode_0x01(Registers& r, RAM& m) {
    /*  mnemonic: LD BC,d16
   *  length: 3
   *  cycles: 12
   *  affects: - - - -
   */
    LD(r, m, r.BC, m.imm16(r.PC));
    return 12;
}
int opcode_0x02(Registers& r, RAM& m) {
    /*  mnemonic: LD (BC),A
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.BC], r.A);
    return 8;
}
int opcode_0x03(Registers& r, RAM& m) {
    /*  mnemonic: INC BC
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    INC(r, m, r.BC);
    return 8;
}
int opcode_0x04(Registers& r, RAM& m) {
    /*  mnemonic: INC B
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H -
   */
    INC(r, m, r.B);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x05(Registers& r, RAM& m) {
    /*  mnemonic: DEC B
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H -
   */
    DEC(r, m, r.B);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x06(Registers& r, RAM& m) {
    /*  mnemonic: LD B,d8
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.B, m.imm8(r.PC));
    return 8;
}
int opcode_0x07(Registers& r, RAM& m) {
    /*  mnemonic: RLCA
   *  length: 1
   *  cycles: 4
   *  affects: 0 0 0 C
   */
    RLCA(r, m);
    r.ZERO_FLAG      = 0;
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x08(Registers& r, RAM& m) {
    /*  mnemonic: LD (a16),SP
   *  length: 3
   *  cycles: 20
   *  affects: - - - -
   */
    LD(r, m, m[m.imm16(r.PC)], r.SP);
    return 20;
}
int opcode_0x09(Registers& r, RAM& m) {
    /*  mnemonic: ADD HL,BC
   *  length: 1
   *  cycles: 8
   *  affects: - 0 H C
   */
    ADD(r, m, r.HL, r.BC);
    r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x0a(Registers& r, RAM& m) {
    /*  mnemonic: LD A,(BC)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.A, m[r.BC]);
    return 8;
}
int opcode_0x0b(Registers& r, RAM& m) {
    /*  mnemonic: DEC BC
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    DEC(r, m, r.BC);
    return 8;
}
int opcode_0x0c(Registers& r, RAM& m) {
    /*  mnemonic: INC C
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H -
   */
    INC(r, m, r.C);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x0d(Registers& r, RAM& m) {
    /*  mnemonic: DEC C
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H -
   */
    DEC(r, m, r.C);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x0e(Registers& r, RAM& m) {
    /*  mnemonic: LD C,d8
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.C, m.imm8(r.PC));
    return 8;
}
int opcode_0x0f(Registers& r, RAM& m) {
    /*  mnemonic: RRCA
   *  length: 1
   *  cycles: 4
   *  affects: 0 0 0 C
   */
    RRCA(r, m);
    r.ZERO_FLAG      = 0;
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x10(Registers& r, RAM& m) {
    /*  mnemonic: STOP 0
   *  length: 2
   *  cycles: 4
   *  affects: - - - -
   */
    STOP(r, m);
    return 4;
}
int opcode_0x11(Registers& r, RAM& m) {
    /*  mnemonic: LD DE,d16
   *  length: 3
   *  cycles: 12
   *  affects: - - - -
   */
    LD(r, m, r.DE, m.imm16(r.PC));
    return 12;
}
int opcode_0x12(Registers& r, RAM& m) {
    /*  mnemonic: LD (DE),A
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.DE], r.A);
    return 8;
}
int opcode_0x13(Registers& r, RAM& m) {
    /*  mnemonic: INC DE
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    INC(r, m, r.DE);
    return 8;
}
int opcode_0x14(Registers& r, RAM& m) {
    /*  mnemonic: INC D
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H -
   */
    INC(r, m, r.D);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x15(Registers& r, RAM& m) {
    /*  mnemonic: DEC D
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H -
   */
    DEC(r, m, r.D);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x16(Registers& r, RAM& m) {
    /*  mnemonic: LD D,d8
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.D, m.imm8(r.PC));
    return 8;
}
int opcode_0x17(Registers& r, RAM& m) {
    /*  mnemonic: RLA
   *  length: 1
   *  cycles: 4
   *  affects: 0 0 0 C
   */
    RLA(r, m);
    r.ZERO_FLAG      = 0;
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x18(Registers& r, RAM& m) {
    /*  mnemonic: JR r8
   *  length: 2
   *  cycles: 12
   *  affects: - - - -
   */
    JR(r, m, m.imm8(r.PC));
    return 12;
}
int opcode_0x19(Registers& r, RAM& m) {
    /*  mnemonic: ADD HL,DE
   *  length: 1
   *  cycles: 8
   *  affects: - 0 H C
   */
    ADD(r, m, r.HL, r.DE);
    r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x1a(Registers& r, RAM& m) {
    /*  mnemonic: LD A,(DE)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.A, m[r.DE]);
    return 8;
}
int opcode_0x1b(Registers& r, RAM& m) {
    /*  mnemonic: DEC DE
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    DEC(r, m, r.DE);
    return 8;
}
int opcode_0x1c(Registers& r, RAM& m) {
    /*  mnemonic: INC E
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H -
   */
    INC(r, m, r.E);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x1d(Registers& r, RAM& m) {
    /*  mnemonic: DEC E
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H -
   */
    DEC(r, m, r.E);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x1e(Registers& r, RAM& m) {
    /*  mnemonic: LD E,d8
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.E, m.imm8(r.PC));
    return 8;
}
int opcode_0x1f(Registers& r, RAM& m) {
    /*  mnemonic: RRA
   *  length: 1
   *  cycles: 4
   *  affects: 0 0 0 C
   */
    RRA(r, m);
    r.ZERO_FLAG      = 0;
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x20(Registers& r, RAM& m) {
    /*  mnemonic: JR NZ,r8
   *  length: 2
   *  cycles: 12/8
   *  affects: - - - -
   */
    return JP(r, m, !r.ZERO_FLAG, m.imm8(r.PC), 12, 8);
}
int opcode_0x21(Registers& r, RAM& m) {
    /*  mnemonic: LD HL,d16
   *  length: 3
   *  cycles: 12
   *  affects: - - - -
   */
    LD(r, m, r.HL, m.imm16(r.PC));
    return 12;
}
int opcode_0x22(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL+),A
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.HL++], r.A);
    return 8;
}
int opcode_0x23(Registers& r, RAM& m) {
    /*  mnemonic: INC HL
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    INC(r, m, r.HL);
    return 8;
}
int opcode_0x24(Registers& r, RAM& m) {
    /*  mnemonic: INC H
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H -
   */
    INC(r, m, r.H);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x25(Registers& r, RAM& m) {
    /*  mnemonic: DEC H
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H -
   */
    DEC(r, m, r.H);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x26(Registers& r, RAM& m) {
    /*  mnemonic: LD H,d8
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.H, m.imm8(r.PC));
    return 8;
}
int opcode_0x27(Registers& r, RAM& m) {
    /*  mnemonic: DAA
   *  length: 1
   *  cycles: 4
   *  affects: Z - 0 C
   */
    DAA(r, m);
    r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x28(Registers& r, RAM& m) {
    /*  mnemonic: JR Z,r8
   *  length: 2
   *  cycles: 12/8
   *  affects: - - - -
   */
    return JP(r, m, r.ZERO_FLAG, m.imm8(r.PC), 12, 8);
}
int opcode_0x29(Registers& r, RAM& m) {
    /*  mnemonic: ADD HL,HL
   *  length: 1
   *  cycles: 8
   *  affects: - 0 H C
   */
    ADD(r, m, r.HL, r.HL);
    r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x2a(Registers& r, RAM& m) {
    /*  mnemonic: LD A,(HL+)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.A, m[r.HL++]);
    return 8;
}
int opcode_0x2b(Registers& r, RAM& m) {
    /*  mnemonic: DEC HL
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    DEC(r, m, r.HL);
    return 8;
}
int opcode_0x2c(Registers& r, RAM& m) {
    /*  mnemonic: INC L
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H -
   */
    INC(r, m, r.L);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x2d(Registers& r, RAM& m) {
    /*  mnemonic: DEC L
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H -
   */
    DEC(r, m, r.L);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x2e(Registers& r, RAM& m) {
    /*  mnemonic: LD L,d8
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.L, m.imm8(r.PC));
    return 8;
}
int opcode_0x2f(Registers& r, RAM& m) {
    /*  mnemonic: CPL
   *  length: 1
   *  cycles: 4
   *  affects: - 1 1 -
   */
    CPL(r, m);
    r.SUB_FLAG       = 1;
    r.HALFCARRY_FLAG = 1;
    return 4;
}
int opcode_0x30(Registers& r, RAM& m) {
    /*  mnemonic: JR NC,r8
   *  length: 2
   *  cycles: 12/8
   *  affects: - - - -
   */
    return JP(r, m, !r.CARRY_FLAG, m.imm8(r.PC), 12, 8);
}
int opcode_0x31(Registers& r, RAM& m) {
    /*  mnemonic: LD SP,d16
   *  length: 3
   *  cycles: 12
   *  affects: - - - -
   */
    LD(r, m, r.SP, m.imm16(r.PC));
    return 12;
}
int opcode_0x32(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL-),A
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.HL--], r.A);
    return 8;
}
int opcode_0x33(Registers& r, RAM& m) {
    /*  mnemonic: INC SP
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    INC(r, m, r.SP);
    return 8;
}
int opcode_0x34(Registers& r, RAM& m) {
    /*  mnemonic: INC (HL)
   *  length: 1
   *  cycles: 12
   *  affects: Z 0 H -
   */
    INC(r, m, m[r.HL]);
    r.SUB_FLAG = 0;
    return 12;
}
int opcode_0x35(Registers& r, RAM& m) {
    /*  mnemonic: DEC (HL)
   *  length: 1
   *  cycles: 12
   *  affects: Z 1 H -
   */
    DEC(r, m, m[r.HL]);
    r.SUB_FLAG = 1;
    return 12;
}
int opcode_0x36(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL),d8
   *  length: 2
   *  cycles: 12
   *  affects: - - - -
   */
    LD(r, m, m[r.HL], m.imm8(r.PC));
    return 12;
}
int opcode_0x37(Registers& r, RAM& m) {
    /*  mnemonic: SCF
   *  length: 1
   *  cycles: 4
   *  affects: - 0 0 1
   */
    SCF(r, m);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 1;
    return 4;
}
int opcode_0x38(Registers& r, RAM& m) {
    /*  mnemonic: JR C,r8
   *  length: 2
   *  cycles: 12/8
   *  affects: - - - -
   */
    return JP(r, m, r.C, m.imm8(r.PC), 12, 8);
}
int opcode_0x39(Registers& r, RAM& m) {
    /*  mnemonic: ADD HL,SP
   *  length: 1
   *  cycles: 8
   *  affects: - 0 H C
   */
    ADD(r, m, r.HL, r.SP);
    r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x3a(Registers& r, RAM& m) {
    /*  mnemonic: LD A,(HL-)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.A, m[r.HL--]);
    return 8;
}
int opcode_0x3b(Registers& r, RAM& m) {
    /*  mnemonic: DEC SP
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    DEC(r, m, r.SP);
    return 8;
}
int opcode_0x3c(Registers& r, RAM& m) {
    /*  mnemonic: INC A
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H -
   */
    INC(r, m, r.A);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x3d(Registers& r, RAM& m) {
    /*  mnemonic: DEC A
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H -
   */
    DEC(r, m, r.A);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x3e(Registers& r, RAM& m) {
    /*  mnemonic: LD A,d8
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.A, m.imm8(r.PC));
    return 8;
}
int opcode_0x3f(Registers& r, RAM& m) {
    /*  mnemonic: CCF
   *  length: 1
   *  cycles: 4
   *  affects: - 0 0 C
   */
    CCF(r, m);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x40(Registers& r, RAM& m) {
    /*  mnemonic: LD B,B
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.B, r.B);
    return 4;
}
int opcode_0x41(Registers& r, RAM& m) {
    /*  mnemonic: LD B,C
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.B, r.C);
    return 4;
}
int opcode_0x42(Registers& r, RAM& m) {
    /*  mnemonic: LD B,D
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.B, r.D);
    return 4;
}
int opcode_0x43(Registers& r, RAM& m) {
    /*  mnemonic: LD B,E
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.B, r.E);
    return 4;
}
int opcode_0x44(Registers& r, RAM& m) {
    /*  mnemonic: LD B,H
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.B, r.H);
    return 4;
}
int opcode_0x45(Registers& r, RAM& m) {
    /*  mnemonic: LD B,L
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.B, r.L);
    return 4;
}
int opcode_0x46(Registers& r, RAM& m) {
    /*  mnemonic: LD B,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.B, m[r.HL]);
    return 8;
}
int opcode_0x47(Registers& r, RAM& m) {
    /*  mnemonic: LD B,A
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.B, r.A);
    return 4;
}
int opcode_0x48(Registers& r, RAM& m) {
    /*  mnemonic: LD C,B
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.C, r.B);
    return 4;
}
int opcode_0x49(Registers& r, RAM& m) {
    /*  mnemonic: LD C,C
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.C, r.C);
    return 4;
}
int opcode_0x4a(Registers& r, RAM& m) {
    /*  mnemonic: LD C,D
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.C, r.D);
    return 4;
}
int opcode_0x4b(Registers& r, RAM& m) {
    /*  mnemonic: LD C,E
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.C, r.E);
    return 4;
}
int opcode_0x4c(Registers& r, RAM& m) {
    /*  mnemonic: LD C,H
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.C, r.H);
    return 4;
}
int opcode_0x4d(Registers& r, RAM& m) {
    /*  mnemonic: LD C,L
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.C, r.L);
    return 4;
}
int opcode_0x4e(Registers& r, RAM& m) {
    /*  mnemonic: LD C,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.C, m[r.HL]);
    return 8;
}
int opcode_0x4f(Registers& r, RAM& m) {
    /*  mnemonic: LD C,A
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.C, r.A);
    return 4;
}
int opcode_0x50(Registers& r, RAM& m) {
    /*  mnemonic: LD D,B
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.D, r.B);
    return 4;
}
int opcode_0x51(Registers& r, RAM& m) {
    /*  mnemonic: LD D,C
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.D, r.C);
    return 4;
}
int opcode_0x52(Registers& r, RAM& m) {
    /*  mnemonic: LD D,D
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.D, r.D);
    return 4;
}
int opcode_0x53(Registers& r, RAM& m) {
    /*  mnemonic: LD D,E
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.D, r.E);
    return 4;
}
int opcode_0x54(Registers& r, RAM& m) {
    /*  mnemonic: LD D,H
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.D, r.H);
    return 4;
}
int opcode_0x55(Registers& r, RAM& m) {
    /*  mnemonic: LD D,L
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.D, r.L);
    return 4;
}
int opcode_0x56(Registers& r, RAM& m) {
    /*  mnemonic: LD D,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.D, m[r.HL]);
    return 8;
}
int opcode_0x57(Registers& r, RAM& m) {
    /*  mnemonic: LD D,A
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.D, r.A);
    return 4;
}
int opcode_0x58(Registers& r, RAM& m) {
    /*  mnemonic: LD E,B
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.E, r.B);
    return 4;
}
int opcode_0x59(Registers& r, RAM& m) {
    /*  mnemonic: LD E,C
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.E, r.C);
    return 4;
}
int opcode_0x5a(Registers& r, RAM& m) {
    /*  mnemonic: LD E,D
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.E, r.D);
    return 4;
}
int opcode_0x5b(Registers& r, RAM& m) {
    /*  mnemonic: LD E,E
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.E, r.E);
    return 4;
}
int opcode_0x5c(Registers& r, RAM& m) {
    /*  mnemonic: LD E,H
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.E, r.H);
    return 4;
}
int opcode_0x5d(Registers& r, RAM& m) {
    /*  mnemonic: LD E,L
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.E, r.L);
    return 4;
}
int opcode_0x5e(Registers& r, RAM& m) {
    /*  mnemonic: LD E,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.E, m[r.HL]);
    return 8;
}
int opcode_0x5f(Registers& r, RAM& m) {
    /*  mnemonic: LD E,A
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.E, r.A);
    return 4;
}
int opcode_0x60(Registers& r, RAM& m) {
    /*  mnemonic: LD H,B
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.H, r.B);
    return 4;
}
int opcode_0x61(Registers& r, RAM& m) {
    /*  mnemonic: LD H,C
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.H, r.C);
    return 4;
}
int opcode_0x62(Registers& r, RAM& m) {
    /*  mnemonic: LD H,D
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.H, r.D);
    return 4;
}
int opcode_0x63(Registers& r, RAM& m) {
    /*  mnemonic: LD H,E
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.H, r.E);
    return 4;
}
int opcode_0x64(Registers& r, RAM& m) {
    /*  mnemonic: LD H,H
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.H, r.H);
    return 4;
}
int opcode_0x65(Registers& r, RAM& m) {
    /*  mnemonic: LD H,L
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.H, r.L);
    return 4;
}
int opcode_0x66(Registers& r, RAM& m) {
    /*  mnemonic: LD H,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.H, m[r.HL]);
    return 8;
}
int opcode_0x67(Registers& r, RAM& m) {
    /*  mnemonic: LD H,A
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.H, r.A);
    return 4;
}
int opcode_0x68(Registers& r, RAM& m) {
    /*  mnemonic: LD L,B
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.L, r.B);
    return 4;
}
int opcode_0x69(Registers& r, RAM& m) {
    /*  mnemonic: LD L,C
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.L, r.C);
    return 4;
}
int opcode_0x6a(Registers& r, RAM& m) {
    /*  mnemonic: LD L,D
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.L, r.D);
    return 4;
}
int opcode_0x6b(Registers& r, RAM& m) {
    /*  mnemonic: LD L,E
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.L, r.E);
    return 4;
}
int opcode_0x6c(Registers& r, RAM& m) {
    /*  mnemonic: LD L,H
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.L, r.H);
    return 4;
}
int opcode_0x6d(Registers& r, RAM& m) {
    /*  mnemonic: LD L,L
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.L, r.L);
    return 4;
}
int opcode_0x6e(Registers& r, RAM& m) {
    /*  mnemonic: LD L,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.L, m[r.HL]);
    return 8;
}
int opcode_0x6f(Registers& r, RAM& m) {
    /*  mnemonic: LD L,A
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.L, r.A);
    return 4;
}
int opcode_0x70(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL),B
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.HL], r.B);
    return 8;
}
int opcode_0x71(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL),C
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.HL], r.C);
    return 8;
}
int opcode_0x72(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL),D
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.HL], r.D);
    return 8;
}
int opcode_0x73(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL),E
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.HL], r.E);
    return 8;
}
int opcode_0x74(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL),H
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.HL], r.H);
    return 8;
}
int opcode_0x75(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL),L
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.HL], r.L);
    return 8;
}
int opcode_0x76(Registers& r, RAM& m) {
    /*  mnemonic: HALT
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    return 4;
}
int opcode_0x77(Registers& r, RAM& m) {
    /*  mnemonic: LD (HL),A
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.HL], r.A);
    return 8;
}
int opcode_0x78(Registers& r, RAM& m) {
    /*  mnemonic: LD A,B
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.A, r.B);
    return 4;
}
int opcode_0x79(Registers& r, RAM& m) {
    /*  mnemonic: LD A,C
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.A, r.C);
    return 4;
}
int opcode_0x7a(Registers& r, RAM& m) {
    /*  mnemonic: LD A,D
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.A, r.D);
    return 4;
}
int opcode_0x7b(Registers& r, RAM& m) {
    /*  mnemonic: LD A,E
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.A, r.E);
    return 4;
}
int opcode_0x7c(Registers& r, RAM& m) {
    /*  mnemonic: LD A,H
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.A, r.H);
    return 4;
}
int opcode_0x7d(Registers& r, RAM& m) {
    /*  mnemonic: LD A,L
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.A, r.L);
    return 4;
}
int opcode_0x7e(Registers& r, RAM& m) {
    /*  mnemonic: LD A,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.A, m[r.HL]);
    return 8;
}
int opcode_0x7f(Registers& r, RAM& m) {
    /*  mnemonic: LD A,A
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    LD(r, m, r.A, r.A);
    return 4;
}
int opcode_0x80(Registers& r, RAM& m) {
    /*  mnemonic: ADD A,B
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADD(r, m, r.A, r.B);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x81(Registers& r, RAM& m) {
    /*  mnemonic: ADD A,C
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADD(r, m, r.A, r.C);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x82(Registers& r, RAM& m) {
    /*  mnemonic: ADD A,D
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADD(r, m, r.A, r.D);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x83(Registers& r, RAM& m) {
    /*  mnemonic: ADD A,E
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADD(r, m, r.A, r.E);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x84(Registers& r, RAM& m) {
    /*  mnemonic: ADD A,H
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADD(r, m, r.A, r.H);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x85(Registers& r, RAM& m) {
    /*  mnemonic: ADD A,L
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADD(r, m, r.A, r.L);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x86(Registers& r, RAM& m) {
    /*  mnemonic: ADD A,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: Z 0 H C
   */
    ADD(r, m, r.A, m[r.HL]);
    r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x87(Registers& r, RAM& m) {
    /*  mnemonic: ADD A,A
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADD(r, m, r.A, r.A);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x88(Registers& r, RAM& m) {
    /*  mnemonic: ADC A,B
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADC(r, m, r.A, r.B);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x89(Registers& r, RAM& m) {
    /*  mnemonic: ADC A,C
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADC(r, m, r.A, r.C);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8a(Registers& r, RAM& m) {
    /*  mnemonic: ADC A,D
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADC(r, m, r.A, r.D);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8b(Registers& r, RAM& m) {
    /*  mnemonic: ADC A,E
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADC(r, m, r.A, r.E);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8c(Registers& r, RAM& m) {
    /*  mnemonic: ADC A,H
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADC(r, m, r.A, r.H);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8d(Registers& r, RAM& m) {
    /*  mnemonic: ADC A,L
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADC(r, m, r.A, r.L);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8e(Registers& r, RAM& m) {
    /*  mnemonic: ADC A,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: Z 0 H C
   */
    ADC(r, m, r.A, m[r.HL]);
    r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x8f(Registers& r, RAM& m) {
    /*  mnemonic: ADC A,A
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 H C
   */
    ADC(r, m, r.A, r.A);
    r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x90(Registers& r, RAM& m) {
    /*  mnemonic: SUB B
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SUB(r, m, r.B);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x91(Registers& r, RAM& m) {
    /*  mnemonic: SUB C
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SUB(r, m, r.C);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x92(Registers& r, RAM& m) {
    /*  mnemonic: SUB D
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SUB(r, m, r.D);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x93(Registers& r, RAM& m) {
    /*  mnemonic: SUB E
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SUB(r, m, r.E);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x94(Registers& r, RAM& m) {
    /*  mnemonic: SUB H
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SUB(r, m, r.H);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x95(Registers& r, RAM& m) {
    /*  mnemonic: SUB L
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SUB(r, m, r.L);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x96(Registers& r, RAM& m) {
    /*  mnemonic: SUB (HL)
   *  length: 1
   *  cycles: 8
   *  affects: Z 1 H C
   */
    SUB(r, m, m[r.HL]);
    r.SUB_FLAG = 1;
    return 8;
}
int opcode_0x97(Registers& r, RAM& m) {
    /*  mnemonic: SUB A
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SUB(r, m, r.A);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x98(Registers& r, RAM& m) {
    /*  mnemonic: SBC A,B
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SBC(r, m, r.A, r.B);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x99(Registers& r, RAM& m) {
    /*  mnemonic: SBC A,C
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SBC(r, m, r.A, r.C);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9a(Registers& r, RAM& m) {
    /*  mnemonic: SBC A,D
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SBC(r, m, r.A, r.D);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9b(Registers& r, RAM& m) {
    /*  mnemonic: SBC A,E
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SBC(r, m, r.A, r.E);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9c(Registers& r, RAM& m) {
    /*  mnemonic: SBC A,H
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SBC(r, m, r.A, r.H);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9d(Registers& r, RAM& m) {
    /*  mnemonic: SBC A,L
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SBC(r, m, r.A, r.L);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9e(Registers& r, RAM& m) {
    /*  mnemonic: SBC A,(HL)
   *  length: 1
   *  cycles: 8
   *  affects: Z 1 H C
   */
    SBC(r, m, r.A, m[r.HL]);
    r.SUB_FLAG = 1;
    return 8;
}
int opcode_0x9f(Registers& r, RAM& m) {
    /*  mnemonic: SBC A,A
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    SBC(r, m, r.A, r.A);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xa0(Registers& r, RAM& m) {
    /*  mnemonic: AND B
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 1 0
   */
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa1(Registers& r, RAM& m) {
    /*  mnemonic: AND C
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 1 0
   */
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa2(Registers& r, RAM& m) {
    /*  mnemonic: AND D
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 1 0
   */
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa3(Registers& r, RAM& m) {
    /*  mnemonic: AND E
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 1 0
   */
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa4(Registers& r, RAM& m) {
    /*  mnemonic: AND H
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 1 0
   */
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa5(Registers& r, RAM& m) {
    /*  mnemonic: AND L
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 1 0
   */
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa6(Registers& r, RAM& m) {
    /*  mnemonic: AND (HL)
   *  length: 1
   *  cycles: 8
   *  affects: Z 0 1 0
   */
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xa7(Registers& r, RAM& m) {
    /*  mnemonic: AND A
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 1 0
   */
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa8(Registers& r, RAM& m) {
    /*  mnemonic: XOR B
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    XOR(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa9(Registers& r, RAM& m) {
    /*  mnemonic: XOR C
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    XOR(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xaa(Registers& r, RAM& m) {
    /*  mnemonic: XOR D
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    XOR(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xab(Registers& r, RAM& m) {
    /*  mnemonic: XOR E
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    XOR(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xac(Registers& r, RAM& m) {
    /*  mnemonic: XOR H
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    XOR(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xad(Registers& r, RAM& m) {
    /*  mnemonic: XOR L
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    XOR(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xae(Registers& r, RAM& m) {
    /*  mnemonic: XOR (HL)
   *  length: 1
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    XOR(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xaf(Registers& r, RAM& m) {
    /*  mnemonic: XOR A
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    XOR(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb0(Registers& r, RAM& m) {
    /*  mnemonic: OR B
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    OR(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb1(Registers& r, RAM& m) {
    /*  mnemonic: OR C
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    OR(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb2(Registers& r, RAM& m) {
    /*  mnemonic: OR D
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    OR(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb3(Registers& r, RAM& m) {
    /*  mnemonic: OR E
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    OR(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb4(Registers& r, RAM& m) {
    /*  mnemonic: OR H
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    OR(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb5(Registers& r, RAM& m) {
    /*  mnemonic: OR L
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    OR(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb6(Registers& r, RAM& m) {
    /*  mnemonic: OR (HL)
   *  length: 1
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    OR(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xb7(Registers& r, RAM& m) {
    /*  mnemonic: OR A
   *  length: 1
   *  cycles: 4
   *  affects: Z 0 0 0
   */
    OR(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb8(Registers& r, RAM& m) {
    /*  mnemonic: CP B
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    CP(r, m, r.B);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xb9(Registers& r, RAM& m) {
    /*  mnemonic: CP C
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    CP(r, m, r.C);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xba(Registers& r, RAM& m) {
    /*  mnemonic: CP D
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    CP(r, m, r.D);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xbb(Registers& r, RAM& m) {
    /*  mnemonic: CP E
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    CP(r, m, r.E);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xbc(Registers& r, RAM& m) {
    /*  mnemonic: CP H
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    CP(r, m, r.H);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xbd(Registers& r, RAM& m) {
    /*  mnemonic: CP L
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    CP(r, m, r.L);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xbe(Registers& r, RAM& m) {
    /*  mnemonic: CP (HL)
   *  length: 1
   *  cycles: 8
   *  affects: Z 1 H C
   */
    CP(r, m, m[r.HL]);
    r.SUB_FLAG = 1;
    return 8;
}
int opcode_0xbf(Registers& r, RAM& m) {
    /*  mnemonic: CP A
   *  length: 1
   *  cycles: 4
   *  affects: Z 1 H C
   */
    CP(r, m, r.A);
    r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xc0(Registers& r, RAM& m) {
    /*  mnemonic: RET NZ
   *  length: 1
   *  cycles: 20/8
   *  affects: - - - -
   */
    return 20 / 8;
}
int opcode_0xc1(Registers& r, RAM& m) {
    /*  mnemonic: POP BC
   *  length: 1
   *  cycles: 12
   *  affects: - - - -
   */
    POP(r, m, r.BC);
    return 12;
}
int opcode_0xc2(Registers& r, RAM& m) {
    /*  mnemonic: JP NZ,a16
   *  length: 3
   *  cycles: 16/12
   *  affects: - - - -
   */
    return JP(r, m, !r.ZERO_FLAG, m.imm16(r.PC), 16, 12);
}
int opcode_0xc3(Registers& r, RAM& m) {
    /*  mnemonic: JP a16
   *  length: 3
   *  cycles: 16
   *  affects: - - - -
   */
    JP(r, m, m.imm16(r.PC));
    return 16;
}
int opcode_0xc4(Registers& r, RAM& m) {
    /*  mnemonic: CALL NZ,a16
   *  length: 3
   *  cycles: 24/12
   *  affects: - - - -
   */
    return CALL(r, m, !r.ZERO_FLAG, m.imm16(r.PC), 24, 12);
}
int opcode_0xc5(Registers& r, RAM& m) {
    /*  mnemonic: PUSH BC
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    PUSH(r, m, r.BC);
    return 16;
}
int opcode_0xc6(Registers& r, RAM& m) {
    /*  mnemonic: ADD A,d8
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 H C
   */
    ADD(r, m, r.A, m.imm8(r.PC));
    r.SUB_FLAG = 0;
    return 8;
}
int opcode_0xc7(Registers& r, RAM& m) {
    /*  mnemonic: RST 00H
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    RST(r, m, 0x00);
    return 16;
}
int opcode_0xc8(Registers& r, RAM& m) {
    /*  mnemonic: RET Z
   *  length: 1
   *  cycles: 20/8
   *  affects: - - - -
   */
    return 20 / 8;
}
int opcode_0xc9(Registers& r, RAM& m) {
    /*  mnemonic: RET
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    return 16;
}
int opcode_0xca(Registers& r, RAM& m) {
    /*  mnemonic: JP Z,a16
   *  length: 3
   *  cycles: 16/12
   *  affects: - - - -
   */
    return JP(r, m, r.ZERO_FLAG, m.imm16(r.PC), 16, 12);
}
int opcode_0xcb(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xcc(Registers& r, RAM& m) {
    /*  mnemonic: CALL Z,a16
   *  length: 3
   *  cycles: 24/12
   *  affects: - - - -
   */
    return CALL(r, m, r.ZERO_FLAG, m.imm16(r.PC), 24, 12);
}
int opcode_0xcd(Registers& r, RAM& m) {
    /*  mnemonic: CALL a16
   *  length: 3
   *  cycles: 24
   *  affects: - - - -
   */
    CALL(r, m, m.imm16(r.PC));
    return 24;
}
int opcode_0xce(Registers& r, RAM& m) {
    /*  mnemonic: ADC A,d8
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 H C
   */
    ADC(r, m, r.A, m.imm8(r.PC));
    r.SUB_FLAG = 0;
    return 8;
}
int opcode_0xcf(Registers& r, RAM& m) {
    /*  mnemonic: RST 08H
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    RST(r, m, 0x08);
    return 16;
}
int opcode_0xd0(Registers& r, RAM& m) {
    /*  mnemonic: RET NC
   *  length: 1
   *  cycles: 20/8
   *  affects: - - - -
   */
    return 20 / 8;
}
int opcode_0xd1(Registers& r, RAM& m) {
    /*  mnemonic: POP DE
   *  length: 1
   *  cycles: 12
   *  affects: - - - -
   */
    POP(r, m, r.DE);
    return 12;
}
int opcode_0xd2(Registers& r, RAM& m) {
    /*  mnemonic: JP NC,a16
   *  length: 3
   *  cycles: 16/12
   *  affects: - - - -
   */
    return JP(r, m, !r.CARRY_FLAG, m.imm16(r.PC), 16, 12);
}
int opcode_0xd3(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xd4(Registers& r, RAM& m) {
    /*  mnemonic: CALL NC,a16
   *  length: 3
   *  cycles: 24/12
   *  affects: - - - -
   */
    return CALL(r, m, !r.CARRY_FLAG, m.imm16(r.PC), 24, 12);
}
int opcode_0xd5(Registers& r, RAM& m) {
    /*  mnemonic: PUSH DE
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    PUSH(r, m, r.DE);
    return 16;
}
int opcode_0xd6(Registers& r, RAM& m) {
    /*  mnemonic: SUB d8
   *  length: 2
   *  cycles: 8
   *  affects: Z 1 H C
   */
    SUB(r, m, m.imm8(r.PC));
    r.SUB_FLAG = 1;
    return 8;
}
int opcode_0xd7(Registers& r, RAM& m) {
    /*  mnemonic: RST 10H
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    RST(r, m, 0x10);
    return 16;
}
int opcode_0xd8(Registers& r, RAM& m) {
    /*  mnemonic: RET C
   *  length: 1
   *  cycles: 20/8
   *  affects: - - - -
   */
    return 20 / 8;
}
int opcode_0xd9(Registers& r, RAM& m) {
    /*  mnemonic: RETI
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    RETI(r, m);
    return 16;
}
int opcode_0xda(Registers& r, RAM& m) {
    /*  mnemonic: JP C,a16
   *  length: 3
   *  cycles: 16/12
   *  affects: - - - -
   */
    return JP(r, m, r.C, m.imm16(r.PC), 16, 12);
}
int opcode_0xdb(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xdc(Registers& r, RAM& m) {
    /*  mnemonic: CALL C,a16
   *  length: 3
   *  cycles: 24/12
   *  affects: - - - -
   */
    return CALL(r, m, r.C, m.imm16(r.PC), 24, 12);
}
int opcode_0xdd(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xde(Registers& r, RAM& m) {
    /*  mnemonic: SBC A,d8
   *  length: 2
   *  cycles: 8
   *  affects: Z 1 H C
   */
    SBC(r, m, r.A, m.imm8(r.PC));
    r.SUB_FLAG = 1;
    return 8;
}
int opcode_0xdf(Registers& r, RAM& m) {
    /*  mnemonic: RST 18H
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    RST(r, m, 0x18);
    return 16;
}
int opcode_0xe0(Registers& r, RAM& m) {
    /*  mnemonic: LDH (a8),A
   *  length: 2
   *  cycles: 12
   *  affects: - - - -
   */
    return 12;
}
int opcode_0xe1(Registers& r, RAM& m) {
    /*  mnemonic: POP HL
   *  length: 1
   *  cycles: 12
   *  affects: - - - -
   */
    POP(r, m, r.HL);
    return 12;
}
int opcode_0xe2(Registers& r, RAM& m) {
    /*  mnemonic: LD (C),A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, m[r.C], r.A);
    return 8;
}
int opcode_0xe3(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xe4(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xe5(Registers& r, RAM& m) {
    /*  mnemonic: PUSH HL
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    PUSH(r, m, r.HL);
    return 16;
}
int opcode_0xe6(Registers& r, RAM& m) {
    /*  mnemonic: AND d8
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 0
   */
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xe7(Registers& r, RAM& m) {
    /*  mnemonic: RST 20H
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    RST(r, m, 0x20);
    return 16;
}
int opcode_0xe8(Registers& r, RAM& m) {
    /*  mnemonic: ADD SP,r8
   *  length: 2
   *  cycles: 16
   *  affects: 0 0 H C
   */
    ADD(r, m, r.SP, m.imm8(r.PC));
    r.ZERO_FLAG = 0;
    r.SUB_FLAG  = 0;
    return 16;
}
int opcode_0xe9(Registers& r, RAM& m) {
    /*  mnemonic: JP (HL)
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    JP(r, m, m[r.HL]);
    return 4;
}
int opcode_0xea(Registers& r, RAM& m) {
    /*  mnemonic: LD (a16),A
   *  length: 3
   *  cycles: 16
   *  affects: - - - -
   */
    LD(r, m, m[m.imm16(r.PC)], r.A);
    return 16;
}
int opcode_0xeb(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xec(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xed(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xee(Registers& r, RAM& m) {
    /*  mnemonic: XOR d8
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    XOR(r, m, m.imm8(r.PC));
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xef(Registers& r, RAM& m) {
    /*  mnemonic: RST 28H
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    RST(r, m, 0x28);
    return 16;
}
int opcode_0xf0(Registers& r, RAM& m) {
    /*  mnemonic: LDH A,(a8)
   *  length: 2
   *  cycles: 12
   *  affects: - - - -
   */
    return 12;
}
int opcode_0xf1(Registers& r, RAM& m) {
    /*  mnemonic: POP AF
   *  length: 1
   *  cycles: 12
   *  affects: Z N H C
   */
    POP(r, m, r.AF);
    return 12;
}
int opcode_0xf2(Registers& r, RAM& m) {
    /*  mnemonic: LD A,(C)
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.A, m[r.C]);
    return 8;
}
int opcode_0xf3(Registers& r, RAM& m) {
    /*  mnemonic: DI
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    DI(r, m);
    return 4;
}
int opcode_0xf4(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xf5(Registers& r, RAM& m) {
    /*  mnemonic: PUSH AF
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    PUSH(r, m, r.AF);
    return 16;
}
int opcode_0xf6(Registers& r, RAM& m) {
    /*  mnemonic: OR d8
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    OR(r, m, m.imm8(r.PC));
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xf7(Registers& r, RAM& m) {
    /*  mnemonic: RST 30H
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    RST(r, m, 0x30);
    return 16;
}
int opcode_0xf8(Registers& r, RAM& m) {
    /*  mnemonic: LD HL,SP+r8
   *  length: 2
   *  cycles: 12
   *  affects: 0 0 H C
   */
    LD(r, m, r.HL, r.SP + m.imm8(r.PC));
    r.ZERO_FLAG = 0;
    r.SUB_FLAG  = 0;
    return 12;
}
int opcode_0xf9(Registers& r, RAM& m) {
    /*  mnemonic: LD SP,HL
   *  length: 1
   *  cycles: 8
   *  affects: - - - -
   */
    LD(r, m, r.SP, r.HL);
    return 8;
}
int opcode_0xfa(Registers& r, RAM& m) {
    /*  mnemonic: LD A,(a16)
   *  length: 3
   *  cycles: 16
   *  affects: - - - -
   */
    LD(r, m, r.A, m[m.imm16(r.PC)]);
    return 16;
}
int opcode_0xfb(Registers& r, RAM& m) {
    /*  mnemonic: EI
   *  length: 1
   *  cycles: 4
   *  affects: - - - -
   */
    EI(r, m);
    return 4;
}
int opcode_0xfc(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xfd(Registers& r, RAM& m) { throw std::runtime_error("invalid instruction"); }
int opcode_0xfe(Registers& r, RAM& m) {
    /*  mnemonic: CP d8
   *  length: 2
   *  cycles: 8
   *  affects: Z 1 H C
   */
    CP(r, m, m.imm8(r.PC));
    r.SUB_FLAG = 1;
    return 8;
}
int opcode_0xff(Registers& r, RAM& m) {
    /*  mnemonic: RST 38H
   *  length: 1
   *  cycles: 16
   *  affects: - - - -
   */
    RST(r, m, 0x38);
    return 16;
}
int opcode_0xcb00(Registers& r, RAM& m) {
    /*  mnemonic: RLC B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RLC(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb01(Registers& r, RAM& m) {
    /*  mnemonic: RLC C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RLC(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb02(Registers& r, RAM& m) {
    /*  mnemonic: RLC D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RLC(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb03(Registers& r, RAM& m) {
    /*  mnemonic: RLC E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RLC(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb04(Registers& r, RAM& m) {
    /*  mnemonic: RLC H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RLC(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb05(Registers& r, RAM& m) {
    /*  mnemonic: RLC L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RLC(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb06(Registers& r, RAM& m) {
    /*  mnemonic: RLC (HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 0 C
   */
    RLC(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb07(Registers& r, RAM& m) {
    /*  mnemonic: RLC A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RLC(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb08(Registers& r, RAM& m) {
    /*  mnemonic: RRC B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RRC(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb09(Registers& r, RAM& m) {
    /*  mnemonic: RRC C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RRC(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0a(Registers& r, RAM& m) {
    /*  mnemonic: RRC D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RRC(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0b(Registers& r, RAM& m) {
    /*  mnemonic: RRC E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RRC(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0c(Registers& r, RAM& m) {
    /*  mnemonic: RRC H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RRC(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0d(Registers& r, RAM& m) {
    /*  mnemonic: RRC L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RRC(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0e(Registers& r, RAM& m) {
    /*  mnemonic: RRC (HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 0 C
   */
    RRC(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb0f(Registers& r, RAM& m) {
    /*  mnemonic: RRC A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RRC(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb10(Registers& r, RAM& m) {
    /*  mnemonic: RL B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RL(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb11(Registers& r, RAM& m) {
    /*  mnemonic: RL C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RL(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb12(Registers& r, RAM& m) {
    /*  mnemonic: RL D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RL(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb13(Registers& r, RAM& m) {
    /*  mnemonic: RL E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RL(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb14(Registers& r, RAM& m) {
    /*  mnemonic: RL H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RL(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb15(Registers& r, RAM& m) {
    /*  mnemonic: RL L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RL(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb16(Registers& r, RAM& m) {
    /*  mnemonic: RL (HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 0 C
   */
    RL(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb17(Registers& r, RAM& m) {
    /*  mnemonic: RL A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RL(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb18(Registers& r, RAM& m) {
    /*  mnemonic: RR B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RR(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb19(Registers& r, RAM& m) {
    /*  mnemonic: RR C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RR(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1a(Registers& r, RAM& m) {
    /*  mnemonic: RR D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RR(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1b(Registers& r, RAM& m) {
    /*  mnemonic: RR E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RR(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1c(Registers& r, RAM& m) {
    /*  mnemonic: RR H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RR(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1d(Registers& r, RAM& m) {
    /*  mnemonic: RR L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RR(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1e(Registers& r, RAM& m) {
    /*  mnemonic: RR (HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 0 C
   */
    RR(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb1f(Registers& r, RAM& m) {
    /*  mnemonic: RR A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    RR(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb20(Registers& r, RAM& m) {
    /*  mnemonic: SLA B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SLA(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb21(Registers& r, RAM& m) {
    /*  mnemonic: SLA C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SLA(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb22(Registers& r, RAM& m) {
    /*  mnemonic: SLA D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SLA(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb23(Registers& r, RAM& m) {
    /*  mnemonic: SLA E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SLA(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb24(Registers& r, RAM& m) {
    /*  mnemonic: SLA H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SLA(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb25(Registers& r, RAM& m) {
    /*  mnemonic: SLA L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SLA(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb26(Registers& r, RAM& m) {
    /*  mnemonic: SLA (HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 0 C
   */
    SLA(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb27(Registers& r, RAM& m) {
    /*  mnemonic: SLA A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SLA(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb28(Registers& r, RAM& m) {
    /*  mnemonic: SRA B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SRA(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb29(Registers& r, RAM& m) {
    /*  mnemonic: SRA C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SRA(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb2a(Registers& r, RAM& m) {
    /*  mnemonic: SRA D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SRA(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb2b(Registers& r, RAM& m) {
    /*  mnemonic: SRA E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SRA(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb2c(Registers& r, RAM& m) {
    /*  mnemonic: SRA H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SRA(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb2d(Registers& r, RAM& m) {
    /*  mnemonic: SRA L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SRA(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb2e(Registers& r, RAM& m) {
    /*  mnemonic: SRA (HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 0 0
   */
    SRA(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 16;
}
int opcode_0xcb2f(Registers& r, RAM& m) {
    /*  mnemonic: SRA A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SRA(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb30(Registers& r, RAM& m) {
    /*  mnemonic: SWAP B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SWAP(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb31(Registers& r, RAM& m) {
    /*  mnemonic: SWAP C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SWAP(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb32(Registers& r, RAM& m) {
    /*  mnemonic: SWAP D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SWAP(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb33(Registers& r, RAM& m) {
    /*  mnemonic: SWAP E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SWAP(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb34(Registers& r, RAM& m) {
    /*  mnemonic: SWAP H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SWAP(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb35(Registers& r, RAM& m) {
    /*  mnemonic: SWAP L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SWAP(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb36(Registers& r, RAM& m) {
    /*  mnemonic: SWAP (HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 0 0
   */
    SWAP(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 16;
}
int opcode_0xcb37(Registers& r, RAM& m) {
    /*  mnemonic: SWAP A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 0
   */
    SWAP(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb38(Registers& r, RAM& m) {
    /*  mnemonic: SRL B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SRL(r, m, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb39(Registers& r, RAM& m) {
    /*  mnemonic: SRL C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SRL(r, m, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3a(Registers& r, RAM& m) {
    /*  mnemonic: SRL D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SRL(r, m, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3b(Registers& r, RAM& m) {
    /*  mnemonic: SRL E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SRL(r, m, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3c(Registers& r, RAM& m) {
    /*  mnemonic: SRL H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SRL(r, m, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3d(Registers& r, RAM& m) {
    /*  mnemonic: SRL L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SRL(r, m, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3e(Registers& r, RAM& m) {
    /*  mnemonic: SRL (HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 0 C
   */
    SRL(r, m, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb3f(Registers& r, RAM& m) {
    /*  mnemonic: SRL A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 0 C
   */
    SRL(r, m, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb40(Registers& r, RAM& m) {
    /*  mnemonic: BIT 0,B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 0, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb41(Registers& r, RAM& m) {
    /*  mnemonic: BIT 0,C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 0, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb42(Registers& r, RAM& m) {
    /*  mnemonic: BIT 0,D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 0, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb43(Registers& r, RAM& m) {
    /*  mnemonic: BIT 0,E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 0, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb44(Registers& r, RAM& m) {
    /*  mnemonic: BIT 0,H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 0, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb45(Registers& r, RAM& m) {
    /*  mnemonic: BIT 0,L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 0, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb46(Registers& r, RAM& m) {
    /*  mnemonic: BIT 0,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 1 -
   */
    BIT(r, m, 0, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb47(Registers& r, RAM& m) {
    /*  mnemonic: BIT 0,A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 0, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb48(Registers& r, RAM& m) {
    /*  mnemonic: BIT 1,B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 1, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb49(Registers& r, RAM& m) {
    /*  mnemonic: BIT 1,C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 1, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4a(Registers& r, RAM& m) {
    /*  mnemonic: BIT 1,D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 1, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4b(Registers& r, RAM& m) {
    /*  mnemonic: BIT 1,E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 1, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4c(Registers& r, RAM& m) {
    /*  mnemonic: BIT 1,H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 1, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4d(Registers& r, RAM& m) {
    /*  mnemonic: BIT 1,L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 1, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4e(Registers& r, RAM& m) {
    /*  mnemonic: BIT 1,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 1 -
   */
    BIT(r, m, 1, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb4f(Registers& r, RAM& m) {
    /*  mnemonic: BIT 1,A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 1, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb50(Registers& r, RAM& m) {
    /*  mnemonic: BIT 2,B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 2, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb51(Registers& r, RAM& m) {
    /*  mnemonic: BIT 2,C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 2, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb52(Registers& r, RAM& m) {
    /*  mnemonic: BIT 2,D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 2, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb53(Registers& r, RAM& m) {
    /*  mnemonic: BIT 2,E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 2, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb54(Registers& r, RAM& m) {
    /*  mnemonic: BIT 2,H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 2, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb55(Registers& r, RAM& m) {
    /*  mnemonic: BIT 2,L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 2, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb56(Registers& r, RAM& m) {
    /*  mnemonic: BIT 2,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 1 -
   */
    BIT(r, m, 2, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb57(Registers& r, RAM& m) {
    /*  mnemonic: BIT 2,A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 2, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb58(Registers& r, RAM& m) {
    /*  mnemonic: BIT 3,B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 3, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb59(Registers& r, RAM& m) {
    /*  mnemonic: BIT 3,C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 3, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5a(Registers& r, RAM& m) {
    /*  mnemonic: BIT 3,D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 3, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5b(Registers& r, RAM& m) {
    /*  mnemonic: BIT 3,E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 3, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5c(Registers& r, RAM& m) {
    /*  mnemonic: BIT 3,H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 3, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5d(Registers& r, RAM& m) {
    /*  mnemonic: BIT 3,L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 3, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5e(Registers& r, RAM& m) {
    /*  mnemonic: BIT 3,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 1 -
   */
    BIT(r, m, 3, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb5f(Registers& r, RAM& m) {
    /*  mnemonic: BIT 3,A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 3, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb60(Registers& r, RAM& m) {
    /*  mnemonic: BIT 4,B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 4, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb61(Registers& r, RAM& m) {
    /*  mnemonic: BIT 4,C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 4, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb62(Registers& r, RAM& m) {
    /*  mnemonic: BIT 4,D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 4, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb63(Registers& r, RAM& m) {
    /*  mnemonic: BIT 4,E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 4, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb64(Registers& r, RAM& m) {
    /*  mnemonic: BIT 4,H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 4, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb65(Registers& r, RAM& m) {
    /*  mnemonic: BIT 4,L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 4, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb66(Registers& r, RAM& m) {
    /*  mnemonic: BIT 4,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 1 -
   */
    BIT(r, m, 4, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb67(Registers& r, RAM& m) {
    /*  mnemonic: BIT 4,A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 4, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb68(Registers& r, RAM& m) {
    /*  mnemonic: BIT 5,B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 5, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb69(Registers& r, RAM& m) {
    /*  mnemonic: BIT 5,C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 5, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6a(Registers& r, RAM& m) {
    /*  mnemonic: BIT 5,D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 5, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6b(Registers& r, RAM& m) {
    /*  mnemonic: BIT 5,E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 5, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6c(Registers& r, RAM& m) {
    /*  mnemonic: BIT 5,H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 5, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6d(Registers& r, RAM& m) {
    /*  mnemonic: BIT 5,L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 5, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6e(Registers& r, RAM& m) {
    /*  mnemonic: BIT 5,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 1 -
   */
    BIT(r, m, 5, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb6f(Registers& r, RAM& m) {
    /*  mnemonic: BIT 5,A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 5, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb70(Registers& r, RAM& m) {
    /*  mnemonic: BIT 6,B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 6, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb71(Registers& r, RAM& m) {
    /*  mnemonic: BIT 6,C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 6, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb72(Registers& r, RAM& m) {
    /*  mnemonic: BIT 6,D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 6, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb73(Registers& r, RAM& m) {
    /*  mnemonic: BIT 6,E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 6, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb74(Registers& r, RAM& m) {
    /*  mnemonic: BIT 6,H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 6, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb75(Registers& r, RAM& m) {
    /*  mnemonic: BIT 6,L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 6, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb76(Registers& r, RAM& m) {
    /*  mnemonic: BIT 6,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 1 -
   */
    BIT(r, m, 6, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb77(Registers& r, RAM& m) {
    /*  mnemonic: BIT 6,A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 6, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb78(Registers& r, RAM& m) {
    /*  mnemonic: BIT 7,B
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 7, r.B);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb79(Registers& r, RAM& m) {
    /*  mnemonic: BIT 7,C
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 7, r.C);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7a(Registers& r, RAM& m) {
    /*  mnemonic: BIT 7,D
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 7, r.D);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7b(Registers& r, RAM& m) {
    /*  mnemonic: BIT 7,E
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 7, r.E);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7c(Registers& r, RAM& m) {
    /*  mnemonic: BIT 7,H
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 7, r.H);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7d(Registers& r, RAM& m) {
    /*  mnemonic: BIT 7,L
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 7, r.L);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7e(Registers& r, RAM& m) {
    /*  mnemonic: BIT 7,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: Z 0 1 -
   */
    BIT(r, m, 7, m[r.HL]);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb7f(Registers& r, RAM& m) {
    /*  mnemonic: BIT 7,A
   *  length: 2
   *  cycles: 8
   *  affects: Z 0 1 -
   */
    BIT(r, m, 7, r.A);
    r.SUB_FLAG       = 0;
    r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb80(Registers& r, RAM& m) {
    /*  mnemonic: RES 0,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 0, r.B);
    return 8;
}
int opcode_0xcb81(Registers& r, RAM& m) {
    /*  mnemonic: RES 0,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 0, r.C);
    return 8;
}
int opcode_0xcb82(Registers& r, RAM& m) {
    /*  mnemonic: RES 0,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 0, r.D);
    return 8;
}
int opcode_0xcb83(Registers& r, RAM& m) {
    /*  mnemonic: RES 0,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 0, r.E);
    return 8;
}
int opcode_0xcb84(Registers& r, RAM& m) {
    /*  mnemonic: RES 0,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 0, r.H);
    return 8;
}
int opcode_0xcb85(Registers& r, RAM& m) {
    /*  mnemonic: RES 0,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 0, r.L);
    return 8;
}
int opcode_0xcb86(Registers& r, RAM& m) {
    /*  mnemonic: RES 0,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    RES(r, m, 0, m[r.HL]);
    return 16;
}
int opcode_0xcb87(Registers& r, RAM& m) {
    /*  mnemonic: RES 0,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 0, r.A);
    return 8;
}
int opcode_0xcb88(Registers& r, RAM& m) {
    /*  mnemonic: RES 1,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 1, r.B);
    return 8;
}
int opcode_0xcb89(Registers& r, RAM& m) {
    /*  mnemonic: RES 1,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 1, r.C);
    return 8;
}
int opcode_0xcb8a(Registers& r, RAM& m) {
    /*  mnemonic: RES 1,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 1, r.D);
    return 8;
}
int opcode_0xcb8b(Registers& r, RAM& m) {
    /*  mnemonic: RES 1,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 1, r.E);
    return 8;
}
int opcode_0xcb8c(Registers& r, RAM& m) {
    /*  mnemonic: RES 1,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 1, r.H);
    return 8;
}
int opcode_0xcb8d(Registers& r, RAM& m) {
    /*  mnemonic: RES 1,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 1, r.L);
    return 8;
}
int opcode_0xcb8e(Registers& r, RAM& m) {
    /*  mnemonic: RES 1,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    RES(r, m, 1, m[r.HL]);
    return 16;
}
int opcode_0xcb8f(Registers& r, RAM& m) {
    /*  mnemonic: RES 1,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 1, r.A);
    return 8;
}
int opcode_0xcb90(Registers& r, RAM& m) {
    /*  mnemonic: RES 2,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 2, r.B);
    return 8;
}
int opcode_0xcb91(Registers& r, RAM& m) {
    /*  mnemonic: RES 2,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 2, r.C);
    return 8;
}
int opcode_0xcb92(Registers& r, RAM& m) {
    /*  mnemonic: RES 2,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 2, r.D);
    return 8;
}
int opcode_0xcb93(Registers& r, RAM& m) {
    /*  mnemonic: RES 2,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 2, r.E);
    return 8;
}
int opcode_0xcb94(Registers& r, RAM& m) {
    /*  mnemonic: RES 2,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 2, r.H);
    return 8;
}
int opcode_0xcb95(Registers& r, RAM& m) {
    /*  mnemonic: RES 2,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 2, r.L);
    return 8;
}
int opcode_0xcb96(Registers& r, RAM& m) {
    /*  mnemonic: RES 2,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    RES(r, m, 2, m[r.HL]);
    return 16;
}
int opcode_0xcb97(Registers& r, RAM& m) {
    /*  mnemonic: RES 2,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 2, r.A);
    return 8;
}
int opcode_0xcb98(Registers& r, RAM& m) {
    /*  mnemonic: RES 3,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 3, r.B);
    return 8;
}
int opcode_0xcb99(Registers& r, RAM& m) {
    /*  mnemonic: RES 3,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 3, r.C);
    return 8;
}
int opcode_0xcb9a(Registers& r, RAM& m) {
    /*  mnemonic: RES 3,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 3, r.D);
    return 8;
}
int opcode_0xcb9b(Registers& r, RAM& m) {
    /*  mnemonic: RES 3,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 3, r.E);
    return 8;
}
int opcode_0xcb9c(Registers& r, RAM& m) {
    /*  mnemonic: RES 3,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 3, r.H);
    return 8;
}
int opcode_0xcb9d(Registers& r, RAM& m) {
    /*  mnemonic: RES 3,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 3, r.L);
    return 8;
}
int opcode_0xcb9e(Registers& r, RAM& m) {
    /*  mnemonic: RES 3,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    RES(r, m, 3, m[r.HL]);
    return 16;
}
int opcode_0xcb9f(Registers& r, RAM& m) {
    /*  mnemonic: RES 3,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 3, r.A);
    return 8;
}
int opcode_0xcba0(Registers& r, RAM& m) {
    /*  mnemonic: RES 4,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 4, r.B);
    return 8;
}
int opcode_0xcba1(Registers& r, RAM& m) {
    /*  mnemonic: RES 4,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 4, r.C);
    return 8;
}
int opcode_0xcba2(Registers& r, RAM& m) {
    /*  mnemonic: RES 4,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 4, r.D);
    return 8;
}
int opcode_0xcba3(Registers& r, RAM& m) {
    /*  mnemonic: RES 4,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 4, r.E);
    return 8;
}
int opcode_0xcba4(Registers& r, RAM& m) {
    /*  mnemonic: RES 4,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 4, r.H);
    return 8;
}
int opcode_0xcba5(Registers& r, RAM& m) {
    /*  mnemonic: RES 4,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 4, r.L);
    return 8;
}
int opcode_0xcba6(Registers& r, RAM& m) {
    /*  mnemonic: RES 4,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    RES(r, m, 4, m[r.HL]);
    return 16;
}
int opcode_0xcba7(Registers& r, RAM& m) {
    /*  mnemonic: RES 4,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 4, r.A);
    return 8;
}
int opcode_0xcba8(Registers& r, RAM& m) {
    /*  mnemonic: RES 5,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 5, r.B);
    return 8;
}
int opcode_0xcba9(Registers& r, RAM& m) {
    /*  mnemonic: RES 5,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 5, r.C);
    return 8;
}
int opcode_0xcbaa(Registers& r, RAM& m) {
    /*  mnemonic: RES 5,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 5, r.D);
    return 8;
}
int opcode_0xcbab(Registers& r, RAM& m) {
    /*  mnemonic: RES 5,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 5, r.E);
    return 8;
}
int opcode_0xcbac(Registers& r, RAM& m) {
    /*  mnemonic: RES 5,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 5, r.H);
    return 8;
}
int opcode_0xcbad(Registers& r, RAM& m) {
    /*  mnemonic: RES 5,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 5, r.L);
    return 8;
}
int opcode_0xcbae(Registers& r, RAM& m) {
    /*  mnemonic: RES 5,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    RES(r, m, 5, m[r.HL]);
    return 16;
}
int opcode_0xcbaf(Registers& r, RAM& m) {
    /*  mnemonic: RES 5,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 5, r.A);
    return 8;
}
int opcode_0xcbb0(Registers& r, RAM& m) {
    /*  mnemonic: RES 6,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 6, r.B);
    return 8;
}
int opcode_0xcbb1(Registers& r, RAM& m) {
    /*  mnemonic: RES 6,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 6, r.C);
    return 8;
}
int opcode_0xcbb2(Registers& r, RAM& m) {
    /*  mnemonic: RES 6,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 6, r.D);
    return 8;
}
int opcode_0xcbb3(Registers& r, RAM& m) {
    /*  mnemonic: RES 6,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 6, r.E);
    return 8;
}
int opcode_0xcbb4(Registers& r, RAM& m) {
    /*  mnemonic: RES 6,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 6, r.H);
    return 8;
}
int opcode_0xcbb5(Registers& r, RAM& m) {
    /*  mnemonic: RES 6,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 6, r.L);
    return 8;
}
int opcode_0xcbb6(Registers& r, RAM& m) {
    /*  mnemonic: RES 6,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    RES(r, m, 6, m[r.HL]);
    return 16;
}
int opcode_0xcbb7(Registers& r, RAM& m) {
    /*  mnemonic: RES 6,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 6, r.A);
    return 8;
}
int opcode_0xcbb8(Registers& r, RAM& m) {
    /*  mnemonic: RES 7,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 7, r.B);
    return 8;
}
int opcode_0xcbb9(Registers& r, RAM& m) {
    /*  mnemonic: RES 7,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 7, r.C);
    return 8;
}
int opcode_0xcbba(Registers& r, RAM& m) {
    /*  mnemonic: RES 7,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 7, r.D);
    return 8;
}
int opcode_0xcbbb(Registers& r, RAM& m) {
    /*  mnemonic: RES 7,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 7, r.E);
    return 8;
}
int opcode_0xcbbc(Registers& r, RAM& m) {
    /*  mnemonic: RES 7,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 7, r.H);
    return 8;
}
int opcode_0xcbbd(Registers& r, RAM& m) {
    /*  mnemonic: RES 7,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 7, r.L);
    return 8;
}
int opcode_0xcbbe(Registers& r, RAM& m) {
    /*  mnemonic: RES 7,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    RES(r, m, 7, m[r.HL]);
    return 16;
}
int opcode_0xcbbf(Registers& r, RAM& m) {
    /*  mnemonic: RES 7,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    RES(r, m, 7, r.A);
    return 8;
}
int opcode_0xcbc0(Registers& r, RAM& m) {
    /*  mnemonic: SET 0,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 0, r.B);
    return 8;
}
int opcode_0xcbc1(Registers& r, RAM& m) {
    /*  mnemonic: SET 0,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 0, r.C);
    return 8;
}
int opcode_0xcbc2(Registers& r, RAM& m) {
    /*  mnemonic: SET 0,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 0, r.D);
    return 8;
}
int opcode_0xcbc3(Registers& r, RAM& m) {
    /*  mnemonic: SET 0,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 0, r.E);
    return 8;
}
int opcode_0xcbc4(Registers& r, RAM& m) {
    /*  mnemonic: SET 0,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 0, r.H);
    return 8;
}
int opcode_0xcbc5(Registers& r, RAM& m) {
    /*  mnemonic: SET 0,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 0, r.L);
    return 8;
}
int opcode_0xcbc6(Registers& r, RAM& m) {
    /*  mnemonic: SET 0,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    SET(r, m, 0, m[r.HL]);
    return 16;
}
int opcode_0xcbc7(Registers& r, RAM& m) {
    /*  mnemonic: SET 0,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 0, r.A);
    return 8;
}
int opcode_0xcbc8(Registers& r, RAM& m) {
    /*  mnemonic: SET 1,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 1, r.B);
    return 8;
}
int opcode_0xcbc9(Registers& r, RAM& m) {
    /*  mnemonic: SET 1,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 1, r.C);
    return 8;
}
int opcode_0xcbca(Registers& r, RAM& m) {
    /*  mnemonic: SET 1,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 1, r.D);
    return 8;
}
int opcode_0xcbcb(Registers& r, RAM& m) {
    /*  mnemonic: SET 1,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 1, r.E);
    return 8;
}
int opcode_0xcbcc(Registers& r, RAM& m) {
    /*  mnemonic: SET 1,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 1, r.H);
    return 8;
}
int opcode_0xcbcd(Registers& r, RAM& m) {
    /*  mnemonic: SET 1,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 1, r.L);
    return 8;
}
int opcode_0xcbce(Registers& r, RAM& m) {
    /*  mnemonic: SET 1,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    SET(r, m, 1, m[r.HL]);
    return 16;
}
int opcode_0xcbcf(Registers& r, RAM& m) {
    /*  mnemonic: SET 1,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 1, r.A);
    return 8;
}
int opcode_0xcbd0(Registers& r, RAM& m) {
    /*  mnemonic: SET 2,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 2, r.B);
    return 8;
}
int opcode_0xcbd1(Registers& r, RAM& m) {
    /*  mnemonic: SET 2,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 2, r.C);
    return 8;
}
int opcode_0xcbd2(Registers& r, RAM& m) {
    /*  mnemonic: SET 2,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 2, r.D);
    return 8;
}
int opcode_0xcbd3(Registers& r, RAM& m) {
    /*  mnemonic: SET 2,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 2, r.E);
    return 8;
}
int opcode_0xcbd4(Registers& r, RAM& m) {
    /*  mnemonic: SET 2,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 2, r.H);
    return 8;
}
int opcode_0xcbd5(Registers& r, RAM& m) {
    /*  mnemonic: SET 2,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 2, r.L);
    return 8;
}
int opcode_0xcbd6(Registers& r, RAM& m) {
    /*  mnemonic: SET 2,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    SET(r, m, 2, m[r.HL]);
    return 16;
}
int opcode_0xcbd7(Registers& r, RAM& m) {
    /*  mnemonic: SET 2,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 2, r.A);
    return 8;
}
int opcode_0xcbd8(Registers& r, RAM& m) {
    /*  mnemonic: SET 3,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 3, r.B);
    return 8;
}
int opcode_0xcbd9(Registers& r, RAM& m) {
    /*  mnemonic: SET 3,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 3, r.C);
    return 8;
}
int opcode_0xcbda(Registers& r, RAM& m) {
    /*  mnemonic: SET 3,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 3, r.D);
    return 8;
}
int opcode_0xcbdb(Registers& r, RAM& m) {
    /*  mnemonic: SET 3,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 3, r.E);
    return 8;
}
int opcode_0xcbdc(Registers& r, RAM& m) {
    /*  mnemonic: SET 3,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 3, r.H);
    return 8;
}
int opcode_0xcbdd(Registers& r, RAM& m) {
    /*  mnemonic: SET 3,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 3, r.L);
    return 8;
}
int opcode_0xcbde(Registers& r, RAM& m) {
    /*  mnemonic: SET 3,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    SET(r, m, 3, m[r.HL]);
    return 16;
}
int opcode_0xcbdf(Registers& r, RAM& m) {
    /*  mnemonic: SET 3,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 3, r.A);
    return 8;
}
int opcode_0xcbe0(Registers& r, RAM& m) {
    /*  mnemonic: SET 4,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 4, r.B);
    return 8;
}
int opcode_0xcbe1(Registers& r, RAM& m) {
    /*  mnemonic: SET 4,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 4, r.C);
    return 8;
}
int opcode_0xcbe2(Registers& r, RAM& m) {
    /*  mnemonic: SET 4,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 4, r.D);
    return 8;
}
int opcode_0xcbe3(Registers& r, RAM& m) {
    /*  mnemonic: SET 4,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 4, r.E);
    return 8;
}
int opcode_0xcbe4(Registers& r, RAM& m) {
    /*  mnemonic: SET 4,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 4, r.H);
    return 8;
}
int opcode_0xcbe5(Registers& r, RAM& m) {
    /*  mnemonic: SET 4,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 4, r.L);
    return 8;
}
int opcode_0xcbe6(Registers& r, RAM& m) {
    /*  mnemonic: SET 4,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    SET(r, m, 4, m[r.HL]);
    return 16;
}
int opcode_0xcbe7(Registers& r, RAM& m) {
    /*  mnemonic: SET 4,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 4, r.A);
    return 8;
}
int opcode_0xcbe8(Registers& r, RAM& m) {
    /*  mnemonic: SET 5,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 5, r.B);
    return 8;
}
int opcode_0xcbe9(Registers& r, RAM& m) {
    /*  mnemonic: SET 5,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 5, r.C);
    return 8;
}
int opcode_0xcbea(Registers& r, RAM& m) {
    /*  mnemonic: SET 5,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 5, r.D);
    return 8;
}
int opcode_0xcbeb(Registers& r, RAM& m) {
    /*  mnemonic: SET 5,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 5, r.E);
    return 8;
}
int opcode_0xcbec(Registers& r, RAM& m) {
    /*  mnemonic: SET 5,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 5, r.H);
    return 8;
}
int opcode_0xcbed(Registers& r, RAM& m) {
    /*  mnemonic: SET 5,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 5, r.L);
    return 8;
}
int opcode_0xcbee(Registers& r, RAM& m) {
    /*  mnemonic: SET 5,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    SET(r, m, 5, m[r.HL]);
    return 16;
}
int opcode_0xcbef(Registers& r, RAM& m) {
    /*  mnemonic: SET 5,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 5, r.A);
    return 8;
}
int opcode_0xcbf0(Registers& r, RAM& m) {
    /*  mnemonic: SET 6,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 6, r.B);
    return 8;
}
int opcode_0xcbf1(Registers& r, RAM& m) {
    /*  mnemonic: SET 6,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 6, r.C);
    return 8;
}
int opcode_0xcbf2(Registers& r, RAM& m) {
    /*  mnemonic: SET 6,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 6, r.D);
    return 8;
}
int opcode_0xcbf3(Registers& r, RAM& m) {
    /*  mnemonic: SET 6,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 6, r.E);
    return 8;
}
int opcode_0xcbf4(Registers& r, RAM& m) {
    /*  mnemonic: SET 6,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 6, r.H);
    return 8;
}
int opcode_0xcbf5(Registers& r, RAM& m) {
    /*  mnemonic: SET 6,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 6, r.L);
    return 8;
}
int opcode_0xcbf6(Registers& r, RAM& m) {
    /*  mnemonic: SET 6,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    SET(r, m, 6, m[r.HL]);
    return 16;
}
int opcode_0xcbf7(Registers& r, RAM& m) {
    /*  mnemonic: SET 6,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 6, r.A);
    return 8;
}
int opcode_0xcbf8(Registers& r, RAM& m) {
    /*  mnemonic: SET 7,B
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 7, r.B);
    return 8;
}
int opcode_0xcbf9(Registers& r, RAM& m) {
    /*  mnemonic: SET 7,C
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 7, r.C);
    return 8;
}
int opcode_0xcbfa(Registers& r, RAM& m) {
    /*  mnemonic: SET 7,D
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 7, r.D);
    return 8;
}
int opcode_0xcbfb(Registers& r, RAM& m) {
    /*  mnemonic: SET 7,E
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 7, r.E);
    return 8;
}
int opcode_0xcbfc(Registers& r, RAM& m) {
    /*  mnemonic: SET 7,H
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 7, r.H);
    return 8;
}
int opcode_0xcbfd(Registers& r, RAM& m) {
    /*  mnemonic: SET 7,L
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 7, r.L);
    return 8;
}
int opcode_0xcbfe(Registers& r, RAM& m) {
    /*  mnemonic: SET 7,(HL)
   *  length: 2
   *  cycles: 16
   *  affects: - - - -
   */
    SET(r, m, 7, m[r.HL]);
    return 16;
}
int opcode_0xcbff(Registers& r, RAM& m) {
    /*  mnemonic: SET 7,A
   *  length: 2
   *  cycles: 8
   *  affects: - - - -
   */
    SET(r, m, 7, r.A);
    return 8;
}
