#ifndef OPCODES_HPP
#define OPCODES_HPP

#include <string>

enum class op
{
    UNKNOWN,
    SYS, // 0NNN calls code at address NNN
    CLS, // 00E0 clear screen
    RET, // 00EE return
    JP, // 1NNN goto/jump to NNN
    CALL, // 2NNN call subroutine at NNN
    SE_I, // 3XNN skips next instruction if VX == NN
    SNE_I, // 4XNN skips next instruction if VX != NN
    SE_R, // 5XY0 skips next instruction if VX equals VY
    LD_I, // 6XNN sets VX to NN
    ADD_I, // 7XNN adds NN to VX (carry unchanged)
    LD_R, // 8XY0 sets VX to value of VY
    OR, // 8XY1 sets VX to VX | VY
    AND, // 8XY2 sets VX to VX & VY
    XOR, // 8XY3 sets VX to VX ^ VY
    ADD_R, // 8XY4 adds VY to VX, VF is set to 1 when there is a carry, 0 if not
    SUB, // 8XY5 subs VY from VX, VF set to 0 if borrow, 1 if not
    SHR, // 8XY6 Vx >>= 1 stores least significant bit of VX in VF
    SUBN, // 8XY7 sets VX to VY - VX. VF is set to 0 if borrow, 1 if not
    SHL, // 8XYE Vx <<= 1 stores most significant vit of VX in VF
    SNE_R, // 9XY0 skips next instruction if VX != VY
    LD_I2, // ANNN sets I to address NNN
    JP_V0, // BNNN jumps to address NNN plus V0
    RND, // CXNN sets VX to result of bitwise AND on random number (0 to 255) and NN
    DRW, // DXYN draws sprite at coordinate VX, VY that has width of 8 pixels and height of N pixels
    SKP, // EX9E skips next instruction if key stored in VX is pressed
    SKNP, // EXA1 skips next instruction if key stored in VX is not pressed
    LD_DT, // FX07 sets VX to value of delay timer
    LD_K, // FX0A wait for key press, then stored in VX
    LD_DT2, // FX15 sets delay timer to VX
    LD_ST, // FX18 sets sound timer to VX
    ADD_I2, // FX1E adds VX to I. VF not affected
    LD_F, // FX29 Sets I to the location of the sprite for the character in VX. Characters 0-F are represented by 4x5 font.
    LD_B, // FX33 stores BCD rep. of VX, with most significant of three digits at address in I, middle at I+1, least at I+2
    DUMP, // FX55 stores V0 to VX in memory starting at address I
    LOAD // FX65 fills V0 to VX with values starting from address I
};

bool is_jump_or_call(op opcode);
bool is_followable(op opcode);

op decode(uint16_t opcode);

std::string opcode_mnemonic(uint16_t opcode);

const char* opcode_description(uint16_t opcode);
const char* opcode_description(op opcode);

#endif