#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include "timer.hpp"
#include "opcodes.hpp"

class InspectableStack {

    std::vector<uint16_t> stack;

public:
    InspectableStack() = default;

    size_t size() const noexcept { return stack.size(); }

    void push_back(uint16_t val) noexcept { stack.push_back(val); }

    uint16_t pop_back() noexcept {
        auto ret = stack.back();
        stack.pop_back();
        return ret;
    }

    uint16_t& back() noexcept { return stack.back(); }

    void emplace_back(uint16_t val) noexcept { stack.emplace_back(val); }

    uint16_t& operator[](size_t index) noexcept { return stack[index]; }

    bool empty() const noexcept { return stack.empty(); }
};

constexpr uint16_t CHIP8_DEFAULT_ENTRY = 0x200;

class Chip8 {

private:
    friend class Debugger;

    void copy_font_data() noexcept;

    std::array<uint8_t, 4096> memory = {};
    std::array<uint8_t, 16>   V      = {};
    std::array<uint8_t, 4>    val    = {};

    uint16_t I = 0;
    uint16_t PC;

    InspectableStack stack;

    op operation;

    uint16_t opcode;

    size_t cycle_count;

    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;

    // for CPU freq
    CETimer<600> timer;

    bool is_ready;

    uint16_t entry_point;
    uint16_t base_address;

    uint16_t fetch(uint16_t addr);
    op       decode(uint16_t opc);
    void     execute();

    void cycle();

    void update_timers();

    // read file on filesystem with name, write to emu memory beginning @ addr
    void read_file(const std::string& name, uint16_t addr);
    void reset_state();

public:
    std::array<std::array<bool, 64>, 32> framebuffer = {};

    std::array<bool, 16> keys = {};

    Chip8();
    // new game with filepath, entry and writing memory to addr
    void new_game(const std::string& filepath, uint16_t entry, uint16_t addr, bool paused);
};

#endif