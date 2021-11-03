#include "opcodes.hpp"
#include <array>
#include <fmt/format.h>

op_new decode(uint16_t opcode) {
    std::array<uint8_t, 4> val;

    // store each 4 bits of opcode into array
    for (auto i = 0; i < 4; ++i) {
        val[i] = (opcode >> (4 * (3 - i))) & 0xF;
    }

    switch (val[0]) {
    case 0x0: {
        if (val[3] == 0xE)
            return op_new::RET;
        else if (val[2] == 0xE)
            return op_new::CLS;
        else
            return op_new::SYS;
    }
    case 0x1: {
        return op_new::JP;
    }
    case 0x2: {
        return op_new::CALL;
    }
    case 0x3: {
        return op_new::SE_I;
    }
    case 0x4: {
        return op_new::SNE_I;
    }
    case 0x5: {
        return op_new::SE_R;
    }
    case 0x6: {
        return op_new::LD_I;
    }
    case 0x7: {
        return op_new::ADD_I;
    }
    case 0x8: {
        switch (val[3]) {
        case 0x0: {
            return op_new::LD_R;
        }
        case 0x1: {
            return op_new::OR;
        }
        case 0x2: {
            return op_new::AND;
        }
        case 0x3: {
            return op_new::XOR;
        }
        case 0x4: {
            return op_new::ADD_R;
        }
        case 0x5: {
            return op_new::SUB;
        }
        case 0x6: {
            return op_new::SHR;
        }
        case 0x7: {
            return op_new::SUBN;
        }
        case 0xE: {
            return op_new::SHL;
        }
        default: {
            return op_new::UNKNOWN;
        }
        }
        break;
    }
    case 0x9: {
        return op_new::SNE_R;
    }
    case 0xA: {
        return op_new::LD_I;
    }
    case 0xB: {
        return op_new::JP_V0;
    }
    case 0xC: {
        return op_new::RND;
    }
    case 0xD: {
        return op_new::DRW;
    }
    case 0xE: {
        switch (val[3]) {
        case 0xE: {
            return op_new::SKP;
        }
        case 0x1: {
            return op_new::SKNP;
        }
        default: {
            return op_new::UNKNOWN;
        }
        }
        break;
    }
    case 0xF: {
        switch (val[3]) {
        case 0x7: {
            return op_new::LD_DT;
        }
        case 0xA: {
            return op_new::LD_K;
        }
        case 0x8: {
            return op_new::LD_ST;
        }
        case 0xE: {
            return op_new::ADD_I2;
        }
        case 0x9: {
            return op_new::LD_F;
        }
        case 0x3: {
            return op_new::LD_B;
        }
        case 0x5: {
            switch (val[2]) {
            case 0x1: {
                return op_new::LD_DT2;
            }
            case 0x5: {
                return op_new::DUMP;
            }
            case 0x6: {
                return op_new::LOAD;
            }
            default: {
                return op_new::UNKNOWN;
            }
            }
            break;
        }
        default: {
            return op_new::UNKNOWN;
        }
        }
        break;
    }
    }
    return op_new::UNKNOWN;
}

