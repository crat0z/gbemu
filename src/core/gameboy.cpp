#include <iostream>
#include <bitset>
#include <fstream>
#include <ctime>
#include <thread>
#include <future>
#include "core/gameboy.hpp"
#include "core/bytes.hpp"
#include "core/gameboy_opcodes.hpp"

namespace core {
    Gameboy::Gameboy() : op_table(create_table()), cb_table(create_cb_table()) { is_ready = false; }

    void Gameboy::reset_state() {
        // just in case in future
        is_ready = false;

        ram = {};
        r   = {};

        cycle_count = 0;
    }

    uint16_t Gameboy::fetch(uint16_t addr) {

        auto code = *reinterpret_cast<uint16_t*>(&ram[addr]);

        // Gameboy is big endian, swap byte order
        code = swap_byte_order(code);

        return code;
    }

    op Gameboy::decode(uint16_t opc) {}

    void Gameboy::execute() {}

    void Gameboy::cycle() {
        auto result = 0;

        auto op = ram[r.PC++];

        if (op == 0xCB) {
            result = cb_table[op](r, ram);
        }
        else {
            result = op_table[op](r, ram);
        }
    }
} // namespace core