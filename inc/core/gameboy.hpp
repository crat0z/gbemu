#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include "core/timer.hpp"
#include "core/opcodes.hpp"
#include "core/stack.hpp"
#include <fmt/format.h>

struct Registers {
    union {
        uint16_t AF;
        struct {
            uint8_t NOTHING        : 4;

            uint8_t CARRY_FLAG     : 1;
            uint8_t HALFCARRY_FLAG : 1;
            uint8_t SUB_FLAG       : 1;
            uint8_t ZERO_FLAG      : 1;

            uint8_t A;
        };
    };
    union {
        uint16_t BC;
        struct {
            uint8_t C;
            uint8_t B;
        };
    };
    union {
        uint16_t DE;
        struct {
            uint8_t E;
            uint8_t D;
        };
    };
    union {
        uint16_t HL;
        struct {
            uint8_t L;
            uint8_t H;
        };
    };
    uint16_t SP = 0xFFFE;
    uint16_t PC;
} __attribute__((aligned(16)));

struct RAM {
    union {
        struct {
            std::array<uint8_t, 0x4000> ROMbank0;
            std::array<uint8_t, 0x4000> ROMbank1;
            std::array<uint8_t, 0x2000> VideoRAM;
            std::array<uint8_t, 0x2000> ExternalRAM;
            std::array<uint8_t, 0x1000> WRAM0;
            std::array<uint8_t, 0x1000> WRAM1;
            std::array<uint8_t, 0x1E00> ECHORAM;
            std::array<uint8_t, 0xA0>   SpriteAttributeTable;
            std::array<uint8_t, 0x60>   NotUsable;
            std::array<uint8_t, 0x80>   IORegisters;
            std::array<uint8_t, 0x7F>   HighRAM;
            uint8_t                     IERegister;
        };
        std::array<uint8_t, 65536> memory;
    };
    uint8_t imm8(uint16_t& PC) {
        auto ret = memory[PC];
        PC += 1;
        return ret;
    }
    uint16_t imm16(uint16_t& PC) {
        auto ret = *reinterpret_cast<uint16_t*>(&memory[PC]);
        PC += 2;
        return ret;
    }

    uint8_t get8(uint16_t addr) { return memory[addr]; }

    uint16_t get16(uint16_t addr) {
        auto ret = *reinterpret_cast<uint16_t*>(&memory[addr]);
        return ret;
    }

    void set16(uint16_t addr, uint16_t val) {
        auto ret = reinterpret_cast<uint16_t*>(&memory[addr]);
        *ret     = val;
    }

    uint8_t& operator[](uint16_t index) { return memory[index]; }
} __attribute__((aligned(65536)));

struct Context {

    Registers r = {};

    RAM m = {};

    uint8_t read8(uint16_t address) { return m.memory[address]; }

    uint16_t read16(uint16_t address) { return *reinterpret_cast<uint16_t*>(&m.memory[address]); }

    void write8(uint16_t address, uint8_t value) { m.memory[address] = value; }

    void write16(uint16_t address, uint16_t value) {
        *reinterpret_cast<uint16_t*>(&m.memory[address]) = value;
    }

    uint8_t  imm8() { return read8(r.PC++); }
    uint16_t imm16() {
        auto val = read16(r.PC);
        r.PC += 2;
        return val;
    }

    uint8_t  peek8(int offset = 0) { return m.get8(r.PC + offset); }
    uint16_t peek16() { return m.get16(r.PC); }
    int8_t   peek8_signed() { return static_cast<int8_t>(m.get8(r.PC)); }

    int8_t imm8_signed() { return static_cast<int8_t>(read8(r.PC++)); }

    std::string print() {
        std::string ret;
        ret.reserve(200);
        ret += "#########################\n";
        ret += fmt::format("Z:{}, N:{}, H:{}, C:{}\n", (uint8_t)r.ZERO_FLAG, (uint8_t)r.SUB_FLAG,
                           (uint8_t)r.HALFCARRY_FLAG, (uint8_t)r.CARRY_FLAG);
        ret += fmt::format("A:{:#04x}\t\tAF:{:#06x}\n", r.A, r.AF);
        ret += fmt::format("B:{:#04x}\tC:{:#04x}\tBC:{:#06x}\n", r.B, r.C, r.BC);
        ret += fmt::format("D:{:#04x}\tE:{:#04x}\tDE:{:#06x}\n", r.D, r.E, r.DE);
        ret += fmt::format("H:{:#04x}\tL:{:#04x}\tHL:{:#06x}\n", r.H, r.L, r.HL);
        ret += fmt::format("PC:{:#06x}\tSP:{:#06x}\n\n", r.PC, r.SP);
        ret += fmt::format("opcode:{:#04x}\n", peek8());

        return ret;
    }
};

namespace core {
    class Gameboy {

    private:
        friend class EmuWrapper;

        Context ctx;

        std::array<int (*)(Context&), 256> op_table;
        std::array<int (*)(Context&), 256> cb_table;

        std::array<std::string (*)(Context&), 256> op_str_table;
        std::array<std::string (*)(Context&), 256> op_cb_str_table;

        CETimer<4194304> timer;

        size_t cycle_count;
        bool   is_ready;

        uint16_t fetch(uint16_t addr);
        op       decode(uint16_t opc);
        void     execute();

        void cycle();

        // read file on filesystem with name, write to emu memory beginning @ addr
        void reset_state();

    public:
        Gameboy();
    };
} // namespace core

#endif