#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include "core/timer.hpp"
#include "core/stack.hpp"
#include <fmt/format.h>
#include "core/context.hpp"

namespace core {
    class Gameboy {

    private:
        friend class EmuWrapper;

        Context ctx;

        std::array<int (*)(Context&), 256> op_table;
        std::array<int (*)(Context&), 256> cb_table;

        std::array<std::string (*)(Context&), 256> op_str_table;
        std::array<std::string (*)(Context&), 256> op_cb_str_table;

        CETimer<4194304> CPUTimer;

        CETimer<16384> DIVTimer;

        size_t cycle_count;
        bool   is_ready;

        void cycle();

        // read file on filesystem with name, write to emu memory beginning @ addr
        void reset_state();

    public:
        Gameboy();
    };
} // namespace core

#endif