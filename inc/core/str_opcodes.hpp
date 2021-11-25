#ifndef STR_OPCODES_HPP
#define STR_OPCODES_HPP

#include <array>
#include <string>

struct Context;

std::array<std::string (*)(Context&), 256> opcode_strings();
std::array<std::string (*)(Context&), 256> cb_opcode_strings();

#endif