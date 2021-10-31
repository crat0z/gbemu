#include <cstdint>
#include <array>
#include <stack>
#include <cstdlib>
#include <string>
#include "graphics.hpp"
#include "timer.hpp"

enum class op
{
    UNKNOWN,
    CALL, // 0NNN calls code at address NNN
    CLEAR, // 00E0 clear screen
    RET, // 00EE return
    GOTO, // 1NNN goto/jump to NNN
    CALL_SUB, // 2NNN call subroutine at NNN
    IF_IMM, // 3XNN skips next instruction if VX == NN
    IF_NOT_IMM, // 4XNN skips next instruction if VX != NN
    IF_REG, // 5XY0 skips next instruction if VX equals VY
    SET_IMM, // 6XNN sets VX to NN
    ADD_IMM, // 7XNN adds NN to VX (carry unchanged)
    SET_REG, // 8XY0 sets VX to value of VY
    OR, // 8XY1 sets VX to VX | VY
    AND, // 8XY2 sets VX to VX & VY
    XOR, // 8XY3 sets VX to VX ^ VY
    ADD_REG, // 8XY4 adds VY to VX, VF is set to 1 when there is a carry, 0 if not
    SUB_REG, // 8XY5 subs VY from VX, VF set to 0 if borrow, 1 if not
    RSHIFT, // 8XY6 Vx >>= 1 stores least significant bit of VX in VF
    SUB_REG2, // 8XY7 sets VX to VY - VX. VF is set to 0 if borrow, 1 if not
    LSHIFT, // 8XYE Vx <<= 1 stores most significant vit of VX in VF
    IF_NOT_REG, // 9XY0 skips next instruction if VX != VY
    SET_I, // ANNN sets I to address NNN
    JMP_OFFSET, // BNNN jumps to address NNN plus V0
    RAND_BITSET, // CXNN sets VX to result of bitwise AND on random number (0 to 255) and NN
    DRAW, // DXYN draws sprite at coordinate VX, VY that has width of 8 pixels and height of N pixels
    SKIP_KEY, // EX9E skips next instruction if key stored in VX is pressed
    SKIP_NOT_KEY, // EXA1 skips next instruction if key stored in VX is not pressed
    DELAY, // FX07 sets VX to value of delay timer
    GET_KEY, // FX0A wait for key press, then stored in VX
    SET_DELAY, // FX15 sets delay timer to VX
    SET_SOUND, // FX18 sets sound timer to VX
    ADD_I, // FX1E adds VX to I. VF not affected
    SET_I_SPRITE, // FX29 Sets I to the location of the sprite for the character in VX. Characters 0-F are represented by 4x5 font.
    STORE_BCD, // FX33 stores BCD rep. of VX, with most significant of three digits at address in I, middle at I+1, least at I+2
    REG_DUMP, // FX55 stores V0 to VX in memory starting at address I
    REG_LOAD // FX65 fills V0 to VX with values starting from address I
};

class Chip8 {

    Graphics graphics;

    std::array<uint8_t, 4096> memory = {};
    std::array<uint8_t, 16>   V      = {};

    uint16_t I;
    uint16_t PC;

    std::array<uint8_t, 4> val  = {};
    std::array<bool, 16>   keys = {};

    std::stack<uint16_t> stack;

    // current operation
    op operation;

    uint16_t opcode;

    uint8_t delay_timer;
    uint8_t sound_timer;

    // for delay and sound timer
    Timer timer;

    // for CPU freq
    Timer freq;

    bool should_draw;

    void fetch();
    op   decode();
    void execute();

    bool cycle();

    bool update_keys();
    void update_timers();

    void read_file(const std::string& name);

public:
    Chip8(const std::string& file_name, size_t f, int x, int y);

    void run();
};