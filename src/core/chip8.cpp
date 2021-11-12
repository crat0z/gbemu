#include <iostream>
#include <bitset>
#include <fstream>
#include <ctime>
#include <thread>
#include <future>

#include "core/chip8.hpp"

const uint8_t fontset[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80 // F
};

static uint16_t swap_byte_order(uint16_t s) { return (s >> 8) | (s << 8); }

namespace core {
    Chip8::Chip8() : stack(16) {
        std::srand(std::time(nullptr));
        is_ready = false;
    }

    void Chip8::copy_font_data() noexcept {
        for (auto i = 0; i < 80; ++i) {
            memory[i] = fontset[i];
        }
    }

    void Chip8::reset_state() {
        // just in case in future
        is_ready = false;

        framebuffer = {};
        memory      = {};
        V           = {};
        val         = {};
        keys        = {};

        while (!stack.empty()) {
            stack.pop_back();
        }

        I  = 0;
        PC = 0;

        cycle_count = 0;
        delay_timer = 0;
        sound_timer = 0;
    }

    uint16_t Chip8::fetch(uint16_t addr) {

        auto code = *reinterpret_cast<uint16_t*>(&memory[addr]);

        // CHIP8 is big endian, swap byte order
        code = swap_byte_order(code);

        return code;
    }

    op Chip8::decode(uint16_t opc) {

        // store each 4 bits of opcode into array
        for (auto i = 0; i < 4; ++i) {
            val[i] = (opc >> (4 * (3 - i))) & 0xF;
        }

        switch (val[0]) {
        case 0x0: {
            if (val[3] == 0xE)
                return op::RET;
            else if (val[2] == 0xE)
                return op::CLS;
            else
                return op::SYS;
        }
        case 0x1: {
            return op::JP;
        }
        case 0x2: {
            return op::CALL;
        }
        case 0x3: {
            return op::SE_I;
        }
        case 0x4: {
            return op::SNE_I;
        }
        case 0x5: {
            return op::SE_R;
        }
        case 0x6: {
            return op::LD_I;
        }
        case 0x7: {
            return op::ADD_I;
        }
        case 0x8: {
            switch (val[3]) {
            case 0x0: {
                return op::LD_R;
            }
            case 0x1: {
                return op::OR;
            }
            case 0x2: {
                return op::AND;
            }
            case 0x3: {
                return op::XOR;
            }
            case 0x4: {
                return op::ADD_R;
            }
            case 0x5: {
                return op::SUB;
            }
            case 0x6: {
                return op::SHR;
            }
            case 0x7: {
                return op::SUBN;
            }
            case 0xE: {
                return op::SHL;
            }
            default: {
                return op::UNKNOWN;
            }
            }
            break;
        }
        case 0x9: {
            return op::SNE_R;
        }
        case 0xA: {
            return op::LD_I2;
        }
        case 0xB: {
            return op::JP_V0;
        }
        case 0xC: {
            return op::RND;
        }
        case 0xD: {
            return op::DRW;
        }
        case 0xE: {
            switch (val[3]) {
            case 0xE: {
                return op::SKP;
            }
            case 0x1: {
                return op::SKNP;
            }
            default: {
                return op::UNKNOWN;
            }
            }
            break;
        }
        case 0xF: {
            switch (val[3]) {
            case 0x7: {
                return op::LD_DT;
            }
            case 0xA: {
                return op::LD_K;
            }
            case 0x8: {
                return op::LD_ST;
            }
            case 0xE: {
                return op::ADD_I2;
            }
            case 0x9: {
                return op::LD_F;
            }
            case 0x3: {
                return op::LD_B;
            }
            case 0x5: {
                switch (val[2]) {
                case 0x1: {
                    return op::LD_DT2;
                }
                case 0x5: {
                    return op::DUMP;
                }
                case 0x6: {
                    return op::LOAD;
                }
                default: {
                    return op::UNKNOWN;
                }
                }
                break;
            }
            default: {
                return op::UNKNOWN;
            }
            }
            break;
        }
        }
        return op::UNKNOWN;
    }

