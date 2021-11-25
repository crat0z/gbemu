#include "core/gameboy_opcodes.hpp"
#include "core/context.hpp"
#include <bitset>
#include <numeric>
#include <stdexcept>

#define OP_INLINE __attribute__((always_inline)) inline

namespace {

    void set_zero(core::Context& c, auto val) {
        if (val == 0) {
            c.r.ZERO_FLAG = 1;
        }
        else {
            c.r.ZERO_FLAG = 0;
        }
    }

    void OP_INLINE PUSH(core::Context& c, auto value) {
        c.r.SP -= 2;
        c.m.set16(c.r.SP, value);
    }
    void OP_INLINE POP(core::Context& c, auto& out) {
        out = c.read16(c.r.SP);
        c.r.SP += 2;
    }

    void OP_INLINE JP(core::Context& c, auto target) { c.r.PC = target; }

    void OP_INLINE LD(auto& first, auto second) { first = second; }
    void OP_INLINE ADD(core::Context& c, auto& first, auto second) {
        // 8 bit case
        if (sizeof(first) == 1) {
            c.r.HALFCARRY_FLAG = (first & 0xF) + (second & 0xF) > 0x0F;
            c.r.CARRY_FLAG     = (unsigned)first + (unsigned)second > 0xFF;

            first += second;
            set_zero(c, first);
        }
        // 16 bit first operand, two cases
        else {
            // ADD SP, r8
            if (sizeof(second) == 1) {
                c.r.HALFCARRY_FLAG = (first & 0xF) + (second & 0xF) > 0xF;
                c.r.CARRY_FLAG     = ((unsigned)first & 0xFF) + ((unsigned)second & 0xFF) > 0xFF;

                first += second;
            }
            // 16 bit second operand
            else {
                uint16_t test = (first & 0xFFF) + (second & 0xFFF);
                if (test & 0x1000) {
                    c.r.HALFCARRY_FLAG = 1;
                }
                else {
                    c.r.HALFCARRY_FLAG = 0;
                }
                c.r.CARRY_FLAG = ((unsigned)first + (unsigned)second) > 0xFFFF;

                first += second;
            }
        }
    }
    void OP_INLINE SUB(core::Context& c, auto first) {
        // always 8 bit operand
        auto& A = c.r.A;

        c.r.HALFCARRY_FLAG = (A & 0xF) < (first & 0xF);
        c.r.CARRY_FLAG     = A < first;

        A -= first;
        set_zero(c, A);
    }
    void OP_INLINE ADC(core::Context& c, auto& first, auto second) {

        auto orig = first;
        first += second + c.r.CARRY_FLAG;

        set_zero(c, first);
        c.r.HALFCARRY_FLAG = ((orig & 0xF) + (second & 0xF) + c.r.CARRY_FLAG) > 0xF;
        c.r.CARRY_FLAG     = ((unsigned)orig + (unsigned)second + c.r.CARRY_FLAG) > 0xFF;
    }
    void OP_INLINE SBC(core::Context& c, auto& first, auto second) {
        // always 8bit

        auto orig = first;
        first -= (second + c.r.CARRY_FLAG);

        set_zero(c, first);
        c.r.HALFCARRY_FLAG = (orig & 0xF) < (second & 0xF) + c.r.CARRY_FLAG;
        c.r.CARRY_FLAG     = ((unsigned)orig) - ((unsigned)second) - c.r.CARRY_FLAG > 0xFF;
    }
    void OP_INLINE XOR(core::Context& c, auto first) {
        c.r.A ^= first;
        set_zero(c, c.r.A);
    }

    void OP_INLINE AND(core::Context& c, auto first) {
        c.r.A &= first;
        set_zero(c, c.r.A);
    }

    void OP_INLINE OR(core::Context& c, auto first) {
        c.r.A |= first;
        set_zero(c, c.r.A);
    }
    void OP_INLINE INC(core::Context& c, auto& first) {
        // 16 bit case, no flags are set
        if (sizeof(first) == 2) {
            first += 1;
        }
        // 8 bit case
        else {
            // affects Z 0 H -
            c.r.HALFCARRY_FLAG = (first & 0xF) + 1 > 0xF;
            first += 1;
            set_zero(c, first);
        }
    }
    void OP_INLINE DEC(core::Context& c, auto& first) {
        // 16 bit case
        if (sizeof(first) == 2) {
            first -= 1;
        }
        // 8 bit case
        else {
            // affects Z 1 H -
            first -= 1;
            c.r.HALFCARRY_FLAG = (first & 0xF) == 0xF;
            set_zero(c, first);
        }
    }
    void OP_INLINE RLC(core::Context& c, auto& first) {
        c.r.CARRY_FLAG = (first & 0x80) >> 7;

        first = std::rotl(first, 1);

        set_zero(c, first);
    }
    void OP_INLINE RRC(core::Context& c, auto& first) {
        c.r.CARRY_FLAG = first & 0x01;

        first = std::rotr(first, 1);

        set_zero(c, first);
    }
    void OP_INLINE RL(core::Context& c, auto& first) {
        auto flag      = c.r.CARRY_FLAG;
        c.r.CARRY_FLAG = (first & 0x80) >> 7;
        first <<= 1;
        first |= flag;

        set_zero(c, first);
    }
    void OP_INLINE RR(core::Context& c, auto& first) {
        auto flag      = c.r.CARRY_FLAG << 7;
        c.r.CARRY_FLAG = first & 0x01;
        first >>= 1;
        first |= flag;

        set_zero(c, first);
    }
    void OP_INLINE SLA(core::Context& c, auto& first) {
        c.r.CARRY_FLAG = (first & 0x80) >> 7;
        first <<= 1;
        // confirm bit 0 is 0

        set_zero(c, first);
    }
    void OP_INLINE SRA(core::Context& c, auto& first) {
        c.r.CARRY_FLAG = first & 0x01;
        auto msb       = first & 0x80;
        first >>= 1;
        first |= msb;

        set_zero(c, first);
    }
    void OP_INLINE SWAP(core::Context& c, auto& first) {
        auto high = (first & 0xF0) >> 4;
        auto low  = (first & 0x0F) << 4;
        first     = high | low;

        set_zero(c, first);
    }
    void OP_INLINE SRL(core::Context& c, auto& first) {
        c.r.CARRY_FLAG = first & 0x01;
        first >>= 1;

        set_zero(c, first);
    }
    void OP_INLINE BIT(core::Context& c, auto first, auto second) {
        auto val = second & (0x1 << first);

        set_zero(c, val);
    }
    void OP_INLINE RES(auto first, auto& second) {
        auto mask = ~(0x1 << first);
        second &= mask;
    }
    void OP_INLINE SET(auto first, auto& second) {
        auto mask = (0x1 << first);
        second |= mask;
    }
    void OP_INLINE RST(core::Context& c, auto first) {
        PUSH(c, c.r.PC);
        JP(c, first);
    }
    void OP_INLINE RLCA(core::Context& c) {
        auto& A = c.r.A;

        c.r.CARRY_FLAG = (A & 0x80) >> 7;

        A = std::rotl(A, 1);
    }
    void OP_INLINE RRCA(core::Context& c) {
        auto& A = c.r.A;

        c.r.CARRY_FLAG = (A & 0x1);

        A = std::rotr(A, 1);
    }
    void OP_INLINE RLA(core::Context& c) {
        auto& A    = c.r.A;
        auto  flag = c.r.CARRY_FLAG;

        c.r.CARRY_FLAG = (A & 0x80) >> 7;
        A <<= 1;
        A |= flag;
    }
    void OP_INLINE RRA(core::Context& c) {
        auto& A    = c.r.A;
        auto  flag = c.r.CARRY_FLAG << 7;

        c.r.CARRY_FLAG = A & 0x1;
        A >>= 1;
        A |= flag;
    }
    void OP_INLINE DAA(core::Context& c) {
        // the dumbest instruction i've ever seen
        uint16_t result = c.r.A;

        if (c.r.SUB_FLAG) {
            if (c.r.CARRY_FLAG) {
                result -= 0x60;
            }
            if (c.r.HALFCARRY_FLAG) {
                result = (result - 0x06) & 0xFF;
            }
        }
        else {
            if (c.r.CARRY_FLAG || (result > 0x99)) {
                result += 0x60;
                c.r.CARRY_FLAG = 1;
            }

            if (c.r.HALFCARRY_FLAG || ((result & 0xf) > 0x9)) {
                result += 0x6;
            }
        }

        c.r.A = result & 0xFF;

        set_zero(c, c.r.A);
    }
    void OP_INLINE SCF(core::Context& c) { c.r.CARRY_FLAG = 1; }
    void OP_INLINE CPL(core::Context& c) { c.r.A ^= 0xFF; }
    void OP_INLINE CCF(core::Context& c) { c.r.CARRY_FLAG ^= 1; }

    void OP_INLINE CP(core::Context& c, auto first) {

        uint8_t result = c.r.A - first;
        set_zero(c, result);
        c.r.CARRY_FLAG     = c.r.A < first;
        c.r.HALFCARRY_FLAG = (c.r.A & 0xF) < (first & 0xF);
    }

    void OP_INLINE STOP(core::Context& c) {}
    void OP_INLINE RET(core::Context& c) { POP(c, c.r.PC); }
    int OP_INLINE  RET(core::Context& c, auto bit, auto T, auto F) {
        if (bit) {
            RET(c);
            return T;
        }
        return F;
    }
    void OP_INLINE JR(core::Context& c, auto first) { c.r.PC += static_cast<int8_t>(first); }
    int OP_INLINE  JR(core::Context& c, auto bit, auto first, auto T, auto F) {
        if (bit) {
            JR(c, first);
            return T;
        }
        return F;
    }
    int OP_INLINE JP(core::Context& c, auto bit, auto first, auto T, auto F) {
        if (bit) {
            JP(c, first);
            return T;
        }
        return F;
    }
    void OP_INLINE CALL(core::Context& c, auto first) {
        PUSH(c, c.r.PC);
        c.r.PC = first;
    }
    int OP_INLINE CALL(core::Context& c, auto bit, auto first, auto T, auto F) {
        if (bit) {
            CALL(c, first);
            return T;
        }
        return F;
    }

    void OP_INLINE EI(core::Context& c) {}
    void OP_INLINE DI(core::Context& c) {}
    void OP_INLINE RETI(core::Context& c) {
        RET(c);
        EI(c);
    }
} // namespace

