#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP
#include "core/opcodes.hpp"
#include <fmt/format.h>

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
                : address{ addr }, opcode{ 0 }, operation{ op::UNKNOWN }, length{ 1 }, mnemonic{} {}

        // length = 2 always at this point, change in future
        Instruction(uint16_t addr, uint16_t opc)
                : address{ addr },
                  opcode{ /*swap_byte_order(opc)*/ },
                  operation{ decode(opc) },
                  length{ 2 },
                  mnemonic{ opcode_mnemonic(opc) } {}

        std::string opcode_string() const noexcept {
            // every instruction is at least one byte long

            auto tmp = opcode;

            std::string ret = fmt::format("{:02X}", tmp & 0xFF);

            for (auto i = 0; i < length - 1; ++i) {
                tmp >>= 8;
                ret.append(fmt::format(" {:02X}", tmp & 0xFF));
            }

            return ret;
        }
    };
} // namespace core

#endif