    void Chip8::execute() {
        // immediates commonly used
        auto imm4  = opcode & 0xF;
        auto imm8  = opcode & 0xFF;
        auto imm12 = opcode & 0xFFF;

        // references to typical Vx, Vy parameters
        auto& Vx = V[val[1]];
        auto& Vy = V[val[2]];

        switch (operation) {
            // for call/jump instructions, we unconditionally add 2 to PC every cycle
            // so by adjusting by -2, we can skip a conditional check
        case op::SYS: {
            stack.emplace_back(PC);
            PC = (imm12)-2;
            break;
        }
        case op::CLS: {

            for (auto& line : framebuffer) {
                for (auto& pixel : line) {
                    pixel = false;
                }
            }

            break;
        }
        case op::RET: {
            // get last PC from stack

            PC = stack.back();
            /*
            currently PC points to last CALL_SUB instruction, but we unconditionally
            increment anyway
         */
            stack.pop_back();
            break;
        }
        case op::JP: {
            PC = (imm12)-2;
            break;
        }
        case op::CALL: {
            // save PC
            stack.emplace_back(PC);
            PC = (imm12)-2;
            break;
        }
        case op::SE_I: {
            if (Vx == imm8) {
                PC += 2;
            }
            break;
        }
        case op::SNE_I: {
            if (Vx != imm8) {
                PC += 2;
            }
            break;
        }
        case op::SE_R: {
            if (Vx == Vy) {
                PC += 2;
            }
            break;
        }
        case op::LD_I: {
            Vx = imm8;
            break;
        }
        case op::ADD_I: {
            Vx += imm8;
            break;
        }
        case op::LD_R: {
            Vx = Vy;
            break;
        }
        case op::OR: {
            Vx |= Vy;
            break;
        }
        case op::AND: {
            Vx &= Vy;
            break;
        }
        case op::XOR: {
            Vx ^= Vy;
            break;
        }
        case op::ADD_R: {

            auto original = Vx;
            Vx += Vy;
            // no overflow case
            if (original < Vx) {
                V[0xF] = 0;
            }
            else {
                V[0xF] = 1;
            }
            break;
        }
        case op::SUB: {
            if (Vx > Vy) {
                V[0xF] = 1;
            }
            else {
                V[0xF] = 0;
            }
            Vx -= Vy;
            break;
        }
        case op::SHR: {
            V[0xF] = Vx & 0x1;
            Vx >>= 1;
            break;
        }
        case op::SUBN: {
            if (Vy > Vx) {
                V[0xF] = 1;
            }
            else {
                V[0xF] = 0;
            }
            Vx = Vy - Vx;
            break;
        }
        case op::SHL: {
            V[0xF] = Vx >> 7;
            Vx <<= 1;
            break;
        }
        case op::SNE_R: {
            if (Vx != Vy) {
                PC += 2;
            }
            break;
        }
        case op::LD_I2: {
            I = imm12;
            break;
        }
        case op::JP_V0: {
            PC = V[0x0] + imm12;
            break;
        }
        case op::RND: {
            uint8_t r = std::rand() & 0xFF;
            Vx        = r & imm8;
            break;
        }
        case op::DRW: {

            auto x = Vx;
            auto y = Vy;
            auto n = imm4;

            if (n == 0)
                n = 16;

            bool pixel_unset = false;

            // N lines down
            for (uint8_t i = 0; i < n; ++i) {
                std::bitset<8> line = memory[I + i];

                uint8_t currY = (y + i) % 32;

                // 8 pixels per line, to right
                for (uint8_t j = 0; j < 8; ++j) {

                    uint8_t currX = (x + j) % 64;

                    // 7 - j because 0 actually is least significant bit
                    if (!pixel_unset && framebuffer[currY][currX] && line[7 - j]) {
                        pixel_unset = true;
                    }
                    framebuffer[currY][currX] ^= line[7 - j];
                }
            }

            if (pixel_unset)
                V[0xF] = 1;
            else
                V[0xF] = 0;

            break;
        }
        case op::SKP: {
            if (keys[Vx]) {
                PC += 2;
            }
            break;
        }
        case op::SKNP: {
            if (!keys[Vx]) {
                PC += 2;
            }
            break;
        }
        case op::LD_DT: {
            Vx = delay_timer;
            break;
        }
        case op::LD_K: {
            bool wait = true;
            while (wait) {

                for (auto i = 0; i < 16; ++i) {
                    if (keys[i]) {
                        Vx   = static_cast<uint8_t>(i);
                        wait = false;
                        break;
                    }
                }
            }
            break;
        }
        case op::LD_DT2: {
            delay_timer = Vx;
            break;
        }
        case op::LD_ST: {
            sound_timer = Vx;
            break;
        }
        case op::ADD_I2: {
            I += Vx;
            break;
        }
        case op::LD_F: {
            uint8_t c = Vx;
            I         = c * 5;
            break;
        }
        case op::LD_B: {
            uint8_t first  = Vx / 100;
            uint8_t second = (Vx / 10) % 10;
            uint8_t third  = Vx % 10;

            memory[I]     = (first);
            memory[I + 1] = (second);
            memory[I + 2] = (third);
            break;
        }
        case op::DUMP: {
            auto ptr = I;
            for (auto i = 0; i <= val[1]; ++i) {
                memory[ptr++] = (V[i]);
            }
            break;
        }
        case op::LOAD: {
            auto ptr = I;
            for (auto i = 0; i <= val[1]; ++i) {
                V[i] = static_cast<uint8_t>(memory[ptr++]);
            }
            break;
        }
        case op::UNKNOWN: {
            std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
            break;
        }
        }
    }

    void Chip8::update_timers() {
        if (delay_timer > 0) {
            delay_timer--;
        }
        if (sound_timer > 0) {
            sound_timer--;
        }
    }

    void Chip8::cycle() {

        // 600Hz
        while (!timer.update())
            ;

        if (cycle_count % 10 == 1) {
            update_timers();
        }

        // fetch
        opcode = fetch(PC);
        // decode
        operation = decode(opcode);
        // execute
        execute();

        PC += 2;

        cycle_count++;
    }
}