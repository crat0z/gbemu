#include "core/memory.hpp"

namespace core {

    uint8_t Memory::get8(uint16_t addr) { return memory[addr]; }

    uint16_t Memory::get16(uint16_t addr) {
        auto ret = *reinterpret_cast<uint16_t*>(&memory[addr]);
        return ret;
    }

    void Memory::set16(uint16_t addr, uint16_t val) {
        auto ret = reinterpret_cast<uint16_t*>(&memory[addr]);
        *ret     = val;
    }

} // namespace core
