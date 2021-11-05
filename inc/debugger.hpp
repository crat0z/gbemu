#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include "chip8.hpp"
#include <vector>

// a wrapper around Chip8 class to allow for debugging functionality
// without cluttering chip8 class itself

class Debugger {

    // std::vector<basic_block> res;

    std::array<uint8_t, 4096> prev_memory = {};
    std::array<uint8_t, 16>   prev_V      = {};

    uint16_t prev_I  = 0;
    uint16_t prev_PC = 0;

    uint8_t prev_dt = 0;
    uint8_t prev_st = 0;

    Chip8& proc;

    op          next_operation;
    uint16_t    next_opcode;
    std::string next_instruction;
    const char* next_description;

    void get_next_instruction() noexcept;
    void save_emu_state() noexcept;
    void get_changes() noexcept;

public:
    std::array<bool, 16> reg_changes = {};

    bool I_change  = false;
    bool PC_change = false;
    bool dt_change = false;
    bool st_change = false;

    Debugger(Chip8& p);

    void reset() noexcept;
    bool is_paused() const noexcept;
    void pause() noexcept;
    void run() noexcept;

    void single_step();

    const char* get_description() const noexcept;

    const char* get_instruction() const noexcept;

    uint16_t get_opcode() const noexcept;
};

#endif