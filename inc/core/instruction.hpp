#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP
#include "core/opcodes.hpp"

namespace core {
    struct Instruction {
        uint16_t    address;
        uint16_t    opcode;
        op          operation;
        uint8_t     length;
        std::string mnemonic;

        Instruction() = default;

        Instruction(const Instruction& rhs) = default;
        Instruction& operator=(const Instruction& rhs) = default;

        Instruction(Instruction&& rhs) = default;
        Instruction& operator=(Instruction&& rhs) = default;

        ~Instruction() = default;

        Instruction(uint16_t addr)
                : address{ addr },
                  opcode{ 0 },
                  operation{ op::UNKNOWN },
                  length{ 0 },
                  mnemonic{ "" } {}

        // length = 2 always at this point, change in future
        Instruction(uint16_t addr, uint16_t opc)
                : address{ addr },
                  opcode{ opc },
                  operation{ decode(opc) },
                  length{ 2 },
                  mnemonic{ opcode_mnemonic(opc) } {}
    };
} // namespace core

#endif