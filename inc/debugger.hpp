#include "chip8.hpp"

// a wrapper around Chip8 class to allow for debugging functionality
// without cluttering chip8 class itself

class Debugger {
    std::array<uint8_t, 4096> prev_memory = {};
    std::array<uint8_t, 16>   prev_V      = {};

    uint16_t prev_I  = 0;
    uint16_t prev_PC = 0;

    uint8_t prev_dt = 0;
    uint8_t prev_st = 0;

    Chip8& proc;

    std::string opcode_instruction;
    const char* opcode_description;

public:
    std::array<bool, 16> reg_changes = {};

    bool I_change  = false;
    bool PC_change = false;
    bool dt_change = false;
    bool st_change = false;

    Debugger(Chip8& p);

    void single_step();

    void reset();
};
