#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>
#include <array>
#include <stack>
#include <string>
#include "timer.hpp"
#include "opcodes.hpp"

class Chip8 {
public:
    // Graphics graphics;

    std::array<std::array<bool, 64>, 32> framebuffer = {};

    std::array<uint8_t, 4096> memory = {};
    std::array<uint8_t, 16>   V      = {};

    uint16_t I = 0;
    uint16_t PC;

    std::array<uint8_t, 4> val  = {};
    std::array<bool, 16>   keys = {};

    std::stack<uint16_t> stack;

    // current operation
    op operation;

    uint16_t opcode;

    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;

    // for CPU freq
    CETimer<600> timer;

    size_t cycle_count = true;

    bool should_draw = false;

    uint16_t fetch(uint16_t addr);
    op       decode(uint16_t opc);
    void     execute();

    void cycle();

    void update_timers();

    void read_file(const std::string& name);

    Chip8(const std::string& file_name);
};

#endif