// create table
std::array<int (*)(core::Context&), 256> create_table() {
    std::array<int (*)(core::Context&), 256> ret;

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

std::array<int (*)(core::Context&), 256> create_cb_table() {
    std::array<int (*)(core::Context&), 256> ret;

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

int opcode_0x00(core::Context& ctx) {
    /*  mnemonic: NOP
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    return 4;
}
int opcode_0x01(core::Context& ctx) {
    /*  mnemonic: LD BC,d16
	*  length: 3
	*  cycles: 12
	*  affects: - - - -
	*/
    LD(ctx.r.BC, ctx.imm16());
    return 12;
}
int opcode_0x02(core::Context& ctx) {
    /*  mnemonic: LD (BC),A
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint16_t temp;
    LD(temp, ctx.r.A);
    ctx.write8(ctx.r.BC, temp);
    return 8;
}
int opcode_0x03(core::Context& ctx) {
    /*  mnemonic: INC BC
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    INC(ctx, ctx.r.BC);
    return 8;
}
int opcode_0x04(core::Context& ctx) {
    /*  mnemonic: INC B
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H -
	*/
    INC(ctx, ctx.r.B);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x05(core::Context& ctx) {
    /*  mnemonic: DEC B
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H -
	*/
    DEC(ctx, ctx.r.B);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x06(core::Context& ctx) {
    /*  mnemonic: LD B,d8
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.B, ctx.imm8());
    return 8;
}
int opcode_0x07(core::Context& ctx) {
    /*  mnemonic: RLCA
	*  length: 1
	*  cycles: 4
	*  affects: 0 0 0 C
	*/
    RLCA(ctx);
    ctx.r.ZERO_FLAG      = 0;
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x08(core::Context& ctx) {
    /*  mnemonic: LD (a16),SP
	*  length: 3
	*  cycles: 20
	*  affects: - - - -
	*/
    uint16_t temp;
    LD(temp, ctx.r.SP);
    ctx.write16(ctx.imm16(), temp);
    return 20;
}
int opcode_0x09(core::Context& ctx) {
    /*  mnemonic: ADD HL,BC
	*  length: 1
	*  cycles: 8
	*  affects: - 0 H C
	*/
    ADD(ctx, ctx.r.HL, ctx.r.BC);
    ctx.r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x0a(core::Context& ctx) {
    /*  mnemonic: LD A,(BC)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.read8(ctx.r.BC));
    return 8;
}
int opcode_0x0b(core::Context& ctx) {
    /*  mnemonic: DEC BC
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    DEC(ctx, ctx.r.BC);
    return 8;
}
int opcode_0x0c(core::Context& ctx) {
    /*  mnemonic: INC C
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H -
	*/
    INC(ctx, ctx.r.C);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x0d(core::Context& ctx) {
    /*  mnemonic: DEC C
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H -
	*/
    DEC(ctx, ctx.r.C);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x0e(core::Context& ctx) {
    /*  mnemonic: LD C,d8
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.C, ctx.imm8());
    return 8;
}
int opcode_0x0f(core::Context& ctx) {
    /*  mnemonic: RRCA
	*  length: 1
	*  cycles: 4
	*  affects: 0 0 0 C
	*/
    RRCA(ctx);
    ctx.r.ZERO_FLAG      = 0;
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x10(core::Context& ctx) {
    /*  mnemonic: STOP 0
	*  length: 2
	*  cycles: 4
	*  affects: - - - -
	*/
    STOP(ctx);
    return 4;
}
int opcode_0x11(core::Context& ctx) {
    /*  mnemonic: LD DE,d16
	*  length: 3
	*  cycles: 12
	*  affects: - - - -
	*/
    LD(ctx.r.DE, ctx.imm16());
    return 12;
}
int opcode_0x12(core::Context& ctx) {
    /*  mnemonic: LD (DE),A
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.A);
    ctx.write8(ctx.r.DE, temp);
    return 8;
}
int opcode_0x13(core::Context& ctx) {
    /*  mnemonic: INC DE
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    INC(ctx, ctx.r.DE);
    return 8;
}
int opcode_0x14(core::Context& ctx) {
    /*  mnemonic: INC D
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H -
	*/
    INC(ctx, ctx.r.D);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x15(core::Context& ctx) {
    /*  mnemonic: DEC D
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H -
	*/
    DEC(ctx, ctx.r.D);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x16(core::Context& ctx) {
    /*  mnemonic: LD D,d8
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.D, ctx.imm8());
    return 8;
}
int opcode_0x17(core::Context& ctx) {
    /*  mnemonic: RLA
	*  length: 1
	*  cycles: 4
	*  affects: 0 0 0 C
	*/
    RLA(ctx);
    ctx.r.ZERO_FLAG      = 0;
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x18(core::Context& ctx) {
    /*  mnemonic: JR r8
	*  length: 2
	*  cycles: 12
	*  affects: - - - -
	*/
    JR(ctx, ctx.imm8_signed());
    return 12;
}
int opcode_0x19(core::Context& ctx) {
    /*  mnemonic: ADD HL,DE
	*  length: 1
	*  cycles: 8
	*  affects: - 0 H C
	*/
    ADD(ctx, ctx.r.HL, ctx.r.DE);
    ctx.r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x1a(core::Context& ctx) {
    /*  mnemonic: LD A,(DE)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.read8(ctx.r.DE));
    return 8;
}
int opcode_0x1b(core::Context& ctx) {
    /*  mnemonic: DEC DE
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    DEC(ctx, ctx.r.DE);
    return 8;
}
int opcode_0x1c(core::Context& ctx) {
    /*  mnemonic: INC E
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H -
	*/
    INC(ctx, ctx.r.E);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x1d(core::Context& ctx) {
    /*  mnemonic: DEC E
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H -
	*/
    DEC(ctx, ctx.r.E);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x1e(core::Context& ctx) {
    /*  mnemonic: LD E,d8
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.E, ctx.imm8());
    return 8;
}
int opcode_0x1f(core::Context& ctx) {
    /*  mnemonic: RRA
	*  length: 1
	*  cycles: 4
	*  affects: 0 0 0 C
	*/
    RRA(ctx);
    ctx.r.ZERO_FLAG      = 0;
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x20(core::Context& ctx) {
    /*  mnemonic: JR NZ,r8
	*  length: 2
	*  cycles: 12/8
	*  affects: - - - -
	*/
    return JR(ctx, !ctx.r.ZERO_FLAG, ctx.imm8_signed(), 12, 8);
}
int opcode_0x21(core::Context& ctx) {
    /*  mnemonic: LD HL,d16
	*  length: 3
	*  cycles: 12
	*  affects: - - - -
	*/
    LD(ctx.r.HL, ctx.imm16());
    return 12;
}
int opcode_0x22(core::Context& ctx) {
    /*  mnemonic: LD (HL+),A
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.A);
    ctx.write8(ctx.r.HL++, temp);
    return 8;
}
int opcode_0x23(core::Context& ctx) {
    /*  mnemonic: INC HL
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    INC(ctx, ctx.r.HL);
    return 8;
}
int opcode_0x24(core::Context& ctx) {
    /*  mnemonic: INC H
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H -
	*/
    INC(ctx, ctx.r.H);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x25(core::Context& ctx) {
    /*  mnemonic: DEC H
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H -
	*/
    DEC(ctx, ctx.r.H);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x26(core::Context& ctx) {
    /*  mnemonic: LD H,d8
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.H, ctx.imm8());
    return 8;
}
int opcode_0x27(core::Context& ctx) {
    /*  mnemonic: DAA
	*  length: 1
	*  cycles: 4
	*  affects: Z - 0 C
	*/
    DAA(ctx);
    ctx.r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x28(core::Context& ctx) {
    /*  mnemonic: JR Z,r8
	*  length: 2
	*  cycles: 12/8
	*  affects: - - - -
	*/
    return JR(ctx, ctx.r.ZERO_FLAG, ctx.imm8_signed(), 12, 8);
}
int opcode_0x29(core::Context& ctx) {
    /*  mnemonic: ADD HL,HL
	*  length: 1
	*  cycles: 8
	*  affects: - 0 H C
	*/
    ADD(ctx, ctx.r.HL, ctx.r.HL);
    ctx.r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x2a(core::Context& ctx) {
    /*  mnemonic: LD A,(HL+)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.read16(ctx.r.HL++));
    return 8;
}
int opcode_0x2b(core::Context& ctx) {
    /*  mnemonic: DEC HL
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    DEC(ctx, ctx.r.HL);
    return 8;
}
int opcode_0x2c(core::Context& ctx) {
    /*  mnemonic: INC L
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H -
	*/
    INC(ctx, ctx.r.L);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x2d(core::Context& ctx) {
    /*  mnemonic: DEC L
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H -
	*/
    DEC(ctx, ctx.r.L);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x2e(core::Context& ctx) {
    /*  mnemonic: LD L,d8
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.L, ctx.imm8());
    return 8;
}
int opcode_0x2f(core::Context& ctx) {
    /*  mnemonic: CPL
	*  length: 1
	*  cycles: 4
	*  affects: - 1 1 -
	*/
    CPL(ctx);
    ctx.r.SUB_FLAG       = 1;
    ctx.r.HALFCARRY_FLAG = 1;
    return 4;
}
int opcode_0x30(core::Context& ctx) {
    /*  mnemonic: JR NC,r8
	*  length: 2
	*  cycles: 12/8
	*  affects: - - - -
	*/
    return JR(ctx, !ctx.r.CARRY_FLAG, ctx.imm8_signed(), 12, 8);
}
int opcode_0x31(core::Context& ctx) {
    /*  mnemonic: LD SP,d16
	*  length: 3
	*  cycles: 12
	*  affects: - - - -
	*/
    LD(ctx.r.SP, ctx.imm16());
    return 12;
}
int opcode_0x32(core::Context& ctx) {
    /*  mnemonic: LD (HL-),A
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.A);
    ctx.write8(ctx.r.HL--, temp);
    return 8;
}
int opcode_0x33(core::Context& ctx) {
    /*  mnemonic: INC SP
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    INC(ctx, ctx.r.SP);
    return 8;
}
int opcode_0x34(core::Context& ctx) {
    /*  mnemonic: INC (HL)
	*  length: 1
	*  cycles: 12
	*  affects: Z 0 H -
	*/
    uint8_t temp = ctx.read8(ctx.r.HL);
    INC(ctx, temp);
    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG = 0;
    return 12;
}
int opcode_0x35(core::Context& ctx) {
    /*  mnemonic: DEC (HL)
	*  length: 1
	*  cycles: 12
	*  affects: Z 1 H -
	*/
    uint8_t temp = ctx.read8(ctx.r.HL);
    DEC(ctx, temp);
    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG = 1;
    return 12;
}
int opcode_0x36(core::Context& ctx) {
    /*  mnemonic: LD (HL),d8
	*  length: 2
	*  cycles: 12
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.imm8());
    ctx.write8(ctx.r.HL, temp);
    return 12;
}
int opcode_0x37(core::Context& ctx) {
    /*  mnemonic: SCF
	*  length: 1
	*  cycles: 4
	*  affects: - 0 0 1
	*/
    SCF(ctx);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 1;
    return 4;
}
int opcode_0x38(core::Context& ctx) {
    /*  mnemonic: JR C,r8
	*  length: 2
	*  cycles: 12/8
	*  affects: - - - -
	*/
    return JR(ctx, ctx.r.CARRY_FLAG, ctx.imm8_signed(), 12, 8);
}
int opcode_0x39(core::Context& ctx) {
    /*  mnemonic: ADD HL,SP
	*  length: 1
	*  cycles: 8
	*  affects: - 0 H C
	*/
    ADD(ctx, ctx.r.HL, ctx.r.SP);
    ctx.r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x3a(core::Context& ctx) {
    /*  mnemonic: LD A,(HL-)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.read16(ctx.r.HL--));
    return 8;
}
int opcode_0x3b(core::Context& ctx) {
    /*  mnemonic: DEC SP
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    DEC(ctx, ctx.r.SP);
    return 8;
}
int opcode_0x3c(core::Context& ctx) {
    /*  mnemonic: INC A
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H -
	*/
    INC(ctx, ctx.r.A);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x3d(core::Context& ctx) {
    /*  mnemonic: DEC A
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H -
	*/
    DEC(ctx, ctx.r.A);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x3e(core::Context& ctx) {
    /*  mnemonic: LD A,d8
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.imm8());
    return 8;
}
int opcode_0x3f(core::Context& ctx) {
    /*  mnemonic: CCF
	*  length: 1
	*  cycles: 4
	*  affects: - 0 0 C
	*/
    CCF(ctx);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 4;
}
int opcode_0x40(core::Context& ctx) {
    /*  mnemonic: LD B,B
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.B, ctx.r.B);
    return 4;
}
int opcode_0x41(core::Context& ctx) {
    /*  mnemonic: LD B,C
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.B, ctx.r.C);
    return 4;
}
int opcode_0x42(core::Context& ctx) {
    /*  mnemonic: LD B,D
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.B, ctx.r.D);
    return 4;
}
int opcode_0x43(core::Context& ctx) {
    /*  mnemonic: LD B,E
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.B, ctx.r.E);
    return 4;
}
int opcode_0x44(core::Context& ctx) {
    /*  mnemonic: LD B,H
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.B, ctx.r.H);
    return 4;
}
int opcode_0x45(core::Context& ctx) {
    /*  mnemonic: LD B,L
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.B, ctx.r.L);
    return 4;
}
int opcode_0x46(core::Context& ctx) {
    /*  mnemonic: LD B,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.B, ctx.read8(ctx.r.HL));
    return 8;
}
int opcode_0x47(core::Context& ctx) {
    /*  mnemonic: LD B,A
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.B, ctx.r.A);
    return 4;
}
int opcode_0x48(core::Context& ctx) {
    /*  mnemonic: LD C,B
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.C, ctx.r.B);
    return 4;
}
int opcode_0x49(core::Context& ctx) {
    /*  mnemonic: LD C,C
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.C, ctx.r.C);
    return 4;
}
int opcode_0x4a(core::Context& ctx) {
    /*  mnemonic: LD C,D
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.C, ctx.r.D);
    return 4;
}
int opcode_0x4b(core::Context& ctx) {
    /*  mnemonic: LD C,E
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.C, ctx.r.E);
    return 4;
}
int opcode_0x4c(core::Context& ctx) {
    /*  mnemonic: LD C,H
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.C, ctx.r.H);
    return 4;
}
int opcode_0x4d(core::Context& ctx) {
    /*  mnemonic: LD C,L
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.C, ctx.r.L);
    return 4;
}
int opcode_0x4e(core::Context& ctx) {
    /*  mnemonic: LD C,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.C, ctx.read8(ctx.r.HL));
    return 8;
}
int opcode_0x4f(core::Context& ctx) {
    /*  mnemonic: LD C,A
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.C, ctx.r.A);
    return 4;
}
int opcode_0x50(core::Context& ctx) {
    /*  mnemonic: LD D,B
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.D, ctx.r.B);
    return 4;
}
int opcode_0x51(core::Context& ctx) {
    /*  mnemonic: LD D,C
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.D, ctx.r.C);
    return 4;
}
int opcode_0x52(core::Context& ctx) {
    /*  mnemonic: LD D,D
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.D, ctx.r.D);
    return 4;
}
int opcode_0x53(core::Context& ctx) {
    /*  mnemonic: LD D,E
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.D, ctx.r.E);
    return 4;
}
int opcode_0x54(core::Context& ctx) {
    /*  mnemonic: LD D,H
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.D, ctx.r.H);
    return 4;
}
int opcode_0x55(core::Context& ctx) {
    /*  mnemonic: LD D,L
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.D, ctx.r.L);
    return 4;
}
int opcode_0x56(core::Context& ctx) {
    /*  mnemonic: LD D,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.D, ctx.read8(ctx.r.HL));
    return 8;
}
int opcode_0x57(core::Context& ctx) {
    /*  mnemonic: LD D,A
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.D, ctx.r.A);
    return 4;
}
int opcode_0x58(core::Context& ctx) {
    /*  mnemonic: LD E,B
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.E, ctx.r.B);
    return 4;
}
int opcode_0x59(core::Context& ctx) {
    /*  mnemonic: LD E,C
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.E, ctx.r.C);
    return 4;
}
int opcode_0x5a(core::Context& ctx) {
    /*  mnemonic: LD E,D
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.E, ctx.r.D);
    return 4;
}
int opcode_0x5b(core::Context& ctx) {
    /*  mnemonic: LD E,E
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.E, ctx.r.E);
    return 4;
}
int opcode_0x5c(core::Context& ctx) {
    /*  mnemonic: LD E,H
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.E, ctx.r.H);
    return 4;
}
int opcode_0x5d(core::Context& ctx) {
    /*  mnemonic: LD E,L
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.E, ctx.r.L);
    return 4;
}
int opcode_0x5e(core::Context& ctx) {
    /*  mnemonic: LD E,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.E, ctx.read8(ctx.r.HL));
    return 8;
}
int opcode_0x5f(core::Context& ctx) {
    /*  mnemonic: LD E,A
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.E, ctx.r.A);
    return 4;
}
int opcode_0x60(core::Context& ctx) {
    /*  mnemonic: LD H,B
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.H, ctx.r.B);
    return 4;
}
int opcode_0x61(core::Context& ctx) {
    /*  mnemonic: LD H,C
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.H, ctx.r.C);
    return 4;
}
int opcode_0x62(core::Context& ctx) {
    /*  mnemonic: LD H,D
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.H, ctx.r.D);
    return 4;
}
int opcode_0x63(core::Context& ctx) {
    /*  mnemonic: LD H,E
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.H, ctx.r.E);
    return 4;
}
int opcode_0x64(core::Context& ctx) {
    /*  mnemonic: LD H,H
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.H, ctx.r.H);
    return 4;
}
int opcode_0x65(core::Context& ctx) {
    /*  mnemonic: LD H,L
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.H, ctx.r.L);
    return 4;
}
int opcode_0x66(core::Context& ctx) {
    /*  mnemonic: LD H,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.H, ctx.read8(ctx.r.HL));
    return 8;
}
int opcode_0x67(core::Context& ctx) {
    /*  mnemonic: LD H,A
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.H, ctx.r.A);
    return 4;
}
int opcode_0x68(core::Context& ctx) {
    /*  mnemonic: LD L,B
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.L, ctx.r.B);
    return 4;
}
int opcode_0x69(core::Context& ctx) {
    /*  mnemonic: LD L,C
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.L, ctx.r.C);
    return 4;
}
int opcode_0x6a(core::Context& ctx) {
    /*  mnemonic: LD L,D
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.L, ctx.r.D);
    return 4;
}
int opcode_0x6b(core::Context& ctx) {
    /*  mnemonic: LD L,E
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.L, ctx.r.E);
    return 4;
}
int opcode_0x6c(core::Context& ctx) {
    /*  mnemonic: LD L,H
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.L, ctx.r.H);
    return 4;
}
int opcode_0x6d(core::Context& ctx) {
    /*  mnemonic: LD L,L
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.L, ctx.r.L);
    return 4;
}
int opcode_0x6e(core::Context& ctx) {
    /*  mnemonic: LD L,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.L, ctx.read8(ctx.r.HL));
    return 8;
}
int opcode_0x6f(core::Context& ctx) {
    /*  mnemonic: LD L,A
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.L, ctx.r.A);
    return 4;
}
int opcode_0x70(core::Context& ctx) {
    /*  mnemonic: LD (HL),B
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.B);
    ctx.write8(ctx.r.HL, temp);
    return 8;
}
int opcode_0x71(core::Context& ctx) {
    /*  mnemonic: LD (HL),C
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.C);
    ctx.write8(ctx.r.HL, temp);
    return 8;
}
int opcode_0x72(core::Context& ctx) {
    /*  mnemonic: LD (HL),D
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.D);
    ctx.write8(ctx.r.HL, temp);
    return 8;
}
int opcode_0x73(core::Context& ctx) {
    /*  mnemonic: LD (HL),E
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.E);
    ctx.write8(ctx.r.HL, temp);
    return 8;
}
int opcode_0x74(core::Context& ctx) {
    /*  mnemonic: LD (HL),H
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.H);
    ctx.write8(ctx.r.HL, temp);
    return 8;
}
int opcode_0x75(core::Context& ctx) {
    /*  mnemonic: LD (HL),L
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.L);
    ctx.write8(ctx.r.HL, temp);
    return 8;
}
int opcode_0x76(core::Context& ctx) {
    /*  mnemonic: HALT
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    return 4;
}
int opcode_0x77(core::Context& ctx) {
    /*  mnemonic: LD (HL),A
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.A);
    ctx.write8(ctx.r.HL, temp);
    return 8;
}
int opcode_0x78(core::Context& ctx) {
    /*  mnemonic: LD A,B
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.r.B);
    return 4;
}
int opcode_0x79(core::Context& ctx) {
    /*  mnemonic: LD A,C
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.r.C);
    return 4;
}
int opcode_0x7a(core::Context& ctx) {
    /*  mnemonic: LD A,D
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.r.D);
    return 4;
}
int opcode_0x7b(core::Context& ctx) {
    /*  mnemonic: LD A,E
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.r.E);
    return 4;
}
int opcode_0x7c(core::Context& ctx) {
    /*  mnemonic: LD A,H
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.r.H);
    return 4;
}
int opcode_0x7d(core::Context& ctx) {
    /*  mnemonic: LD A,L
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.r.L);
    return 4;
}
int opcode_0x7e(core::Context& ctx) {
    /*  mnemonic: LD A,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.read8(ctx.r.HL));
    return 8;
}
int opcode_0x7f(core::Context& ctx) {
    /*  mnemonic: LD A,A
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.r.A);
    return 4;
}
int opcode_0x80(core::Context& ctx) {
    /*  mnemonic: ADD A,B
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADD(ctx, ctx.r.A, ctx.r.B);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x81(core::Context& ctx) {
    /*  mnemonic: ADD A,C
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADD(ctx, ctx.r.A, ctx.r.C);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x82(core::Context& ctx) {
    /*  mnemonic: ADD A,D
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADD(ctx, ctx.r.A, ctx.r.D);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x83(core::Context& ctx) {
    /*  mnemonic: ADD A,E
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADD(ctx, ctx.r.A, ctx.r.E);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x84(core::Context& ctx) {
    /*  mnemonic: ADD A,H
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADD(ctx, ctx.r.A, ctx.r.H);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x85(core::Context& ctx) {
    /*  mnemonic: ADD A,L
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADD(ctx, ctx.r.A, ctx.r.L);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x86(core::Context& ctx) {
    /*  mnemonic: ADD A,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: Z 0 H C
	*/
    ADD(ctx, ctx.r.A, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x87(core::Context& ctx) {
    /*  mnemonic: ADD A,A
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADD(ctx, ctx.r.A, ctx.r.A);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x88(core::Context& ctx) {
    /*  mnemonic: ADC A,B
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADC(ctx, ctx.r.A, ctx.r.B);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x89(core::Context& ctx) {
    /*  mnemonic: ADC A,C
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADC(ctx, ctx.r.A, ctx.r.C);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8a(core::Context& ctx) {
    /*  mnemonic: ADC A,D
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADC(ctx, ctx.r.A, ctx.r.D);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8b(core::Context& ctx) {
    /*  mnemonic: ADC A,E
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADC(ctx, ctx.r.A, ctx.r.E);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8c(core::Context& ctx) {
    /*  mnemonic: ADC A,H
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADC(ctx, ctx.r.A, ctx.r.H);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8d(core::Context& ctx) {
    /*  mnemonic: ADC A,L
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADC(ctx, ctx.r.A, ctx.r.L);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x8e(core::Context& ctx) {
    /*  mnemonic: ADC A,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: Z 0 H C
	*/
    ADC(ctx, ctx.r.A, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG = 0;
    return 8;
}
int opcode_0x8f(core::Context& ctx) {
    /*  mnemonic: ADC A,A
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 H C
	*/
    ADC(ctx, ctx.r.A, ctx.r.A);
    ctx.r.SUB_FLAG = 0;
    return 4;
}
int opcode_0x90(core::Context& ctx) {
    /*  mnemonic: SUB B
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SUB(ctx, ctx.r.B);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x91(core::Context& ctx) {
    /*  mnemonic: SUB C
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SUB(ctx, ctx.r.C);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x92(core::Context& ctx) {
    /*  mnemonic: SUB D
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SUB(ctx, ctx.r.D);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x93(core::Context& ctx) {
    /*  mnemonic: SUB E
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SUB(ctx, ctx.r.E);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x94(core::Context& ctx) {
    /*  mnemonic: SUB H
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SUB(ctx, ctx.r.H);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x95(core::Context& ctx) {
    /*  mnemonic: SUB L
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SUB(ctx, ctx.r.L);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x96(core::Context& ctx) {
    /*  mnemonic: SUB (HL)
	*  length: 1
	*  cycles: 8
	*  affects: Z 1 H C
	*/
    SUB(ctx, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG = 1;
    return 8;
}
int opcode_0x97(core::Context& ctx) {
    /*  mnemonic: SUB A
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SUB(ctx, ctx.r.A);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x98(core::Context& ctx) {
    /*  mnemonic: SBC A,B
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SBC(ctx, ctx.r.A, ctx.r.B);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x99(core::Context& ctx) {
    /*  mnemonic: SBC A,C
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SBC(ctx, ctx.r.A, ctx.r.C);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9a(core::Context& ctx) {
    /*  mnemonic: SBC A,D
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SBC(ctx, ctx.r.A, ctx.r.D);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9b(core::Context& ctx) {
    /*  mnemonic: SBC A,E
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SBC(ctx, ctx.r.A, ctx.r.E);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9c(core::Context& ctx) {
    /*  mnemonic: SBC A,H
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SBC(ctx, ctx.r.A, ctx.r.H);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9d(core::Context& ctx) {
    /*  mnemonic: SBC A,L
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SBC(ctx, ctx.r.A, ctx.r.L);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0x9e(core::Context& ctx) {
    /*  mnemonic: SBC A,(HL)
	*  length: 1
	*  cycles: 8
	*  affects: Z 1 H C
	*/
    SBC(ctx, ctx.r.A, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG = 1;
    return 8;
}
int opcode_0x9f(core::Context& ctx) {
    /*  mnemonic: SBC A,A
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    SBC(ctx, ctx.r.A, ctx.r.A);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xa0(core::Context& ctx) {
    /*  mnemonic: AND B
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 1 0
	*/

    AND(ctx, ctx.r.B);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa1(core::Context& ctx) {
    /*  mnemonic: AND C
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 1 0
	*/
    AND(ctx, ctx.r.C);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa2(core::Context& ctx) {
    /*  mnemonic: AND D
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 1 0
	*/
    AND(ctx, ctx.r.D);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa3(core::Context& ctx) {
    /*  mnemonic: AND E
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 1 0
	*/
    AND(ctx, ctx.r.E);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa4(core::Context& ctx) {
    /*  mnemonic: AND H
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 1 0
	*/
    AND(ctx, ctx.r.H);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa5(core::Context& ctx) {
    /*  mnemonic: AND L
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 1 0
	*/
    AND(ctx, ctx.r.L);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa6(core::Context& ctx) {
    /*  mnemonic: AND (HL)
	*  length: 1
	*  cycles: 8
	*  affects: Z 0 1 0
	*/
    AND(ctx, ctx.read8(ctx.r.HL));

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xa7(core::Context& ctx) {
    /*  mnemonic: AND A
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 1 0
	*/
    AND(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa8(core::Context& ctx) {
    /*  mnemonic: XOR B
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    XOR(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xa9(core::Context& ctx) {
    /*  mnemonic: XOR C
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    XOR(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xaa(core::Context& ctx) {
    /*  mnemonic: XOR D
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    XOR(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xab(core::Context& ctx) {
    /*  mnemonic: XOR E
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    XOR(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xac(core::Context& ctx) {
    /*  mnemonic: XOR H
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    XOR(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xad(core::Context& ctx) {
    /*  mnemonic: XOR L
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    XOR(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xae(core::Context& ctx) {
    /*  mnemonic: XOR (HL)
	*  length: 1
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    XOR(ctx, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xaf(core::Context& ctx) {
    /*  mnemonic: XOR A
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    XOR(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb0(core::Context& ctx) {
    /*  mnemonic: OR B
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    OR(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb1(core::Context& ctx) {
    /*  mnemonic: OR C
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    OR(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb2(core::Context& ctx) {
    /*  mnemonic: OR D
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    OR(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb3(core::Context& ctx) {
    /*  mnemonic: OR E
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    OR(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb4(core::Context& ctx) {
    /*  mnemonic: OR H
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    OR(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb5(core::Context& ctx) {
    /*  mnemonic: OR L
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    OR(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb6(core::Context& ctx) {
    /*  mnemonic: OR (HL)
	*  length: 1
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    OR(ctx, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xb7(core::Context& ctx) {
    /*  mnemonic: OR A
	*  length: 1
	*  cycles: 4
	*  affects: Z 0 0 0
	*/
    OR(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 4;
}
int opcode_0xb8(core::Context& ctx) {
    /*  mnemonic: CP B
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    CP(ctx, ctx.r.B);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xb9(core::Context& ctx) {
    /*  mnemonic: CP C
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    CP(ctx, ctx.r.C);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xba(core::Context& ctx) {
    /*  mnemonic: CP D
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    CP(ctx, ctx.r.D);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xbb(core::Context& ctx) {
    /*  mnemonic: CP E
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    CP(ctx, ctx.r.E);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xbc(core::Context& ctx) {
    /*  mnemonic: CP H
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    CP(ctx, ctx.r.H);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xbd(core::Context& ctx) {
    /*  mnemonic: CP L
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    CP(ctx, ctx.r.L);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xbe(core::Context& ctx) {
    /*  mnemonic: CP (HL)
	*  length: 1
	*  cycles: 8
	*  affects: Z 1 H C
	*/
    CP(ctx, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG = 1;
    return 8;
}
int opcode_0xbf(core::Context& ctx) {
    /*  mnemonic: CP A
	*  length: 1
	*  cycles: 4
	*  affects: Z 1 H C
	*/
    CP(ctx, ctx.r.A);
    ctx.r.SUB_FLAG = 1;
    return 4;
}
int opcode_0xc0(core::Context& ctx) {
    /*  mnemonic: RET NZ
	*  length: 1
	*  cycles: 20/8
	*  affects: - - - -
	*/
    return RET(ctx, !ctx.r.ZERO_FLAG, 20, 8);
}
int opcode_0xc1(core::Context& ctx) {
    /*  mnemonic: POP BC
	*  length: 1
	*  cycles: 12
	*  affects: - - - -
	*/
    POP(ctx, ctx.r.BC);
    return 12;
}
int opcode_0xc2(core::Context& ctx) {
    /*  mnemonic: JP NZ,a16
	*  length: 3
	*  cycles: 16/12
	*  affects: - - - -
	*/
    return JP(ctx, !ctx.r.ZERO_FLAG, ctx.imm16(), 16, 12);
}
int opcode_0xc3(core::Context& ctx) {
    /*  mnemonic: JP a16
	*  length: 3
	*  cycles: 16
	*  affects: - - - -
	*/
    JP(ctx, ctx.imm16());
    return 16;
}
int opcode_0xc4(core::Context& ctx) {
    /*  mnemonic: CALL NZ,a16
	*  length: 3
	*  cycles: 24/12
	*  affects: - - - -
	*/
    return CALL(ctx, !ctx.r.ZERO_FLAG, ctx.imm16(), 24, 12);
}
int opcode_0xc5(core::Context& ctx) {
    /*  mnemonic: PUSH BC
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    PUSH(ctx, ctx.r.BC);
    return 16;
}
int opcode_0xc6(core::Context& ctx) {
    /*  mnemonic: ADD A,d8
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 H C
	*/
    ADD(ctx, ctx.r.A, ctx.imm8());
    ctx.r.SUB_FLAG = 0;
    return 8;
}
int opcode_0xc7(core::Context& ctx) {
    /*  mnemonic: RST 00H
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RST(ctx, 0x00);
    return 16;
}
int opcode_0xc8(core::Context& ctx) {
    /*  mnemonic: RET Z
	*  length: 1
	*  cycles: 20/8
	*  affects: - - - -
	*/
    return RET(ctx, ctx.r.ZERO_FLAG, 20, 8);
}
int opcode_0xc9(core::Context& ctx) {
    /*  mnemonic: RET
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RET(ctx);
    return 16;
}
int opcode_0xca(core::Context& ctx) {
    /*  mnemonic: JP Z,a16
	*  length: 3
	*  cycles: 16/12
	*  affects: - - - -
	*/
    return JP(ctx, ctx.r.ZERO_FLAG, ctx.imm16(), 16, 12);
}
int opcode_0xcc(core::Context& ctx) {
    /*  mnemonic: CALL Z,a16
	*  length: 3
	*  cycles: 24/12
	*  affects: - - - -
	*/
    return CALL(ctx, ctx.r.ZERO_FLAG, ctx.imm16(), 24, 12);
}
int opcode_0xcd(core::Context& ctx) {
    /*  mnemonic: CALL a16
	*  length: 3
	*  cycles: 24
	*  affects: - - - -
	*/
    CALL(ctx, ctx.imm16());
    return 24;
}
int opcode_0xce(core::Context& ctx) {
    /*  mnemonic: ADC A,d8
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 H C
	*/
    ADC(ctx, ctx.r.A, ctx.imm8());
    ctx.r.SUB_FLAG = 0;
    return 8;
}
int opcode_0xcf(core::Context& ctx) {
    /*  mnemonic: RST 08H
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RST(ctx, 0x08);
    return 16;
}
int opcode_0xd0(core::Context& ctx) {
    /*  mnemonic: RET NC
	*  length: 1
	*  cycles: 20/8
	*  affects: - - - -
	*/
    return RET(ctx, !ctx.r.CARRY_FLAG, 20, 8);
}
int opcode_0xd1(core::Context& ctx) {
    /*  mnemonic: POP DE
	*  length: 1
	*  cycles: 12
	*  affects: - - - -
	*/
    POP(ctx, ctx.r.DE);
    return 12;
}
int opcode_0xd2(core::Context& ctx) {
    /*  mnemonic: JP NC,a16
	*  length: 3
	*  cycles: 16/12
	*  affects: - - - -
	*/
    return JP(ctx, !ctx.r.CARRY_FLAG, ctx.imm16(), 16, 12);
}
int opcode_0xd3(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xd4(core::Context& ctx) {
    /*  mnemonic: CALL NC,a16
	*  length: 3
	*  cycles: 24/12
	*  affects: - - - -
	*/
    return CALL(ctx, !ctx.r.CARRY_FLAG, ctx.imm16(), 24, 12);
}
int opcode_0xd5(core::Context& ctx) {
    /*  mnemonic: PUSH DE
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    PUSH(ctx, ctx.r.DE);
    return 16;
}
int opcode_0xd6(core::Context& ctx) {
    /*  mnemonic: SUB d8
	*  length: 2
	*  cycles: 8
	*  affects: Z 1 H C
	*/
    SUB(ctx, ctx.imm8());
    ctx.r.SUB_FLAG = 1;
    return 8;
}
int opcode_0xd7(core::Context& ctx) {
    /*  mnemonic: RST 10H
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RST(ctx, 0x10);
    return 16;
}
int opcode_0xd8(core::Context& ctx) {
    /*  mnemonic: RET C
	*  length: 1
	*  cycles: 20/8
	*  affects: - - - -
	*/
    return RET(ctx, ctx.r.CARRY_FLAG, 20, 8);
}
int opcode_0xd9(core::Context& ctx) {
    /*  mnemonic: RETI
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RETI(ctx);
    return 16;
}
int opcode_0xda(core::Context& ctx) {
    /*  mnemonic: JP C,a16
	*  length: 3
	*  cycles: 16/12
	*  affects: - - - -
	*/
    return JP(ctx, ctx.r.CARRY_FLAG, ctx.imm16(), 16, 12);
}
int opcode_0xdb(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xdc(core::Context& ctx) {
    /*  mnemonic: CALL C,a16
	*  length: 3
	*  cycles: 24/12
	*  affects: - - - -
	*/
    return CALL(ctx, ctx.r.CARRY_FLAG, ctx.imm16(), 24, 12);
}
int opcode_0xdd(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xde(core::Context& ctx) {
    /*  mnemonic: SBC A,d8
	*  length: 2
	*  cycles: 8
	*  affects: Z 1 H C
	*/
    SBC(ctx, ctx.r.A, ctx.imm8());
    ctx.r.SUB_FLAG = 1;
    return 8;
}
int opcode_0xdf(core::Context& ctx) {
    /*  mnemonic: RST 18H
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RST(ctx, 0x18);
    return 16;
}
int opcode_0xe0(core::Context& ctx) {
    /*  mnemonic: LDH (a8),A
	*  length: 2
	*  cycles: 12
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.A);
    ctx.write8(0xFF00 + ctx.imm8(), temp);
    return 12;
}
int opcode_0xe1(core::Context& ctx) {
    /*  mnemonic: POP HL
	*  length: 1
	*  cycles: 12
	*  affects: - - - -
	*/
    POP(ctx, ctx.r.HL);
    return 12;
}
int opcode_0xe2(core::Context& ctx) {
    /*  mnemonic: LD (C),A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    uint8_t temp;
    LD(temp, ctx.r.A);
    ctx.write8(0xFF00 + ctx.r.C, temp);
    return 8;
}
int opcode_0xe3(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xe4(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xe5(core::Context& ctx) {
    /*  mnemonic: PUSH HL
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    PUSH(ctx, ctx.r.HL);
    return 16;
}
int opcode_0xe6(core::Context& ctx) {
    /*  mnemonic: AND d8
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 0
	*/
    AND(ctx, ctx.imm8());
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xe7(core::Context& ctx) {
    /*  mnemonic: RST 20H
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RST(ctx, 0x20);
    return 16;
}
int opcode_0xe8(core::Context& ctx) {
    /*  mnemonic: ADD SP,r8
	*  length: 2
	*  cycles: 16
	*  affects: 0 0 H C
	*/
    ADD(ctx, ctx.r.SP, ctx.imm8_signed());
    ctx.r.ZERO_FLAG = 0;
    ctx.r.SUB_FLAG  = 0;
    return 16;
}
int opcode_0xe9(core::Context& ctx) {
    /*  mnemonic: JP (HL)
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    JP(ctx, ctx.r.HL);
    return 4;
}
int opcode_0xea(core::Context& ctx) {
    /*  mnemonic: LD (a16),A
	*  length: 3
	*  cycles: 16
	*  affects: - - - -
	*/

    uint8_t temp;
    LD(temp, ctx.r.A);
    ctx.write8(ctx.imm16(), temp);
    return 16;
}
int opcode_0xeb(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xec(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xed(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xee(core::Context& ctx) {
    /*  mnemonic: XOR d8
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    XOR(ctx, ctx.imm8());
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xef(core::Context& ctx) {
    /*  mnemonic: RST 28H
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RST(ctx, 0x28);
    return 16;
}
int opcode_0xf0(core::Context& ctx) {
    /*  mnemonic: LDH A,(a8)
	*  length: 2
	*  cycles: 12
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.read8(0xFF00 + ctx.imm8()));
    return 12;
}
int opcode_0xf1(core::Context& ctx) {
    /*  mnemonic: POP AF
	*  length: 1
	*  cycles: 12
	*  affects: Z N H C
	*/
    POP(ctx, ctx.r.AF);
    ctx.r.AF &= 0xFFF0;
    return 12;
}
int opcode_0xf2(core::Context& ctx) {
    /*  mnemonic: LD A,(C)
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.read8(0xFF00 + ctx.r.C));
    return 8;
}
int opcode_0xf3(core::Context& ctx) {
    /*  mnemonic: DI
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    DI(ctx);
    return 4;
}
int opcode_0xf4(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xf5(core::Context& ctx) {
    /*  mnemonic: PUSH AF
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    PUSH(ctx, ctx.r.AF);
    return 16;
}
int opcode_0xf6(core::Context& ctx) {
    /*  mnemonic: OR d8
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    OR(ctx, ctx.imm8());
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xf7(core::Context& ctx) {
    /*  mnemonic: RST 30H
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RST(ctx, 0x30);
    return 16;
}
int opcode_0xf8(core::Context& ctx) {
    /*  mnemonic: LD HL,SP+r8
	*  length: 2
	*  cycles: 12
	*  affects: 0 0 H C
	*/
    auto offset = ctx.imm8_signed();

    if (((ctx.r.SP & 0xF) + (offset & 0xF)) > 0xF) {
        ctx.r.HALFCARRY_FLAG = 1;
    }
    else {
        ctx.r.HALFCARRY_FLAG = 0;
    }

    if (((ctx.r.SP & 0xFF) + (offset & 0xFF)) > 0xFF) {
        ctx.r.CARRY_FLAG = 1;
    }
    else {
        ctx.r.CARRY_FLAG = 0;
    }

    LD(ctx.r.HL, ctx.r.SP + offset);
    ctx.r.ZERO_FLAG = 0;
    ctx.r.SUB_FLAG  = 0;
    return 12;
}
int opcode_0xf9(core::Context& ctx) {
    /*  mnemonic: LD SP,HL
	*  length: 1
	*  cycles: 8
	*  affects: - - - -
	*/
    LD(ctx.r.SP, ctx.r.HL);
    return 8;
}
int opcode_0xfa(core::Context& ctx) {
    /*  mnemonic: LD A,(a16)
	*  length: 3
	*  cycles: 16
	*  affects: - - - -
	*/
    LD(ctx.r.A, ctx.read8(ctx.imm16()));
    return 16;
}
int opcode_0xfb(core::Context& ctx) {
    /*  mnemonic: EI
	*  length: 1
	*  cycles: 4
	*  affects: - - - -
	*/
    EI(ctx);
    return 4;
}
int opcode_0xfc(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xfd(core::Context& ctx) { throw std::runtime_error("invalid instruction"); }
int opcode_0xfe(core::Context& ctx) {
    /*  mnemonic: CP d8
	*  length: 2
	*  cycles: 8
	*  affects: Z 1 H C
	*/
    CP(ctx, ctx.imm8());
    ctx.r.SUB_FLAG = 1;
    return 8;
}
int opcode_0xff(core::Context& ctx) {
    /*  mnemonic: RST 38H
	*  length: 1
	*  cycles: 16
	*  affects: - - - -
	*/
    RST(ctx, 0x38);
    return 16;
}
int opcode_0xcb00(core::Context& ctx) {
    /*  mnemonic: RLC B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RLC(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb01(core::Context& ctx) {
    /*  mnemonic: RLC C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RLC(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb02(core::Context& ctx) {
    /*  mnemonic: RLC D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RLC(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb03(core::Context& ctx) {
    /*  mnemonic: RLC E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RLC(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb04(core::Context& ctx) {
    /*  mnemonic: RLC H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RLC(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb05(core::Context& ctx) {
    /*  mnemonic: RLC L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RLC(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb06(core::Context& ctx) {
    /*  mnemonic: RLC (HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 0 C
	*/
    uint8_t temp = ctx.read8(ctx.r.HL);
    RLC(ctx, temp);
    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb07(core::Context& ctx) {
    /*  mnemonic: RLC A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RLC(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb08(core::Context& ctx) {
    /*  mnemonic: RRC B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RRC(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb09(core::Context& ctx) {
    /*  mnemonic: RRC C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RRC(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0a(core::Context& ctx) {
    /*  mnemonic: RRC D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RRC(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0b(core::Context& ctx) {
    /*  mnemonic: RRC E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RRC(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0c(core::Context& ctx) {
    /*  mnemonic: RRC H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RRC(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0d(core::Context& ctx) {
    /*  mnemonic: RRC L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RRC(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb0e(core::Context& ctx) {
    /*  mnemonic: RRC (HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 0 C
	*/
    uint8_t temp = ctx.read8(ctx.r.HL);

    RRC(ctx, temp);

    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb0f(core::Context& ctx) {
    /*  mnemonic: RRC A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RRC(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb10(core::Context& ctx) {
    /*  mnemonic: RL B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RL(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb11(core::Context& ctx) {
    /*  mnemonic: RL C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RL(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb12(core::Context& ctx) {
    /*  mnemonic: RL D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RL(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb13(core::Context& ctx) {
    /*  mnemonic: RL E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RL(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb14(core::Context& ctx) {
    /*  mnemonic: RL H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RL(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb15(core::Context& ctx) {
    /*  mnemonic: RL L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RL(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb16(core::Context& ctx) {
    /*  mnemonic: RL (HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 0 C
	*/
    uint8_t temp = ctx.read8(ctx.r.HL);
    RL(ctx, temp);
    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb17(core::Context& ctx) {
    /*  mnemonic: RL A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RL(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb18(core::Context& ctx) {
    /*  mnemonic: RR B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RR(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb19(core::Context& ctx) {
    /*  mnemonic: RR C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RR(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1a(core::Context& ctx) {
    /*  mnemonic: RR D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RR(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1b(core::Context& ctx) {
    /*  mnemonic: RR E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RR(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1c(core::Context& ctx) {
    /*  mnemonic: RR H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RR(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1d(core::Context& ctx) {
    /*  mnemonic: RR L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RR(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb1e(core::Context& ctx) {
    /*  mnemonic: RR (HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 0 C
	*/
    auto temp = ctx.read8(ctx.r.HL);
    RR(ctx, temp);
    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb1f(core::Context& ctx) {
    /*  mnemonic: RR A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    RR(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb20(core::Context& ctx) {
    /*  mnemonic: SLA B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SLA(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb21(core::Context& ctx) {
    /*  mnemonic: SLA C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SLA(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb22(core::Context& ctx) {
    /*  mnemonic: SLA D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SLA(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb23(core::Context& ctx) {
    /*  mnemonic: SLA E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SLA(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb24(core::Context& ctx) {
    /*  mnemonic: SLA H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SLA(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb25(core::Context& ctx) {
    /*  mnemonic: SLA L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SLA(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb26(core::Context& ctx) {
    /*  mnemonic: SLA (HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 0 C
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SLA(ctx, temp);
    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb27(core::Context& ctx) {
    /*  mnemonic: SLA A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SLA(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb28(core::Context& ctx) {
    /*  mnemonic: SRA B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SRA(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb29(core::Context& ctx) {
    /*  mnemonic: SRA C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SRA(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb2a(core::Context& ctx) {
    /*  mnemonic: SRA D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SRA(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb2b(core::Context& ctx) {
    /*  mnemonic: SRA E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SRA(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb2c(core::Context& ctx) {
    /*  mnemonic: SRA H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SRA(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb2d(core::Context& ctx) {
    /*  mnemonic: SRA L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SRA(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb2e(core::Context& ctx) {
    /*  mnemonic: SRA (HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 0 0
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SRA(ctx, temp);
    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb2f(core::Context& ctx) {
    /*  mnemonic: SRA A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SRA(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb30(core::Context& ctx) {
    /*  mnemonic: SWAP B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SWAP(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb31(core::Context& ctx) {
    /*  mnemonic: SWAP C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SWAP(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb32(core::Context& ctx) {
    /*  mnemonic: SWAP D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SWAP(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb33(core::Context& ctx) {
    /*  mnemonic: SWAP E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SWAP(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb34(core::Context& ctx) {
    /*  mnemonic: SWAP H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SWAP(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb35(core::Context& ctx) {
    /*  mnemonic: SWAP L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SWAP(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb36(core::Context& ctx) {
    /*  mnemonic: SWAP (HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 0 0
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SWAP(ctx, temp);
    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 16;
}
int opcode_0xcb37(core::Context& ctx) {
    /*  mnemonic: SWAP A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 0
	*/
    SWAP(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    ctx.r.CARRY_FLAG     = 0;
    return 8;
}
int opcode_0xcb38(core::Context& ctx) {
    /*  mnemonic: SRL B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SRL(ctx, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb39(core::Context& ctx) {
    /*  mnemonic: SRL C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SRL(ctx, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3a(core::Context& ctx) {
    /*  mnemonic: SRL D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SRL(ctx, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3b(core::Context& ctx) {
    /*  mnemonic: SRL E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SRL(ctx, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3c(core::Context& ctx) {
    /*  mnemonic: SRL H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SRL(ctx, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3d(core::Context& ctx) {
    /*  mnemonic: SRL L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SRL(ctx, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb3e(core::Context& ctx) {
    /*  mnemonic: SRL (HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 0 C
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SRL(ctx, temp);
    ctx.write8(ctx.r.HL, temp);

    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 16;
}
int opcode_0xcb3f(core::Context& ctx) {
    /*  mnemonic: SRL A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 0 C
	*/
    SRL(ctx, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 0;
    return 8;
}
int opcode_0xcb40(core::Context& ctx) {
    /*  mnemonic: BIT 0,B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 0, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb41(core::Context& ctx) {
    /*  mnemonic: BIT 0,C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 0, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb42(core::Context& ctx) {
    /*  mnemonic: BIT 0,D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 0, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb43(core::Context& ctx) {
    /*  mnemonic: BIT 0,E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 0, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb44(core::Context& ctx) {
    /*  mnemonic: BIT 0,H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 0, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb45(core::Context& ctx) {
    /*  mnemonic: BIT 0,L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 0, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb46(core::Context& ctx) {
    /*  mnemonic: BIT 0,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 0, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb47(core::Context& ctx) {
    /*  mnemonic: BIT 0,A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 0, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb48(core::Context& ctx) {
    /*  mnemonic: BIT 1,B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 1, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb49(core::Context& ctx) {
    /*  mnemonic: BIT 1,C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 1, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4a(core::Context& ctx) {
    /*  mnemonic: BIT 1,D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 1, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4b(core::Context& ctx) {
    /*  mnemonic: BIT 1,E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 1, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4c(core::Context& ctx) {
    /*  mnemonic: BIT 1,H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 1, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4d(core::Context& ctx) {
    /*  mnemonic: BIT 1,L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 1, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb4e(core::Context& ctx) {
    /*  mnemonic: BIT 1,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 1, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb4f(core::Context& ctx) {
    /*  mnemonic: BIT 1,A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 1, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb50(core::Context& ctx) {
    /*  mnemonic: BIT 2,B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 2, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb51(core::Context& ctx) {
    /*  mnemonic: BIT 2,C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 2, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb52(core::Context& ctx) {
    /*  mnemonic: BIT 2,D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 2, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb53(core::Context& ctx) {
    /*  mnemonic: BIT 2,E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 2, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb54(core::Context& ctx) {
    /*  mnemonic: BIT 2,H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 2, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb55(core::Context& ctx) {
    /*  mnemonic: BIT 2,L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 2, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb56(core::Context& ctx) {
    /*  mnemonic: BIT 2,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 2, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb57(core::Context& ctx) {
    /*  mnemonic: BIT 2,A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 2, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb58(core::Context& ctx) {
    /*  mnemonic: BIT 3,B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 3, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb59(core::Context& ctx) {
    /*  mnemonic: BIT 3,C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 3, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5a(core::Context& ctx) {
    /*  mnemonic: BIT 3,D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 3, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5b(core::Context& ctx) {
    /*  mnemonic: BIT 3,E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 3, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5c(core::Context& ctx) {
    /*  mnemonic: BIT 3,H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 3, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5d(core::Context& ctx) {
    /*  mnemonic: BIT 3,L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 3, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb5e(core::Context& ctx) {
    /*  mnemonic: BIT 3,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 3, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb5f(core::Context& ctx) {
    /*  mnemonic: BIT 3,A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 3, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb60(core::Context& ctx) {
    /*  mnemonic: BIT 4,B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 4, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb61(core::Context& ctx) {
    /*  mnemonic: BIT 4,C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 4, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb62(core::Context& ctx) {
    /*  mnemonic: BIT 4,D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 4, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb63(core::Context& ctx) {
    /*  mnemonic: BIT 4,E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 4, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb64(core::Context& ctx) {
    /*  mnemonic: BIT 4,H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 4, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb65(core::Context& ctx) {
    /*  mnemonic: BIT 4,L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 4, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb66(core::Context& ctx) {
    /*  mnemonic: BIT 4,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 4, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb67(core::Context& ctx) {
    /*  mnemonic: BIT 4,A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 4, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb68(core::Context& ctx) {
    /*  mnemonic: BIT 5,B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 5, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb69(core::Context& ctx) {
    /*  mnemonic: BIT 5,C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 5, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6a(core::Context& ctx) {
    /*  mnemonic: BIT 5,D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 5, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6b(core::Context& ctx) {
    /*  mnemonic: BIT 5,E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 5, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6c(core::Context& ctx) {
    /*  mnemonic: BIT 5,H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 5, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6d(core::Context& ctx) {
    /*  mnemonic: BIT 5,L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 5, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb6e(core::Context& ctx) {
    /*  mnemonic: BIT 5,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 5, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb6f(core::Context& ctx) {
    /*  mnemonic: BIT 5,A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 5, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb70(core::Context& ctx) {
    /*  mnemonic: BIT 6,B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 6, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb71(core::Context& ctx) {
    /*  mnemonic: BIT 6,C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 6, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb72(core::Context& ctx) {
    /*  mnemonic: BIT 6,D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 6, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb73(core::Context& ctx) {
    /*  mnemonic: BIT 6,E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 6, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb74(core::Context& ctx) {
    /*  mnemonic: BIT 6,H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 6, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb75(core::Context& ctx) {
    /*  mnemonic: BIT 6,L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 6, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb76(core::Context& ctx) {
    /*  mnemonic: BIT 6,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 6, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb77(core::Context& ctx) {
    /*  mnemonic: BIT 6,A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 6, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb78(core::Context& ctx) {
    /*  mnemonic: BIT 7,B
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 7, ctx.r.B);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb79(core::Context& ctx) {
    /*  mnemonic: BIT 7,C
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 7, ctx.r.C);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7a(core::Context& ctx) {
    /*  mnemonic: BIT 7,D
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 7, ctx.r.D);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7b(core::Context& ctx) {
    /*  mnemonic: BIT 7,E
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 7, ctx.r.E);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7c(core::Context& ctx) {
    /*  mnemonic: BIT 7,H
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 7, ctx.r.H);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7d(core::Context& ctx) {
    /*  mnemonic: BIT 7,L
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 7, ctx.r.L);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb7e(core::Context& ctx) {
    /*  mnemonic: BIT 7,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 7, ctx.read8(ctx.r.HL));
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 16;
}
int opcode_0xcb7f(core::Context& ctx) {
    /*  mnemonic: BIT 7,A
	*  length: 2
	*  cycles: 8
	*  affects: Z 0 1 -
	*/
    BIT(ctx, 7, ctx.r.A);
    ctx.r.SUB_FLAG       = 0;
    ctx.r.HALFCARRY_FLAG = 1;
    return 8;
}
int opcode_0xcb80(core::Context& ctx) {
    /*  mnemonic: RES 0,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(0, ctx.r.B);
    return 8;
}
int opcode_0xcb81(core::Context& ctx) {
    /*  mnemonic: RES 0,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(0, ctx.r.C);
    return 8;
}
int opcode_0xcb82(core::Context& ctx) {
    /*  mnemonic: RES 0,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(0, ctx.r.D);
    return 8;
}
int opcode_0xcb83(core::Context& ctx) {
    /*  mnemonic: RES 0,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(0, ctx.r.E);
    return 8;
}
int opcode_0xcb84(core::Context& ctx) {
    /*  mnemonic: RES 0,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(0, ctx.r.H);
    return 8;
}
int opcode_0xcb85(core::Context& ctx) {
    /*  mnemonic: RES 0,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(0, ctx.r.L);
    return 8;
}
int opcode_0xcb86(core::Context& ctx) {
    /*  mnemonic: RES 0,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    RES(0, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcb87(core::Context& ctx) {
    /*  mnemonic: RES 0,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(0, ctx.r.A);
    return 8;
}
int opcode_0xcb88(core::Context& ctx) {
    /*  mnemonic: RES 1,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(1, ctx.r.B);
    return 8;
}
int opcode_0xcb89(core::Context& ctx) {
    /*  mnemonic: RES 1,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(1, ctx.r.C);
    return 8;
}
int opcode_0xcb8a(core::Context& ctx) {
    /*  mnemonic: RES 1,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(1, ctx.r.D);
    return 8;
}
int opcode_0xcb8b(core::Context& ctx) {
    /*  mnemonic: RES 1,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(1, ctx.r.E);
    return 8;
}
int opcode_0xcb8c(core::Context& ctx) {
    /*  mnemonic: RES 1,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(1, ctx.r.H);
    return 8;
}
int opcode_0xcb8d(core::Context& ctx) {
    /*  mnemonic: RES 1,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(1, ctx.r.L);
    return 8;
}
int opcode_0xcb8e(core::Context& ctx) {
    /*  mnemonic: RES 1,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    RES(1, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcb8f(core::Context& ctx) {
    /*  mnemonic: RES 1,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(1, ctx.r.A);
    return 8;
}
int opcode_0xcb90(core::Context& ctx) {
    /*  mnemonic: RES 2,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(2, ctx.r.B);
    return 8;
}
int opcode_0xcb91(core::Context& ctx) {
    /*  mnemonic: RES 2,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(2, ctx.r.C);
    return 8;
}
int opcode_0xcb92(core::Context& ctx) {
    /*  mnemonic: RES 2,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(2, ctx.r.D);
    return 8;
}
int opcode_0xcb93(core::Context& ctx) {
    /*  mnemonic: RES 2,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(2, ctx.r.E);
    return 8;
}
int opcode_0xcb94(core::Context& ctx) {
    /*  mnemonic: RES 2,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(2, ctx.r.H);
    return 8;
}
int opcode_0xcb95(core::Context& ctx) {
    /*  mnemonic: RES 2,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(2, ctx.r.L);
    return 8;
}
int opcode_0xcb96(core::Context& ctx) {
    /*  mnemonic: RES 2,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    RES(2, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcb97(core::Context& ctx) {
    /*  mnemonic: RES 2,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(2, ctx.r.A);
    return 8;
}
int opcode_0xcb98(core::Context& ctx) {
    /*  mnemonic: RES 3,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(3, ctx.r.B);
    return 8;
}
int opcode_0xcb99(core::Context& ctx) {
    /*  mnemonic: RES 3,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(3, ctx.r.C);
    return 8;
}
int opcode_0xcb9a(core::Context& ctx) {
    /*  mnemonic: RES 3,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(3, ctx.r.D);
    return 8;
}
int opcode_0xcb9b(core::Context& ctx) {
    /*  mnemonic: RES 3,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(3, ctx.r.E);
    return 8;
}
int opcode_0xcb9c(core::Context& ctx) {
    /*  mnemonic: RES 3,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(3, ctx.r.H);
    return 8;
}
int opcode_0xcb9d(core::Context& ctx) {
    /*  mnemonic: RES 3,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(3, ctx.r.L);
    return 8;
}
int opcode_0xcb9e(core::Context& ctx) {
    /*  mnemonic: RES 3,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    RES(3, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcb9f(core::Context& ctx) {
    /*  mnemonic: RES 3,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(3, ctx.r.A);
    return 8;
}
int opcode_0xcba0(core::Context& ctx) {
    /*  mnemonic: RES 4,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(4, ctx.r.B);
    return 8;
}
int opcode_0xcba1(core::Context& ctx) {
    /*  mnemonic: RES 4,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(4, ctx.r.C);
    return 8;
}
int opcode_0xcba2(core::Context& ctx) {
    /*  mnemonic: RES 4,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(4, ctx.r.D);
    return 8;
}
int opcode_0xcba3(core::Context& ctx) {
    /*  mnemonic: RES 4,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(4, ctx.r.E);
    return 8;
}
int opcode_0xcba4(core::Context& ctx) {
    /*  mnemonic: RES 4,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(4, ctx.r.H);
    return 8;
}
int opcode_0xcba5(core::Context& ctx) {
    /*  mnemonic: RES 4,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(4, ctx.r.L);
    return 8;
}
int opcode_0xcba6(core::Context& ctx) {
    /*  mnemonic: RES 4,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    RES(4, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcba7(core::Context& ctx) {
    /*  mnemonic: RES 4,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(4, ctx.r.A);
    return 8;
}
int opcode_0xcba8(core::Context& ctx) {
    /*  mnemonic: RES 5,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(5, ctx.r.B);
    return 8;
}
int opcode_0xcba9(core::Context& ctx) {
    /*  mnemonic: RES 5,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(5, ctx.r.C);
    return 8;
}
int opcode_0xcbaa(core::Context& ctx) {
    /*  mnemonic: RES 5,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(5, ctx.r.D);
    return 8;
}
int opcode_0xcbab(core::Context& ctx) {
    /*  mnemonic: RES 5,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(5, ctx.r.E);
    return 8;
}
int opcode_0xcbac(core::Context& ctx) {
    /*  mnemonic: RES 5,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(5, ctx.r.H);
    return 8;
}
int opcode_0xcbad(core::Context& ctx) {
    /*  mnemonic: RES 5,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(5, ctx.r.L);
    return 8;
}
int opcode_0xcbae(core::Context& ctx) {
    /*  mnemonic: RES 5,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    RES(5, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbaf(core::Context& ctx) {
    /*  mnemonic: RES 5,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(5, ctx.r.A);
    return 8;
}
int opcode_0xcbb0(core::Context& ctx) {
    /*  mnemonic: RES 6,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(6, ctx.r.B);
    return 8;
}
int opcode_0xcbb1(core::Context& ctx) {
    /*  mnemonic: RES 6,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(6, ctx.r.C);
    return 8;
}
int opcode_0xcbb2(core::Context& ctx) {
    /*  mnemonic: RES 6,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(6, ctx.r.D);
    return 8;
}
int opcode_0xcbb3(core::Context& ctx) {
    /*  mnemonic: RES 6,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(6, ctx.r.E);
    return 8;
}
int opcode_0xcbb4(core::Context& ctx) {
    /*  mnemonic: RES 6,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(6, ctx.r.H);
    return 8;
}
int opcode_0xcbb5(core::Context& ctx) {
    /*  mnemonic: RES 6,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(6, ctx.r.L);
    return 8;
}
int opcode_0xcbb6(core::Context& ctx) {
    /*  mnemonic: RES 6,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    RES(6, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbb7(core::Context& ctx) {
    /*  mnemonic: RES 6,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(6, ctx.r.A);
    return 8;
}
int opcode_0xcbb8(core::Context& ctx) {
    /*  mnemonic: RES 7,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(7, ctx.r.B);
    return 8;
}
int opcode_0xcbb9(core::Context& ctx) {
    /*  mnemonic: RES 7,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(7, ctx.r.C);
    return 8;
}
int opcode_0xcbba(core::Context& ctx) {
    /*  mnemonic: RES 7,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(7, ctx.r.D);
    return 8;
}
int opcode_0xcbbb(core::Context& ctx) {
    /*  mnemonic: RES 7,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(7, ctx.r.E);
    return 8;
}
int opcode_0xcbbc(core::Context& ctx) {
    /*  mnemonic: RES 7,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(7, ctx.r.H);
    return 8;
}
int opcode_0xcbbd(core::Context& ctx) {
    /*  mnemonic: RES 7,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(7, ctx.r.L);
    return 8;
}
int opcode_0xcbbe(core::Context& ctx) {
    /*  mnemonic: RES 7,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    RES(7, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbbf(core::Context& ctx) {
    /*  mnemonic: RES 7,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    RES(7, ctx.r.A);
    return 8;
}
int opcode_0xcbc0(core::Context& ctx) {
    /*  mnemonic: SET 0,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(0, ctx.r.B);
    return 8;
}
int opcode_0xcbc1(core::Context& ctx) {
    /*  mnemonic: SET 0,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(0, ctx.r.C);
    return 8;
}
int opcode_0xcbc2(core::Context& ctx) {
    /*  mnemonic: SET 0,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(0, ctx.r.D);
    return 8;
}
int opcode_0xcbc3(core::Context& ctx) {
    /*  mnemonic: SET 0,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(0, ctx.r.E);
    return 8;
}
int opcode_0xcbc4(core::Context& ctx) {
    /*  mnemonic: SET 0,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(0, ctx.r.H);
    return 8;
}
int opcode_0xcbc5(core::Context& ctx) {
    /*  mnemonic: SET 0,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(0, ctx.r.L);
    return 8;
}
int opcode_0xcbc6(core::Context& ctx) {
    /*  mnemonic: SET 0,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SET(0, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbc7(core::Context& ctx) {
    /*  mnemonic: SET 0,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(0, ctx.r.A);
    return 8;
}
int opcode_0xcbc8(core::Context& ctx) {
    /*  mnemonic: SET 1,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(1, ctx.r.B);
    return 8;
}
int opcode_0xcbc9(core::Context& ctx) {
    /*  mnemonic: SET 1,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(1, ctx.r.C);
    return 8;
}
int opcode_0xcbca(core::Context& ctx) {
    /*  mnemonic: SET 1,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(1, ctx.r.D);
    return 8;
}
int opcode_0xcbcb(core::Context& ctx) {
    /*  mnemonic: SET 1,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(1, ctx.r.E);
    return 8;
}
int opcode_0xcbcc(core::Context& ctx) {
    /*  mnemonic: SET 1,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(1, ctx.r.H);
    return 8;
}
int opcode_0xcbcd(core::Context& ctx) {
    /*  mnemonic: SET 1,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(1, ctx.r.L);
    return 8;
}
int opcode_0xcbce(core::Context& ctx) {
    /*  mnemonic: SET 1,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SET(1, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbcf(core::Context& ctx) {
    /*  mnemonic: SET 1,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(1, ctx.r.A);
    return 8;
}
int opcode_0xcbd0(core::Context& ctx) {
    /*  mnemonic: SET 2,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(2, ctx.r.B);
    return 8;
}
int opcode_0xcbd1(core::Context& ctx) {
    /*  mnemonic: SET 2,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(2, ctx.r.C);
    return 8;
}
int opcode_0xcbd2(core::Context& ctx) {
    /*  mnemonic: SET 2,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(2, ctx.r.D);
    return 8;
}
int opcode_0xcbd3(core::Context& ctx) {
    /*  mnemonic: SET 2,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(2, ctx.r.E);
    return 8;
}
int opcode_0xcbd4(core::Context& ctx) {
    /*  mnemonic: SET 2,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(2, ctx.r.H);
    return 8;
}
int opcode_0xcbd5(core::Context& ctx) {
    /*  mnemonic: SET 2,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(2, ctx.r.L);
    return 8;
}
int opcode_0xcbd6(core::Context& ctx) {
    /*  mnemonic: SET 2,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SET(2, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbd7(core::Context& ctx) {
    /*  mnemonic: SET 2,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(2, ctx.r.A);
    return 8;
}
int opcode_0xcbd8(core::Context& ctx) {
    /*  mnemonic: SET 3,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(3, ctx.r.B);
    return 8;
}
int opcode_0xcbd9(core::Context& ctx) {
    /*  mnemonic: SET 3,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(3, ctx.r.C);
    return 8;
}
int opcode_0xcbda(core::Context& ctx) {
    /*  mnemonic: SET 3,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(3, ctx.r.D);
    return 8;
}
int opcode_0xcbdb(core::Context& ctx) {
    /*  mnemonic: SET 3,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(3, ctx.r.E);
    return 8;
}
int opcode_0xcbdc(core::Context& ctx) {
    /*  mnemonic: SET 3,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(3, ctx.r.H);
    return 8;
}
int opcode_0xcbdd(core::Context& ctx) {
    /*  mnemonic: SET 3,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(3, ctx.r.L);
    return 8;
}
int opcode_0xcbde(core::Context& ctx) {
    /*  mnemonic: SET 3,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SET(3, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbdf(core::Context& ctx) {
    /*  mnemonic: SET 3,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(3, ctx.r.A);
    return 8;
}
int opcode_0xcbe0(core::Context& ctx) {
    /*  mnemonic: SET 4,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(4, ctx.r.B);
    return 8;
}
int opcode_0xcbe1(core::Context& ctx) {
    /*  mnemonic: SET 4,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(4, ctx.r.C);
    return 8;
}
int opcode_0xcbe2(core::Context& ctx) {
    /*  mnemonic: SET 4,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(4, ctx.r.D);
    return 8;
}
int opcode_0xcbe3(core::Context& ctx) {
    /*  mnemonic: SET 4,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(4, ctx.r.E);
    return 8;
}
int opcode_0xcbe4(core::Context& ctx) {
    /*  mnemonic: SET 4,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(4, ctx.r.H);
    return 8;
}
int opcode_0xcbe5(core::Context& ctx) {
    /*  mnemonic: SET 4,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(4, ctx.r.L);
    return 8;
}
int opcode_0xcbe6(core::Context& ctx) {
    /*  mnemonic: SET 4,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SET(4, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbe7(core::Context& ctx) {
    /*  mnemonic: SET 4,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(4, ctx.r.A);
    return 8;
}
int opcode_0xcbe8(core::Context& ctx) {
    /*  mnemonic: SET 5,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(5, ctx.r.B);
    return 8;
}
int opcode_0xcbe9(core::Context& ctx) {
    /*  mnemonic: SET 5,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(5, ctx.r.C);
    return 8;
}
int opcode_0xcbea(core::Context& ctx) {
    /*  mnemonic: SET 5,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(5, ctx.r.D);
    return 8;
}
int opcode_0xcbeb(core::Context& ctx) {
    /*  mnemonic: SET 5,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(5, ctx.r.E);
    return 8;
}
int opcode_0xcbec(core::Context& ctx) {
    /*  mnemonic: SET 5,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(5, ctx.r.H);
    return 8;
}
int opcode_0xcbed(core::Context& ctx) {
    /*  mnemonic: SET 5,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(5, ctx.r.L);
    return 8;
}
int opcode_0xcbee(core::Context& ctx) {
    /*  mnemonic: SET 5,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SET(5, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbef(core::Context& ctx) {
    /*  mnemonic: SET 5,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(5, ctx.r.A);
    return 8;
}
int opcode_0xcbf0(core::Context& ctx) {
    /*  mnemonic: SET 6,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(6, ctx.r.B);
    return 8;
}
int opcode_0xcbf1(core::Context& ctx) {
    /*  mnemonic: SET 6,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(6, ctx.r.C);
    return 8;
}
int opcode_0xcbf2(core::Context& ctx) {
    /*  mnemonic: SET 6,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(6, ctx.r.D);
    return 8;
}
int opcode_0xcbf3(core::Context& ctx) {
    /*  mnemonic: SET 6,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(6, ctx.r.E);
    return 8;
}
int opcode_0xcbf4(core::Context& ctx) {
    /*  mnemonic: SET 6,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(6, ctx.r.H);
    return 8;
}
int opcode_0xcbf5(core::Context& ctx) {
    /*  mnemonic: SET 6,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(6, ctx.r.L);
    return 8;
}
int opcode_0xcbf6(core::Context& ctx) {
    /*  mnemonic: SET 6,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SET(6, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbf7(core::Context& ctx) {
    /*  mnemonic: SET 6,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(6, ctx.r.A);
    return 8;
}
int opcode_0xcbf8(core::Context& ctx) {
    /*  mnemonic: SET 7,B
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(7, ctx.r.B);
    return 8;
}
int opcode_0xcbf9(core::Context& ctx) {
    /*  mnemonic: SET 7,C
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(7, ctx.r.C);
    return 8;
}
int opcode_0xcbfa(core::Context& ctx) {
    /*  mnemonic: SET 7,D
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(7, ctx.r.D);
    return 8;
}
int opcode_0xcbfb(core::Context& ctx) {
    /*  mnemonic: SET 7,E
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(7, ctx.r.E);
    return 8;
}
int opcode_0xcbfc(core::Context& ctx) {
    /*  mnemonic: SET 7,H
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(7, ctx.r.H);
    return 8;
}
int opcode_0xcbfd(core::Context& ctx) {
    /*  mnemonic: SET 7,L
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(7, ctx.r.L);
    return 8;
}
int opcode_0xcbfe(core::Context& ctx) {
    /*  mnemonic: SET 7,(HL)
	*  length: 2
	*  cycles: 16
	*  affects: - - - -
	*/
    auto temp = ctx.read8(ctx.r.HL);
    SET(7, temp);
    ctx.write8(ctx.r.HL, temp);
    return 16;
}
int opcode_0xcbff(core::Context& ctx) {
    /*  mnemonic: SET 7,A
	*  length: 2
	*  cycles: 8
	*  affects: - - - -
	*/
    SET(7, ctx.r.A);
    return 8;
}