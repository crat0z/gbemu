#include <iostream>
#include <bitset>
#include <fstream>
#include <ctime>
#include <thread>
#include <future>
#include "core/gameboy.hpp"
#include "core/gameboy_opcodes.hpp"
#include "core/str_opcodes.hpp"

namespace core {
    Gameboy::Gameboy()
            : op_table(create_table()),
              cb_table(create_cb_table()),
              op_str_table(opcode_strings()),
              op_cb_str_table(cb_opcode_strings()) {
        is_ready = false;
    }

    void Gameboy::reset_state() {
        // just in case in future
        is_ready = false;

        ctx = {};

        cycle_count = 0;
    }

    uint16_t Gameboy::fetch(uint16_t addr) {

        auto code = *reinterpret_cast<uint16_t*>(&ctx.m[addr]);

        return code;
    }

    op Gameboy::decode(uint16_t opc) {}

    void Gameboy::execute() {}

    void Gameboy::cycle() {
        static auto result = 0;

        while (result) {
            // wait for it
            while (!timer.update()) {}
            result--;
        }

        auto op = ctx.imm8();

        if (op == 0xCB) {
            op     = ctx.imm8();
            result = cb_table[op](ctx);
        }
        else {
            result = op_table[op](ctx);
        }
    }
} // namespace core
