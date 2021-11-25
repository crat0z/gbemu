#include "core/context.hpp"

#include <iostream>
#include <fmt/format.h>

namespace core {
    uint8_t Context::read8(uint16_t address) { return m.memory[address]; }

    uint16_t Context::read16(uint16_t address) {
        return *reinterpret_cast<uint16_t*>(&m.memory[address]);
    }

    void Context::write8(uint16_t address, uint8_t value) {
        if (address == 0xFF02 && value == 0x81) {
            std::cout << static_cast<char>(m[0xFF01]);
            m.memory[0xFF02] = 0x01;
        }
        m.memory[address] = value;
    }

    void Context::write16(uint16_t address, uint16_t value) {
        *reinterpret_cast<uint16_t*>(&m.memory[address]) = value;
    }

    uint8_t  Context::imm8() { return read8(r.PC++); }
    uint16_t Context::imm16() {
        auto val = read16(r.PC);
        r.PC += 2;
        return val;
    }

    uint8_t  Context::peek8(int offset) { return m.get8(r.PC + offset); }
    uint16_t Context::peek16() { return m.get16(r.PC); }
    int8_t   Context::peek8_signed() { return static_cast<int8_t>(m.get8(r.PC)); }

    int8_t Context::imm8_signed() { return static_cast<int8_t>(read8(r.PC++)); }

    std::string Context::print() {
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
} // namespace core