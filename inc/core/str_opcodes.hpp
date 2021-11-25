#ifndef STR_OPCODES_HPP
#define STR_OPCODES_HPP

#include <array>
#include <string>

namespace core {
    struct Context;
}
std::array<std::string (*)(core::Context&), 256> opcode_strings();
std::array<std::string (*)(core::Context&), 256> cb_opcode_strings();

#endif