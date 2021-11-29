#include "core/context.hpp"

#include <iostream>
#include <fmt/format.h>

namespace core {
    uint8_t Context::read8(uint16_t address) { return m.memory[address]; }

    uint16_t Context::read16(uint16_t address) {
        uint16_t first  = read8(address);
        auto     second = read8(address + 1);

        return (second << 8) | first;
    }

    void Context::TIMA_update(int last_cycle) {
        TIMA_count += last_cycle;
        if (TIMA_count >= TIMA_factor) {
            TIMA_count -= TIMA_factor;
            TIMA_inc();
        }
    }

    bool Context::interrupt_pending() const { return (m.IFRegister & m.IERegister) != 0; }

    void Context::write8(uint16_t address, uint8_t value) {
        switch (address) {
        default:
            m.memory[address] = value;
            return;
        case 0xFF02:
            if (value == 0x81) {
                std::cout << static_cast<char>(read8(0xFF01));
                m.memory[0xFF02] = 0x01;
            }
            return;
        case 0xFF04:
            m.DIV = 0;
            return;
        case 0xFF07:
            uint8_t select = value & 0x3;
            uint8_t enable = (value >> 2) & 0x1;

            m.TAC = value & 0x7;

            switch (select) {
            case 0b00:
                TIMA_factor = 1024;
                break;
            case 0b01:
                TIMA_factor = 16;
                break;
            case 0b10:
                TIMA_factor = 64;
                break;
            case 0b11:
                TIMA_factor = 256;
                break;
            default:
                break;
            }
            TIMA_count = 0;
            return;
        }
    }

    void Context::write16(uint16_t address, uint16_t value) {
        uint8_t first  = value & 0xFF;
        uint8_t second = value >> 8;

        write8(address, first);
        write8(address + 1, second);
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

    /* If a TMA write is executed on the same cycle as the content
       of TMA is transferred to TIMA due to a timer overflow, the old value is transferred to TIMA.
       TLDR: check timer before executing
    */
    void Context::TIMA_inc() {
        if (m.get_TAC_enabled()) {
            if (m.TIMA == 0xFF) {
                m.TIMA = m.TMA;
                m.set_IF_Timer();
            }
            else {
                m.TIMA += 1;
            }
        }
    }

    void Context::DIV_inc() { m.DIV += 1; }

    std::string Context::print() {
        std::string ret;
        ret.reserve(200);
        ret += "#########################\n";
        ret += fmt::format("Z:{}, N:{}, H:{}, C:{}\n", (uint8_t)r.get_zero(), (uint8_t)r.get_sub(),
                           (uint8_t)r.get_halfcarry(), (uint8_t)r.get_carry());
        ret += fmt::format("A:{:#04x}\t\tAF:{:#06x}\n", r.A, r.AF);
        ret += fmt::format("B:{:#04x}\tC:{:#04x}\tBC:{:#06x}\n", r.B, r.C, r.BC);
        ret += fmt::format("D:{:#04x}\tE:{:#04x}\tDE:{:#06x}\n", r.D, r.E, r.DE);
        ret += fmt::format("H:{:#04x}\tL:{:#04x}\tHL:{:#06x}\n", r.H, r.L, r.HL);
        ret += fmt::format("PC:{:#06x}\tSP:{:#06x}\n\n", r.PC, r.SP);

        return ret;
    }
} // namespace core