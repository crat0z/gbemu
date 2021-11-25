#include "core/memory.hpp"

namespace core {

    uint8_t Memory::imm8(uint16_t& PC) {
        auto ret = memory[PC];
        PC += 1;
        return ret;
    }
    uint16_t Memory::imm16(uint16_t& PC) {
        auto ret = *reinterpret_cast<uint16_t*>(&memory[PC]);
        PC += 2;
        return ret;
    }

    uint8_t Memory::get8(uint16_t addr) { return memory[addr]; }

    uint16_t Memory::get16(uint16_t addr) {
        auto ret = *reinterpret_cast<uint16_t*>(&memory[addr]);
        return ret;
    }

    void Memory::set16(uint16_t addr, uint16_t val) {
        auto ret = reinterpret_cast<uint16_t*>(&memory[addr]);
        *ret     = val;
    }

    uint8_t& Memory::operator[](uint16_t index) { return memory[index]; }
} // namespace core
