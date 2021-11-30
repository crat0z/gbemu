
#ifndef GAMEBOY_OPCODES_HPP
#define GAMEBOY_OPCODES_HPP

#include <array>

namespace core {
    struct Context;
}

std::array<int (*)(core::Context&), 256> create_table();
std::array<int (*)(core::Context&), 256> create_cb_table();

#endif