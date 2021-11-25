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

        /* while (result) {
            // wait for it
            while (timer.update()) {}
            result--;
        } */

        //std::cout << ctx.print();

        auto op = ctx.imm8();

        if (op == 0xCB) {
            op = ctx.imm8();

            //std::cout << op_cb_str_table[op](ctx);
            //std::cout << '\n';
            result = cb_table[op](ctx);
        }
        else {
            //std::cout << op_str_table[op](ctx);
            //std::cout << '\n';
            result = op_table[op](ctx);
        }

        if (ctx.m[0xFF02] == 0x81) {
            char ch = static_cast<char>(ctx.m[0xFF01]);
            std::cout << ch;
            ctx.m[0xFF02] = 0x1;
        }
    }
} // namespace core
