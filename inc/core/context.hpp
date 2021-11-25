#ifndef CONTEXT_HPP
#define CONTEXT_HPP
#include "registers.hpp"
#include "memory.hpp"
#include <string>

namespace core {

    struct Context {

        core::Registers r = {};

        core::Memory m = {};

        uint8_t read8(uint16_t address);

        uint16_t read16(uint16_t address);

        void write8(uint16_t address, uint8_t value);

        void write16(uint16_t address, uint16_t value);

        uint8_t  imm8();
        uint16_t imm16();

        uint8_t  peek8(int offset = 0);
        uint16_t peek16();
        int8_t   peek8_signed();

        int8_t imm8_signed();

        std::string print();
    };

} // namespace core
#endif