std::string opcode_instruction(uint16_t opcode) {
    op_new o = decode(opcode);

    std::array<uint8_t, 4> val;

    // store each 4 bits of opcode into array
    for (auto i = 0; i < 4; ++i) {
        val[i] = (opcode >> (4 * (3 - i))) & 0xF;
    }

    auto imm4  = opcode & 0xF;
    auto imm8  = opcode & 0xFF;
    auto imm12 = opcode & 0xFFF;

    switch (o) {

    case op_new::SYS: {
        return fmt::format("SYS 0x{:03X}", imm12);
    }
    case op_new::CLS: {
        return "CLS";
    }
    case op_new::RET: {
        return "RET";
    }
    case op_new::JP: {
        return fmt::format("JP 0x{:03X}", imm12);
    }
    case op_new::CALL: {
        return fmt::format("CALL 0x{:03X}", imm12);
    }
    case op_new::SE_I: {
        return fmt::format("SE V{0:x}, {1}", val[1], imm8);
    }
    case op_new::SNE_I: {
        return fmt::format("SNE V{0:x}, {1}", val[1], imm8);
    }
    case op_new::SE_R: {
        return fmt::format("SE V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::LD_I: {
        return fmt::format("LD V{0:x}, {1}", val[1], imm8);
    }
    case op_new::ADD_I: {
        return fmt::format("ADD V{0:x}, {1}", val[1], imm8);
    }
    case op_new::LD_R: {
        return fmt::format("LD V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::OR: {
        return fmt::format("OR V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::AND: {
        return fmt::format("AND V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::XOR: {
        return fmt::format("XOR V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::ADD_R: {
        return fmt::format("ADD V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::SUB: {
        return fmt::format("SUB V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::SHR: {
        return fmt::format("SHR");
    }
    case op_new::SUBN: {
        return fmt::format("SUBN V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::SHL: {
        return fmt::format("SHL");
    }
    case op_new::SNE_R: {
        return fmt::format("SNE V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::LD_I2: {
        return fmt::format("LD I, 0x{:03X}", imm12);
    }
    case op_new::JP_V0: {
        return fmt::format("JP V0, 0x{:03X}", imm12);
    }
    case op_new::RND: {
        return fmt::format("RND V{0:x}, 0x{1:02X}", val[1], imm8);
    }
    case op_new::DRW: {
        return fmt::format("DRW V{0:x}, V{1:x}", val[1], val[2]);
    }
    case op_new::SKP: {
        return fmt::format("SKP V{0:x}", val[1]);
    }
    case op_new::SKNP: {
        return fmt::format("SKNP V{0:x}", val[1]);
    }
    case op_new::LD_DT: {
        return fmt::format("LD V{0:x}, DT", val[1]);
    }
    case op_new::LD_K: {
        return fmt::format("LD V{0:x}, K", val[1]);
    }
    case op_new::LD_DT2: {
        return fmt::format("LD DT, V{0:x}", val[1]);
    }
    case op_new::LD_ST: {
        return fmt::format("LD ST, V{0:x}", val[1]);
    }
    case op_new::ADD_I2: {
        return fmt::format("ADD I, V{0:x}", val[1]);
    }
    case op_new::LD_F: {
        return fmt::format("LD F, V{0:x}", val[1]);
    }
    case op_new::LD_B: {
        return fmt::format("LD B, V{0:x}", val[1]);
    }
    case op_new::DUMP: {
        return fmt::format("LD [I], V{0:x}", val[1]);
    }
    case op_new::LOAD: {
        return fmt::format("LD V{0:x}, [I]", val[1]);
    }
    case op_new::UNKNOWN: {
    default:
        return "Unknown";
    }
    }
}

const char* opcode_description(op_new opcode) {
    switch (opcode) {

    case op_new::SYS: {
        return "0NNN: Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN. Not necessary for most ROMs.";
    }
    case op_new::CLS: {
        return "00E0: Clears the screen.";
    }
    case op_new::RET: {
        return "00EE: Returns from a subroutine.";
    }
    case op_new::JP: {
        return "1NNN: Jumps to address NNN.";
    }
    case op_new::CALL: {
        return "2NNN: Calls subroutine at NNN.";
    }
    case op_new::SE_I: {
        return "3XNN: Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)";
    }
    case op_new::SNE_I: {
        return "4XNN: Skips the next instruction if VX does not equal NN. (Usually the next instruction is a jump to skip a code block)";
    }
    case op_new::SE_R: {
        return "5XY0: Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)";
    }
    case op_new::LD_I: {
        return "6XNN: Sets VX to NN.";
    }
    case op_new::ADD_I: {
        return "7XNN: Adds NN to VX. (Carry flag is not changed)";
    }
    case op_new::LD_R: {
        return "8XY0: Sets VX to the value of VY.";
    }
    case op_new::OR: {
        return "8XY1: Sets VX to VX or VY. (Bitwise OR operation)";
    }
    case op_new::AND: {
        return "8XY2: Sets VX to VX and VY. (Bitwise AND operation)";
    }
    case op_new::XOR: {
        return "8XY3: Sets VX to VX xor VY.";
    }
    case op_new::ADD_R: {
        return "8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not.";
    }
    case op_new::SUB: {
        return "8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not.";
    }
    case op_new::SHR: {
        return "8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1.";
    }
    case op_new::SUBN: {
        return "8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.";
    }
    case op_new::SHL: {
        return "8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1.";
    }
    case op_new::SNE_R: {
        return "9XY0: Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block)";
    }
    case op_new::LD_I2: {
        return "ANNN: Sets I to the address NNN";
    }
    case op_new::JP_V0: {
        return "BNNN: Jumps to the address NNN plus V0.";
    }
    case op_new::RND: {
        return "CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.";
    }
    case op_new::DRW: {
        return "DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value does not change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen";
    }
    case op_new::SKP: {
        return "EX9E: Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)";
    }
    case op_new::SKNP: {
        return "EXA1: Skips the next instruction if the key stored in VX is not pressed. (Usually the next instruction is a jump to skip a code block)";
    }
    case op_new::LD_DT: {
        return "FX07: Sets VX to the value of the delay timer.";
    }
    case op_new::LD_K: {
        return "FX0A: A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)";
    }
    case op_new::LD_DT2: {
        return "FX15: Sets the delay timer to VX.";
    }
    case op_new::LD_ST: {
        return "FX18: Sets the sound timer to VX.";
    }
    case op_new::ADD_I2: {
        return "FX1E: Adds VX to I. VF is not affected.";
    }
    case op_new::LD_F: {
        return "FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.";
    }
    case op_new::LD_B: {
        return "FX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)";
    }
    case op_new::DUMP: {
        return "FX55: Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.";
    }
    case op_new::LOAD: {
        return "FX65: Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.";
    }
    case op_new::UNKNOWN: {
    default:
        return "Unknown instruction";
    }
    }
}