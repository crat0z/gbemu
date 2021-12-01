#include "core/context.hpp"

#include <iostream>
#include <fmt/format.h>

namespace core {

    Context::Context(std::shared_ptr<Memory> mem) : m(mem), r{} {}

    uint8_t Context::read8(uint16_t address) {
        uint8_t first_byte = address >> 8;
        //uint8_t second_byte = address & 0xFF;

        switch (first_byte & 0xF0) {
        case 0x00:
        case 0x10:
        case 0x20:
        case 0x30:
        case 0x40:
        case 0x50:
        case 0x60:
        case 0x70:
        case 0x80:
        case 0x90:
        case 0xA0:
        case 0xB0:
        case 0xC0:
        case 0xD0:
            return m->memory[address];
        case 0xE0:
        case 0xF0:
            switch (first_byte & 0xF) {
            case 0x0:
            case 0x1:
            case 0x2:
            case 0x3:
            case 0x4:
            case 0x5:
            case 0x6:
            case 0x7:
            case 0x8:
            case 0x9:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xD:
                return m->memory[address - 0x2000];
            case 0xE: {
                return m->memory[address];
            }
            case 0xF:
                return m->memory[address];
            }
        }
        __builtin_unreachable();
    }
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

    bool Context::interrupt_pending() const { return (m->IFRegister & m->IERegister) != 0; }

    /*
        switch (address & 0xF) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF:
        }
        break;
     */

    void Context::write8(uint16_t address, uint8_t value) {

        uint8_t first_byte  = address >> 8;
        uint8_t second_byte = address & 0xFF;

        switch (first_byte & 0xF0) {
        case 0x00:
        case 0x10:
        case 0x20:
        case 0x30:
        case 0x40:
        case 0x50:
        case 0x60:
        case 0x70:
        case 0x80:
        case 0x90:
        case 0xA0:
        case 0xB0:
        case 0xC0:
        case 0xD0:
            m->memory[address] = value;
            return;
            // ECHO RAM from E000-FDFF
        case 0xE0:

        case 0xF0:
            switch (first_byte & 0xF) {
            case 0x0:
            case 0x1:
            case 0x2:
            case 0x3:
            case 0x4:
            case 0x5:
            case 0x6:
            case 0x7:
            case 0x8:
            case 0x9:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xD:
                m->memory[address - 0x2000] = value;
                return;
            case 0xE: {
                m->memory[address] = value;
                return;
            }
                // 0xFF00-0xFFFF
            case 0xF:
                switch (second_byte) {
                default: {
                    m->memory[address] = value;
                    return;
                }
                    // 0xFF02
                case 0x02: {
                    if (value == 0x81) {
                        std::cout << static_cast<char>(read8(0xFF01));
                        m->memory[0xFF02] = 0x01;
                    }
                    return;
                }
                case 0x04: {
                    m->DIV = 0;
                    return;
                }
                case 0x07: {
                    uint8_t select = value & 0x3;

                    //TIMA_enabled = (value >> 2) & 0x1;

                    m->TAC = value & 0x7;

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
            }
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

    uint8_t  Context::peek8(int offset) { return m->get8(r.PC + offset); }
    uint16_t Context::peek16() { return m->get16(r.PC); }
    int8_t   Context::peek8_signed() { return static_cast<int8_t>(m->get8(r.PC)); }

    int8_t Context::imm8_signed() { return static_cast<int8_t>(read8(r.PC++)); }

    /* If a TMA write is executed on the same cycle as the content
       of TMA is transferred to TIMA due to a timer overflow, the old value is transferred to TIMA.
       TLDR: check timer before executing
    */
    void Context::TIMA_inc() {
        if (m->get_TAC_enabled()) {
            if (m->TIMA == 0xFF) {
                m->TIMA = m->TMA;
                m->set_IF_Timer();
            }
            else {
                m->TIMA += 1;
            }
        }
    }

    void Context::DIV_inc() { m->DIV += 1; }

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