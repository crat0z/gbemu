#include <fmt/format.h>
#include "core/gameboy.hpp"

std::string opcode_str_0x00([[maybe_unused]] Context& ctx) { return std::string("NOP"); }
std::string opcode_str_0x01([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "BC", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0x02([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(BC)", 'A');
}
std::string opcode_str_0x03([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", "BC"); }
std::string opcode_str_0x04([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", 'B'); }
std::string opcode_str_0x05([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", 'B'); }
std::string opcode_str_0x06([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'B', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0x07([[maybe_unused]] Context& ctx) { return std::string("RLCA"); }
std::string opcode_str_0x08([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", fmt::format("({:#06x})", ctx.peek16()), "SP");
}
std::string opcode_str_0x09([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", "HL", "BC");
}
std::string opcode_str_0x0a([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', "(BC)");
}
std::string opcode_str_0x0b([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", "BC"); }
std::string opcode_str_0x0c([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", 'C'); }
std::string opcode_str_0x0d([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", 'C'); }
std::string opcode_str_0x0e([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'C', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0x0f([[maybe_unused]] Context& ctx) { return std::string("RRCA"); }
std::string opcode_str_0x10([[maybe_unused]] Context& ctx) { return fmt::format("STOP {}", 0); }
std::string opcode_str_0x11([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "DE", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0x12([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(DE)", 'A');
}
std::string opcode_str_0x13([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", "DE"); }
std::string opcode_str_0x14([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", 'D'); }
std::string opcode_str_0x15([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", 'D'); }
std::string opcode_str_0x16([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'D', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0x17([[maybe_unused]] Context& ctx) { return std::string("RLA"); }
std::string opcode_str_0x18([[maybe_unused]] Context& ctx) {
    return fmt::format("JR {}", ctx.peek8_signed());
}
std::string opcode_str_0x19([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", "HL", "DE");
}
std::string opcode_str_0x1a([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', "(DE)");
}
std::string opcode_str_0x1b([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", "DE"); }
std::string opcode_str_0x1c([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", 'E'); }
std::string opcode_str_0x1d([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", 'E'); }
std::string opcode_str_0x1e([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'E', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0x1f([[maybe_unused]] Context& ctx) { return std::string("RRA"); }
std::string opcode_str_0x20([[maybe_unused]] Context& ctx) {
    return fmt::format("JR {},{}", "NZ", ctx.peek8_signed());
}
std::string opcode_str_0x21([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "HL", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0x22([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL+)", 'A');
}
std::string opcode_str_0x23([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", "HL"); }
std::string opcode_str_0x24([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", 'H'); }
std::string opcode_str_0x25([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", 'H'); }
std::string opcode_str_0x26([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'H', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0x27([[maybe_unused]] Context& ctx) { return std::string("DAA"); }
std::string opcode_str_0x28([[maybe_unused]] Context& ctx) {
    return fmt::format("JR {},{}", 'Z', ctx.peek8_signed());
}
std::string opcode_str_0x29([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", "HL", "HL");
}
std::string opcode_str_0x2a([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', "(HL+)");
}
std::string opcode_str_0x2b([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", "HL"); }
std::string opcode_str_0x2c([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", 'L'); }
std::string opcode_str_0x2d([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", 'L'); }
std::string opcode_str_0x2e([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'L', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0x2f([[maybe_unused]] Context& ctx) { return std::string("CPL"); }
std::string opcode_str_0x30([[maybe_unused]] Context& ctx) {
    return fmt::format("JR {},{}", "NC", ctx.peek8_signed());
}
std::string opcode_str_0x31([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "SP", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0x32([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL-)", 'A');
}
std::string opcode_str_0x33([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", "SP"); }
std::string opcode_str_0x34([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", "(HL)"); }
std::string opcode_str_0x35([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", "(HL)"); }
std::string opcode_str_0x36([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL)", fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0x37([[maybe_unused]] Context& ctx) { return std::string("SCF"); }
std::string opcode_str_0x38([[maybe_unused]] Context& ctx) {
    return fmt::format("JR {},{}", 'C', ctx.peek8_signed());
}
std::string opcode_str_0x39([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", "HL", "SP");
}
std::string opcode_str_0x3a([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', "(HL-)");
}
std::string opcode_str_0x3b([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", "SP"); }
std::string opcode_str_0x3c([[maybe_unused]] Context& ctx) { return fmt::format("INC {}", 'A'); }
std::string opcode_str_0x3d([[maybe_unused]] Context& ctx) { return fmt::format("DEC {}", 'A'); }
std::string opcode_str_0x3e([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0x3f([[maybe_unused]] Context& ctx) { return std::string("CCF"); }
std::string opcode_str_0x40([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'B', 'B');
}
std::string opcode_str_0x41([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'B', 'C');
}
std::string opcode_str_0x42([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'B', 'D');
}
std::string opcode_str_0x43([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'B', 'E');
}
std::string opcode_str_0x44([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'B', 'H');
}
std::string opcode_str_0x45([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'B', 'L');
}
std::string opcode_str_0x46([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'B', "(HL)");
}
std::string opcode_str_0x47([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'B', 'A');
}
std::string opcode_str_0x48([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'C', 'B');
}
std::string opcode_str_0x49([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'C', 'C');
}
std::string opcode_str_0x4a([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'C', 'D');
}
std::string opcode_str_0x4b([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'C', 'E');
}
std::string opcode_str_0x4c([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'C', 'H');
}
std::string opcode_str_0x4d([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'C', 'L');
}
std::string opcode_str_0x4e([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'C', "(HL)");
}
std::string opcode_str_0x4f([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'C', 'A');
}
std::string opcode_str_0x50([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'D', 'B');
}
std::string opcode_str_0x51([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'D', 'C');
}
std::string opcode_str_0x52([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'D', 'D');
}
std::string opcode_str_0x53([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'D', 'E');
}
std::string opcode_str_0x54([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'D', 'H');
}
std::string opcode_str_0x55([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'D', 'L');
}
std::string opcode_str_0x56([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'D', "(HL)");
}
std::string opcode_str_0x57([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'D', 'A');
}
std::string opcode_str_0x58([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'E', 'B');
}
std::string opcode_str_0x59([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'E', 'C');
}
std::string opcode_str_0x5a([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'E', 'D');
}
std::string opcode_str_0x5b([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'E', 'E');
}
std::string opcode_str_0x5c([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'E', 'H');
}
std::string opcode_str_0x5d([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'E', 'L');
}
std::string opcode_str_0x5e([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'E', "(HL)");
}
std::string opcode_str_0x5f([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'E', 'A');
}
std::string opcode_str_0x60([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'H', 'B');
}
std::string opcode_str_0x61([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'H', 'C');
}
std::string opcode_str_0x62([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'H', 'D');
}
std::string opcode_str_0x63([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'H', 'E');
}
std::string opcode_str_0x64([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'H', 'H');
}
std::string opcode_str_0x65([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'H', 'L');
}
std::string opcode_str_0x66([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'H', "(HL)");
}
std::string opcode_str_0x67([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'H', 'A');
}
std::string opcode_str_0x68([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'L', 'B');
}
std::string opcode_str_0x69([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'L', 'C');
}
std::string opcode_str_0x6a([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'L', 'D');
}
std::string opcode_str_0x6b([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'L', 'E');
}
std::string opcode_str_0x6c([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'L', 'H');
}
std::string opcode_str_0x6d([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'L', 'L');
}
std::string opcode_str_0x6e([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'L', "(HL)");
}
std::string opcode_str_0x6f([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'L', 'A');
}
std::string opcode_str_0x70([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL)", 'B');
}
std::string opcode_str_0x71([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL)", 'C');
}
std::string opcode_str_0x72([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL)", 'D');
}
std::string opcode_str_0x73([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL)", 'E');
}
std::string opcode_str_0x74([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL)", 'H');
}
std::string opcode_str_0x75([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL)", 'L');
}
std::string opcode_str_0x76([[maybe_unused]] Context& ctx) { return std::string("HALT"); }
std::string opcode_str_0x77([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(HL)", 'A');
}
std::string opcode_str_0x78([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', 'B');
}
std::string opcode_str_0x79([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', 'C');
}
std::string opcode_str_0x7a([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', 'D');
}
std::string opcode_str_0x7b([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', 'E');
}
std::string opcode_str_0x7c([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', 'H');
}
std::string opcode_str_0x7d([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', 'L');
}
std::string opcode_str_0x7e([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', "(HL)");
}
std::string opcode_str_0x7f([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', 'A');
}
std::string opcode_str_0x80([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", 'A', 'B');
}
std::string opcode_str_0x81([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", 'A', 'C');
}
std::string opcode_str_0x82([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", 'A', 'D');
}
std::string opcode_str_0x83([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", 'A', 'E');
}
std::string opcode_str_0x84([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", 'A', 'H');
}
std::string opcode_str_0x85([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", 'A', 'L');
}
std::string opcode_str_0x86([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", 'A', "(HL)");
}
std::string opcode_str_0x87([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", 'A', 'A');
}
std::string opcode_str_0x88([[maybe_unused]] Context& ctx) {
    return fmt::format("ADC {},{}", 'A', 'B');
}
std::string opcode_str_0x89([[maybe_unused]] Context& ctx) {
    return fmt::format("ADC {},{}", 'A', 'C');
}
std::string opcode_str_0x8a([[maybe_unused]] Context& ctx) {
    return fmt::format("ADC {},{}", 'A', 'D');
}
std::string opcode_str_0x8b([[maybe_unused]] Context& ctx) {
    return fmt::format("ADC {},{}", 'A', 'E');
}
std::string opcode_str_0x8c([[maybe_unused]] Context& ctx) {
    return fmt::format("ADC {},{}", 'A', 'H');
}
std::string opcode_str_0x8d([[maybe_unused]] Context& ctx) {
    return fmt::format("ADC {},{}", 'A', 'L');
}
std::string opcode_str_0x8e([[maybe_unused]] Context& ctx) {
    return fmt::format("ADC {},{}", 'A', "(HL)");
}
std::string opcode_str_0x8f([[maybe_unused]] Context& ctx) {
    return fmt::format("ADC {},{}", 'A', 'A');
}
std::string opcode_str_0x90([[maybe_unused]] Context& ctx) { return fmt::format("SUB {}", 'B'); }
std::string opcode_str_0x91([[maybe_unused]] Context& ctx) { return fmt::format("SUB {}", 'C'); }
std::string opcode_str_0x92([[maybe_unused]] Context& ctx) { return fmt::format("SUB {}", 'D'); }
std::string opcode_str_0x93([[maybe_unused]] Context& ctx) { return fmt::format("SUB {}", 'E'); }
std::string opcode_str_0x94([[maybe_unused]] Context& ctx) { return fmt::format("SUB {}", 'H'); }
std::string opcode_str_0x95([[maybe_unused]] Context& ctx) { return fmt::format("SUB {}", 'L'); }
std::string opcode_str_0x96([[maybe_unused]] Context& ctx) { return fmt::format("SUB {}", "(HL)"); }
std::string opcode_str_0x97([[maybe_unused]] Context& ctx) { return fmt::format("SUB {}", 'A'); }
std::string opcode_str_0x98([[maybe_unused]] Context& ctx) {
    return fmt::format("SBC {},{}", 'A', 'B');
}
std::string opcode_str_0x99([[maybe_unused]] Context& ctx) {
    return fmt::format("SBC {},{}", 'A', 'C');
}
std::string opcode_str_0x9a([[maybe_unused]] Context& ctx) {
    return fmt::format("SBC {},{}", 'A', 'D');
}
std::string opcode_str_0x9b([[maybe_unused]] Context& ctx) {
    return fmt::format("SBC {},{}", 'A', 'E');
}
std::string opcode_str_0x9c([[maybe_unused]] Context& ctx) {
    return fmt::format("SBC {},{}", 'A', 'H');
}
std::string opcode_str_0x9d([[maybe_unused]] Context& ctx) {
    return fmt::format("SBC {},{}", 'A', 'L');
}
std::string opcode_str_0x9e([[maybe_unused]] Context& ctx) {
    return fmt::format("SBC {},{}", 'A', "(HL)");
}
std::string opcode_str_0x9f([[maybe_unused]] Context& ctx) {
    return fmt::format("SBC {},{}", 'A', 'A');
}
std::string opcode_str_0xa0([[maybe_unused]] Context& ctx) { return fmt::format("AND {}", 'B'); }
std::string opcode_str_0xa1([[maybe_unused]] Context& ctx) { return fmt::format("AND {}", 'C'); }
std::string opcode_str_0xa2([[maybe_unused]] Context& ctx) { return fmt::format("AND {}", 'D'); }
std::string opcode_str_0xa3([[maybe_unused]] Context& ctx) { return fmt::format("AND {}", 'E'); }
std::string opcode_str_0xa4([[maybe_unused]] Context& ctx) { return fmt::format("AND {}", 'H'); }
std::string opcode_str_0xa5([[maybe_unused]] Context& ctx) { return fmt::format("AND {}", 'L'); }
std::string opcode_str_0xa6([[maybe_unused]] Context& ctx) { return fmt::format("AND {}", "(HL)"); }
std::string opcode_str_0xa7([[maybe_unused]] Context& ctx) { return fmt::format("AND {}", 'A'); }
std::string opcode_str_0xa8([[maybe_unused]] Context& ctx) { return fmt::format("XOR {}", 'B'); }
std::string opcode_str_0xa9([[maybe_unused]] Context& ctx) { return fmt::format("XOR {}", 'C'); }
std::string opcode_str_0xaa([[maybe_unused]] Context& ctx) { return fmt::format("XOR {}", 'D'); }
std::string opcode_str_0xab([[maybe_unused]] Context& ctx) { return fmt::format("XOR {}", 'E'); }
std::string opcode_str_0xac([[maybe_unused]] Context& ctx) { return fmt::format("XOR {}", 'H'); }
std::string opcode_str_0xad([[maybe_unused]] Context& ctx) { return fmt::format("XOR {}", 'L'); }
std::string opcode_str_0xae([[maybe_unused]] Context& ctx) { return fmt::format("XOR {}", "(HL)"); }
std::string opcode_str_0xaf([[maybe_unused]] Context& ctx) { return fmt::format("XOR {}", 'A'); }
std::string opcode_str_0xb0([[maybe_unused]] Context& ctx) { return fmt::format("OR {}", 'B'); }
std::string opcode_str_0xb1([[maybe_unused]] Context& ctx) { return fmt::format("OR {}", 'C'); }
std::string opcode_str_0xb2([[maybe_unused]] Context& ctx) { return fmt::format("OR {}", 'D'); }
std::string opcode_str_0xb3([[maybe_unused]] Context& ctx) { return fmt::format("OR {}", 'E'); }
std::string opcode_str_0xb4([[maybe_unused]] Context& ctx) { return fmt::format("OR {}", 'H'); }
std::string opcode_str_0xb5([[maybe_unused]] Context& ctx) { return fmt::format("OR {}", 'L'); }
std::string opcode_str_0xb6([[maybe_unused]] Context& ctx) { return fmt::format("OR {}", "(HL)"); }
std::string opcode_str_0xb7([[maybe_unused]] Context& ctx) { return fmt::format("OR {}", 'A'); }
std::string opcode_str_0xb8([[maybe_unused]] Context& ctx) { return fmt::format("CP {}", 'B'); }
std::string opcode_str_0xb9([[maybe_unused]] Context& ctx) { return fmt::format("CP {}", 'C'); }
std::string opcode_str_0xba([[maybe_unused]] Context& ctx) { return fmt::format("CP {}", 'D'); }
std::string opcode_str_0xbb([[maybe_unused]] Context& ctx) { return fmt::format("CP {}", 'E'); }
std::string opcode_str_0xbc([[maybe_unused]] Context& ctx) { return fmt::format("CP {}", 'H'); }
std::string opcode_str_0xbd([[maybe_unused]] Context& ctx) { return fmt::format("CP {}", 'L'); }
std::string opcode_str_0xbe([[maybe_unused]] Context& ctx) { return fmt::format("CP {}", "(HL)"); }
std::string opcode_str_0xbf([[maybe_unused]] Context& ctx) { return fmt::format("CP {}", 'A'); }
std::string opcode_str_0xc0([[maybe_unused]] Context& ctx) { return fmt::format("RET {}", "NZ"); }
std::string opcode_str_0xc1([[maybe_unused]] Context& ctx) { return fmt::format("POP {}", "BC"); }
std::string opcode_str_0xc2([[maybe_unused]] Context& ctx) {
    return fmt::format("JP {},{}", "NZ", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xc3([[maybe_unused]] Context& ctx) {
    return fmt::format("JP {}", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xc4([[maybe_unused]] Context& ctx) {
    return fmt::format("CALL {},{}", "NZ", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xc5([[maybe_unused]] Context& ctx) { return fmt::format("PUSH {}", "BC"); }
std::string opcode_str_0xc6([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", 'A', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0xc7([[maybe_unused]] Context& ctx) { return fmt::format("RST {}", "0x00"); }
std::string opcode_str_0xc8([[maybe_unused]] Context& ctx) { return fmt::format("RET {}", 'Z'); }
std::string opcode_str_0xc9([[maybe_unused]] Context& ctx) { return std::string("RET"); }
std::string opcode_str_0xca([[maybe_unused]] Context& ctx) {
    return fmt::format("JP {},{}", 'Z', fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xcc([[maybe_unused]] Context& ctx) {
    return fmt::format("CALL {},{}", 'Z', fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xcd([[maybe_unused]] Context& ctx) {
    return fmt::format("CALL {}", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xce([[maybe_unused]] Context& ctx) {
    return fmt::format("ADC {},{}", 'A', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0xcf([[maybe_unused]] Context& ctx) { return fmt::format("RST {}", "0x08"); }
std::string opcode_str_0xd0([[maybe_unused]] Context& ctx) { return fmt::format("RET {}", "NC"); }
std::string opcode_str_0xd1([[maybe_unused]] Context& ctx) { return fmt::format("POP {}", "DE"); }
std::string opcode_str_0xd2([[maybe_unused]] Context& ctx) {
    return fmt::format("JP {},{}", "NC", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xd3([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xd4([[maybe_unused]] Context& ctx) {
    return fmt::format("CALL {},{}", "NC", fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xd5([[maybe_unused]] Context& ctx) { return fmt::format("PUSH {}", "DE"); }
std::string opcode_str_0xd6([[maybe_unused]] Context& ctx) {
    return fmt::format("SUB {}", fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0xd7([[maybe_unused]] Context& ctx) { return fmt::format("RST {}", "0x10"); }
std::string opcode_str_0xd8([[maybe_unused]] Context& ctx) { return fmt::format("RET {}", 'C'); }
std::string opcode_str_0xd9([[maybe_unused]] Context& ctx) { return std::string("RETI"); }
std::string opcode_str_0xda([[maybe_unused]] Context& ctx) {
    return fmt::format("JP {},{}", 'C', fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xdb([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xdc([[maybe_unused]] Context& ctx) {
    return fmt::format("CALL {},{}", 'C', fmt::format("{:#06x}", ctx.peek16()));
}
std::string opcode_str_0xdd([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xde([[maybe_unused]] Context& ctx) {
    return fmt::format("SBC {},{}", 'A', fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0xdf([[maybe_unused]] Context& ctx) { return fmt::format("RST {}", "0x18"); }
std::string opcode_str_0xe0([[maybe_unused]] Context& ctx) {
    return fmt::format("LDH {},{}", fmt::format("({:#04x})", ctx.peek8()), 'A');
}
std::string opcode_str_0xe1([[maybe_unused]] Context& ctx) { return fmt::format("POP {}", "HL"); }
std::string opcode_str_0xe2([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "(C)", 'A');
}
std::string opcode_str_0xe3([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xe4([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xe5([[maybe_unused]] Context& ctx) { return fmt::format("PUSH {}", "HL"); }
std::string opcode_str_0xe6([[maybe_unused]] Context& ctx) {
    return fmt::format("AND {}", fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0xe7([[maybe_unused]] Context& ctx) { return fmt::format("RST {}", "0x20"); }
std::string opcode_str_0xe8([[maybe_unused]] Context& ctx) {
    return fmt::format("ADD {},{}", "SP", ctx.peek8_signed());
}
std::string opcode_str_0xe9([[maybe_unused]] Context& ctx) { return fmt::format("JP {}", "(HL)"); }
std::string opcode_str_0xea([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", fmt::format("({:#06x})", ctx.peek16()), 'A');
}
std::string opcode_str_0xeb([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xec([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xed([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xee([[maybe_unused]] Context& ctx) {
    return fmt::format("XOR {}", fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0xef([[maybe_unused]] Context& ctx) { return fmt::format("RST {}", "0x28"); }
std::string opcode_str_0xf0([[maybe_unused]] Context& ctx) {
    return fmt::format("LDH {},{}", 'A', fmt::format("({:#04x})", ctx.peek8()));
}
std::string opcode_str_0xf1([[maybe_unused]] Context& ctx) { return fmt::format("POP {}", "AF"); }
std::string opcode_str_0xf2([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', "(C)");
}
std::string opcode_str_0xf3([[maybe_unused]] Context& ctx) { return std::string("DI"); }
std::string opcode_str_0xf4([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xf5([[maybe_unused]] Context& ctx) { return fmt::format("PUSH {}", "AF"); }
std::string opcode_str_0xf6([[maybe_unused]] Context& ctx) {
    return fmt::format("OR {}", fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0xf7([[maybe_unused]] Context& ctx) { return fmt::format("RST {}", "0x30"); }
std::string opcode_str_0xf8([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "HL", fmt::format("SP+{:+}", ctx.peek8_signed()));
}
std::string opcode_str_0xf9([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", "SP", "HL");
}
std::string opcode_str_0xfa([[maybe_unused]] Context& ctx) {
    return fmt::format("LD {},{}", 'A', fmt::format("({:#06x})", ctx.peek16()));
}
std::string opcode_str_0xfb([[maybe_unused]] Context& ctx) { return std::string("EI"); }
std::string opcode_str_0xfc([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xfd([[maybe_unused]] Context& ctx) { return "UNKNOWN"; }
std::string opcode_str_0xfe([[maybe_unused]] Context& ctx) {
    return fmt::format("CP {}", fmt::format("{:#04x}", ctx.peek8()));
}
std::string opcode_str_0xff([[maybe_unused]] Context& ctx) { return fmt::format("RST {}", "0x38"); }
std::string opcode_str_0xcb00([[maybe_unused]] Context& ctx) { return fmt::format("RLC {}", 'B'); }
std::string opcode_str_0xcb01([[maybe_unused]] Context& ctx) { return fmt::format("RLC {}", 'C'); }
std::string opcode_str_0xcb02([[maybe_unused]] Context& ctx) { return fmt::format("RLC {}", 'D'); }
std::string opcode_str_0xcb03([[maybe_unused]] Context& ctx) { return fmt::format("RLC {}", 'E'); }
std::string opcode_str_0xcb04([[maybe_unused]] Context& ctx) { return fmt::format("RLC {}", 'H'); }
std::string opcode_str_0xcb05([[maybe_unused]] Context& ctx) { return fmt::format("RLC {}", 'L'); }
std::string opcode_str_0xcb06([[maybe_unused]] Context& ctx) {
    return fmt::format("RLC {}", "(HL)");
}
std::string opcode_str_0xcb07([[maybe_unused]] Context& ctx) { return fmt::format("RLC {}", 'A'); }
std::string opcode_str_0xcb08([[maybe_unused]] Context& ctx) { return fmt::format("RRC {}", 'B'); }
std::string opcode_str_0xcb09([[maybe_unused]] Context& ctx) { return fmt::format("RRC {}", 'C'); }
std::string opcode_str_0xcb0a([[maybe_unused]] Context& ctx) { return fmt::format("RRC {}", 'D'); }
std::string opcode_str_0xcb0b([[maybe_unused]] Context& ctx) { return fmt::format("RRC {}", 'E'); }
std::string opcode_str_0xcb0c([[maybe_unused]] Context& ctx) { return fmt::format("RRC {}", 'H'); }
std::string opcode_str_0xcb0d([[maybe_unused]] Context& ctx) { return fmt::format("RRC {}", 'L'); }
std::string opcode_str_0xcb0e([[maybe_unused]] Context& ctx) {
    return fmt::format("RRC {}", "(HL)");
}
std::string opcode_str_0xcb0f([[maybe_unused]] Context& ctx) { return fmt::format("RRC {}", 'A'); }
std::string opcode_str_0xcb10([[maybe_unused]] Context& ctx) { return fmt::format("RL {}", 'B'); }
std::string opcode_str_0xcb11([[maybe_unused]] Context& ctx) { return fmt::format("RL {}", 'C'); }
std::string opcode_str_0xcb12([[maybe_unused]] Context& ctx) { return fmt::format("RL {}", 'D'); }
std::string opcode_str_0xcb13([[maybe_unused]] Context& ctx) { return fmt::format("RL {}", 'E'); }
std::string opcode_str_0xcb14([[maybe_unused]] Context& ctx) { return fmt::format("RL {}", 'H'); }
std::string opcode_str_0xcb15([[maybe_unused]] Context& ctx) { return fmt::format("RL {}", 'L'); }
std::string opcode_str_0xcb16([[maybe_unused]] Context& ctx) {
    return fmt::format("RL {}", "(HL)");
}
std::string opcode_str_0xcb17([[maybe_unused]] Context& ctx) { return fmt::format("RL {}", 'A'); }
std::string opcode_str_0xcb18([[maybe_unused]] Context& ctx) { return fmt::format("RR {}", 'B'); }
std::string opcode_str_0xcb19([[maybe_unused]] Context& ctx) { return fmt::format("RR {}", 'C'); }
std::string opcode_str_0xcb1a([[maybe_unused]] Context& ctx) { return fmt::format("RR {}", 'D'); }
std::string opcode_str_0xcb1b([[maybe_unused]] Context& ctx) { return fmt::format("RR {}", 'E'); }
std::string opcode_str_0xcb1c([[maybe_unused]] Context& ctx) { return fmt::format("RR {}", 'H'); }
std::string opcode_str_0xcb1d([[maybe_unused]] Context& ctx) { return fmt::format("RR {}", 'L'); }
std::string opcode_str_0xcb1e([[maybe_unused]] Context& ctx) {
    return fmt::format("RR {}", "(HL)");
}
std::string opcode_str_0xcb1f([[maybe_unused]] Context& ctx) { return fmt::format("RR {}", 'A'); }
std::string opcode_str_0xcb20([[maybe_unused]] Context& ctx) { return fmt::format("SLA {}", 'B'); }
std::string opcode_str_0xcb21([[maybe_unused]] Context& ctx) { return fmt::format("SLA {}", 'C'); }
std::string opcode_str_0xcb22([[maybe_unused]] Context& ctx) { return fmt::format("SLA {}", 'D'); }
std::string opcode_str_0xcb23([[maybe_unused]] Context& ctx) { return fmt::format("SLA {}", 'E'); }
std::string opcode_str_0xcb24([[maybe_unused]] Context& ctx) { return fmt::format("SLA {}", 'H'); }
std::string opcode_str_0xcb25([[maybe_unused]] Context& ctx) { return fmt::format("SLA {}", 'L'); }
std::string opcode_str_0xcb26([[maybe_unused]] Context& ctx) {
    return fmt::format("SLA {}", "(HL)");
}
std::string opcode_str_0xcb27([[maybe_unused]] Context& ctx) { return fmt::format("SLA {}", 'A'); }
std::string opcode_str_0xcb28([[maybe_unused]] Context& ctx) { return fmt::format("SRA {}", 'B'); }
std::string opcode_str_0xcb29([[maybe_unused]] Context& ctx) { return fmt::format("SRA {}", 'C'); }
std::string opcode_str_0xcb2a([[maybe_unused]] Context& ctx) { return fmt::format("SRA {}", 'D'); }
std::string opcode_str_0xcb2b([[maybe_unused]] Context& ctx) { return fmt::format("SRA {}", 'E'); }
std::string opcode_str_0xcb2c([[maybe_unused]] Context& ctx) { return fmt::format("SRA {}", 'H'); }
std::string opcode_str_0xcb2d([[maybe_unused]] Context& ctx) { return fmt::format("SRA {}", 'L'); }
std::string opcode_str_0xcb2e([[maybe_unused]] Context& ctx) {
    return fmt::format("SRA {}", "(HL)");
}
std::string opcode_str_0xcb2f([[maybe_unused]] Context& ctx) { return fmt::format("SRA {}", 'A'); }
std::string opcode_str_0xcb30([[maybe_unused]] Context& ctx) { return fmt::format("SWAP {}", 'B'); }
std::string opcode_str_0xcb31([[maybe_unused]] Context& ctx) { return fmt::format("SWAP {}", 'C'); }
std::string opcode_str_0xcb32([[maybe_unused]] Context& ctx) { return fmt::format("SWAP {}", 'D'); }
std::string opcode_str_0xcb33([[maybe_unused]] Context& ctx) { return fmt::format("SWAP {}", 'E'); }
std::string opcode_str_0xcb34([[maybe_unused]] Context& ctx) { return fmt::format("SWAP {}", 'H'); }
std::string opcode_str_0xcb35([[maybe_unused]] Context& ctx) { return fmt::format("SWAP {}", 'L'); }
std::string opcode_str_0xcb36([[maybe_unused]] Context& ctx) {
    return fmt::format("SWAP {}", "(HL)");
}
std::string opcode_str_0xcb37([[maybe_unused]] Context& ctx) { return fmt::format("SWAP {}", 'A'); }
std::string opcode_str_0xcb38([[maybe_unused]] Context& ctx) { return fmt::format("SRL {}", 'B'); }
std::string opcode_str_0xcb39([[maybe_unused]] Context& ctx) { return fmt::format("SRL {}", 'C'); }
std::string opcode_str_0xcb3a([[maybe_unused]] Context& ctx) { return fmt::format("SRL {}", 'D'); }
std::string opcode_str_0xcb3b([[maybe_unused]] Context& ctx) { return fmt::format("SRL {}", 'E'); }
std::string opcode_str_0xcb3c([[maybe_unused]] Context& ctx) { return fmt::format("SRL {}", 'H'); }
std::string opcode_str_0xcb3d([[maybe_unused]] Context& ctx) { return fmt::format("SRL {}", 'L'); }
std::string opcode_str_0xcb3e([[maybe_unused]] Context& ctx) {
    return fmt::format("SRL {}", "(HL)");
}
std::string opcode_str_0xcb3f([[maybe_unused]] Context& ctx) { return fmt::format("SRL {}", 'A'); }
std::string opcode_str_0xcb40([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 0, 'B');
}
std::string opcode_str_0xcb41([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 0, 'C');
}
std::string opcode_str_0xcb42([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 0, 'D');
}
std::string opcode_str_0xcb43([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 0, 'E');
}
std::string opcode_str_0xcb44([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 0, 'H');
}
std::string opcode_str_0xcb45([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 0, 'L');
}
std::string opcode_str_0xcb46([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 0, "(HL)");
}
std::string opcode_str_0xcb47([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 0, 'A');
}
std::string opcode_str_0xcb48([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 1, 'B');
}
std::string opcode_str_0xcb49([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 1, 'C');
}
std::string opcode_str_0xcb4a([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 1, 'D');
}
std::string opcode_str_0xcb4b([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 1, 'E');
}
std::string opcode_str_0xcb4c([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 1, 'H');
}
std::string opcode_str_0xcb4d([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 1, 'L');
}
std::string opcode_str_0xcb4e([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 1, "(HL)");
}
std::string opcode_str_0xcb4f([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 1, 'A');
}
std::string opcode_str_0xcb50([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 2, 'B');
}
std::string opcode_str_0xcb51([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 2, 'C');
}
std::string opcode_str_0xcb52([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 2, 'D');
}
std::string opcode_str_0xcb53([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 2, 'E');
}
std::string opcode_str_0xcb54([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 2, 'H');
}
std::string opcode_str_0xcb55([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 2, 'L');
}
std::string opcode_str_0xcb56([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 2, "(HL)");
}
std::string opcode_str_0xcb57([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 2, 'A');
}
std::string opcode_str_0xcb58([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 3, 'B');
}
std::string opcode_str_0xcb59([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 3, 'C');
}
std::string opcode_str_0xcb5a([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 3, 'D');
}
std::string opcode_str_0xcb5b([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 3, 'E');
}
std::string opcode_str_0xcb5c([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 3, 'H');
}
std::string opcode_str_0xcb5d([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 3, 'L');
}
std::string opcode_str_0xcb5e([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 3, "(HL)");
}
std::string opcode_str_0xcb5f([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 3, 'A');
}
std::string opcode_str_0xcb60([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 4, 'B');
}
std::string opcode_str_0xcb61([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 4, 'C');
}
std::string opcode_str_0xcb62([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 4, 'D');
}
std::string opcode_str_0xcb63([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 4, 'E');
}
std::string opcode_str_0xcb64([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 4, 'H');
}
std::string opcode_str_0xcb65([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 4, 'L');
}
std::string opcode_str_0xcb66([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 4, "(HL)");
}
std::string opcode_str_0xcb67([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 4, 'A');
}
std::string opcode_str_0xcb68([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 5, 'B');
}
std::string opcode_str_0xcb69([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 5, 'C');
}
std::string opcode_str_0xcb6a([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 5, 'D');
}
std::string opcode_str_0xcb6b([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 5, 'E');
}
std::string opcode_str_0xcb6c([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 5, 'H');
}
std::string opcode_str_0xcb6d([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 5, 'L');
}
std::string opcode_str_0xcb6e([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 5, "(HL)");
}
std::string opcode_str_0xcb6f([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 5, 'A');
}
std::string opcode_str_0xcb70([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 6, 'B');
}
std::string opcode_str_0xcb71([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 6, 'C');
}
std::string opcode_str_0xcb72([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 6, 'D');
}
std::string opcode_str_0xcb73([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 6, 'E');
}
std::string opcode_str_0xcb74([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 6, 'H');
}
std::string opcode_str_0xcb75([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 6, 'L');
}
std::string opcode_str_0xcb76([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 6, "(HL)");
}
std::string opcode_str_0xcb77([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 6, 'A');
}
std::string opcode_str_0xcb78([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 7, 'B');
}
std::string opcode_str_0xcb79([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 7, 'C');
}
std::string opcode_str_0xcb7a([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 7, 'D');
}
std::string opcode_str_0xcb7b([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 7, 'E');
}
std::string opcode_str_0xcb7c([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 7, 'H');
}
std::string opcode_str_0xcb7d([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 7, 'L');
}
std::string opcode_str_0xcb7e([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 7, "(HL)");
}
std::string opcode_str_0xcb7f([[maybe_unused]] Context& ctx) {
    return fmt::format("BIT {},{}", 7, 'A');
}
std::string opcode_str_0xcb80([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 0, 'B');
}
std::string opcode_str_0xcb81([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 0, 'C');
}
std::string opcode_str_0xcb82([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 0, 'D');
}
std::string opcode_str_0xcb83([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 0, 'E');
}
std::string opcode_str_0xcb84([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 0, 'H');
}
std::string opcode_str_0xcb85([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 0, 'L');
}
std::string opcode_str_0xcb86([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 0, "(HL)");
}
std::string opcode_str_0xcb87([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 0, 'A');
}
std::string opcode_str_0xcb88([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 1, 'B');
}
std::string opcode_str_0xcb89([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 1, 'C');
}
std::string opcode_str_0xcb8a([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 1, 'D');
}
std::string opcode_str_0xcb8b([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 1, 'E');
}
std::string opcode_str_0xcb8c([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 1, 'H');
}
std::string opcode_str_0xcb8d([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 1, 'L');
}
std::string opcode_str_0xcb8e([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 1, "(HL)");
}
std::string opcode_str_0xcb8f([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 1, 'A');
}
std::string opcode_str_0xcb90([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 2, 'B');
}
std::string opcode_str_0xcb91([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 2, 'C');
}
std::string opcode_str_0xcb92([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 2, 'D');
}
std::string opcode_str_0xcb93([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 2, 'E');
}
std::string opcode_str_0xcb94([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 2, 'H');
}
std::string opcode_str_0xcb95([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 2, 'L');
}
std::string opcode_str_0xcb96([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 2, "(HL)");
}
std::string opcode_str_0xcb97([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 2, 'A');
}
std::string opcode_str_0xcb98([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 3, 'B');
}
std::string opcode_str_0xcb99([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 3, 'C');
}
std::string opcode_str_0xcb9a([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 3, 'D');
}
std::string opcode_str_0xcb9b([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 3, 'E');
}
std::string opcode_str_0xcb9c([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 3, 'H');
}
std::string opcode_str_0xcb9d([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 3, 'L');
}
std::string opcode_str_0xcb9e([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 3, "(HL)");
}
std::string opcode_str_0xcb9f([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 3, 'A');
}
std::string opcode_str_0xcba0([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 4, 'B');
}
std::string opcode_str_0xcba1([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 4, 'C');
}
std::string opcode_str_0xcba2([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 4, 'D');
}
std::string opcode_str_0xcba3([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 4, 'E');
}
std::string opcode_str_0xcba4([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 4, 'H');
}
std::string opcode_str_0xcba5([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 4, 'L');
}
std::string opcode_str_0xcba6([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 4, "(HL)");
}
std::string opcode_str_0xcba7([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 4, 'A');
}
std::string opcode_str_0xcba8([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 5, 'B');
}
std::string opcode_str_0xcba9([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 5, 'C');
}
std::string opcode_str_0xcbaa([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 5, 'D');
}
std::string opcode_str_0xcbab([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 5, 'E');
}
std::string opcode_str_0xcbac([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 5, 'H');
}
std::string opcode_str_0xcbad([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 5, 'L');
}
std::string opcode_str_0xcbae([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 5, "(HL)");
}
std::string opcode_str_0xcbaf([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 5, 'A');
}
std::string opcode_str_0xcbb0([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 6, 'B');
}
std::string opcode_str_0xcbb1([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 6, 'C');
}
std::string opcode_str_0xcbb2([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 6, 'D');
}
std::string opcode_str_0xcbb3([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 6, 'E');
}
std::string opcode_str_0xcbb4([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 6, 'H');
}
std::string opcode_str_0xcbb5([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 6, 'L');
}
std::string opcode_str_0xcbb6([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 6, "(HL)");
}
std::string opcode_str_0xcbb7([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 6, 'A');
}
std::string opcode_str_0xcbb8([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 7, 'B');
}
std::string opcode_str_0xcbb9([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 7, 'C');
}
std::string opcode_str_0xcbba([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 7, 'D');
}
std::string opcode_str_0xcbbb([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 7, 'E');
}
std::string opcode_str_0xcbbc([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 7, 'H');
}
std::string opcode_str_0xcbbd([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 7, 'L');
}
std::string opcode_str_0xcbbe([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 7, "(HL)");
}
std::string opcode_str_0xcbbf([[maybe_unused]] Context& ctx) {
    return fmt::format("RES {},{}", 7, 'A');
}
std::string opcode_str_0xcbc0([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 0, 'B');
}
std::string opcode_str_0xcbc1([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 0, 'C');
}
std::string opcode_str_0xcbc2([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 0, 'D');
}
std::string opcode_str_0xcbc3([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 0, 'E');
}
std::string opcode_str_0xcbc4([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 0, 'H');
}
std::string opcode_str_0xcbc5([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 0, 'L');
}
std::string opcode_str_0xcbc6([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 0, "(HL)");
}
std::string opcode_str_0xcbc7([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 0, 'A');
}
std::string opcode_str_0xcbc8([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 1, 'B');
}
std::string opcode_str_0xcbc9([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 1, 'C');
}
std::string opcode_str_0xcbca([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 1, 'D');
}
std::string opcode_str_0xcbcb([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 1, 'E');
}
std::string opcode_str_0xcbcc([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 1, 'H');
}
std::string opcode_str_0xcbcd([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 1, 'L');
}
std::string opcode_str_0xcbce([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 1, "(HL)");
}
std::string opcode_str_0xcbcf([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 1, 'A');
}
std::string opcode_str_0xcbd0([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 2, 'B');
}
std::string opcode_str_0xcbd1([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 2, 'C');
}
std::string opcode_str_0xcbd2([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 2, 'D');
}
std::string opcode_str_0xcbd3([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 2, 'E');
}
std::string opcode_str_0xcbd4([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 2, 'H');
}
std::string opcode_str_0xcbd5([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 2, 'L');
}
std::string opcode_str_0xcbd6([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 2, "(HL)");
}
std::string opcode_str_0xcbd7([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 2, 'A');
}
std::string opcode_str_0xcbd8([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 3, 'B');
}
std::string opcode_str_0xcbd9([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 3, 'C');
}
std::string opcode_str_0xcbda([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 3, 'D');
}
std::string opcode_str_0xcbdb([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 3, 'E');
}
std::string opcode_str_0xcbdc([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 3, 'H');
}
std::string opcode_str_0xcbdd([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 3, 'L');
}
std::string opcode_str_0xcbde([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 3, "(HL)");
}
std::string opcode_str_0xcbdf([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 3, 'A');
}
std::string opcode_str_0xcbe0([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 4, 'B');
}
std::string opcode_str_0xcbe1([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 4, 'C');
}
std::string opcode_str_0xcbe2([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 4, 'D');
}
std::string opcode_str_0xcbe3([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 4, 'E');
}
std::string opcode_str_0xcbe4([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 4, 'H');
}
std::string opcode_str_0xcbe5([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 4, 'L');
}
std::string opcode_str_0xcbe6([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 4, "(HL)");
}
std::string opcode_str_0xcbe7([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 4, 'A');
}
std::string opcode_str_0xcbe8([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 5, 'B');
}
std::string opcode_str_0xcbe9([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 5, 'C');
}
std::string opcode_str_0xcbea([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 5, 'D');
}
std::string opcode_str_0xcbeb([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 5, 'E');
}
std::string opcode_str_0xcbec([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 5, 'H');
}
std::string opcode_str_0xcbed([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 5, 'L');
}
std::string opcode_str_0xcbee([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 5, "(HL)");
}
std::string opcode_str_0xcbef([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 5, 'A');
}
std::string opcode_str_0xcbf0([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 6, 'B');
}
std::string opcode_str_0xcbf1([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 6, 'C');
}
std::string opcode_str_0xcbf2([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 6, 'D');
}
std::string opcode_str_0xcbf3([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 6, 'E');
}
std::string opcode_str_0xcbf4([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 6, 'H');
}
std::string opcode_str_0xcbf5([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 6, 'L');
}
std::string opcode_str_0xcbf6([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 6, "(HL)");
}
std::string opcode_str_0xcbf7([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 6, 'A');
}
std::string opcode_str_0xcbf8([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 7, 'B');
}
std::string opcode_str_0xcbf9([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 7, 'C');
}
std::string opcode_str_0xcbfa([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 7, 'D');
}
std::string opcode_str_0xcbfb([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 7, 'E');
}
std::string opcode_str_0xcbfc([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 7, 'H');
}
std::string opcode_str_0xcbfd([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 7, 'L');
}
std::string opcode_str_0xcbfe([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 7, "(HL)");
}
std::string opcode_str_0xcbff([[maybe_unused]] Context& ctx) {
    return fmt::format("SET {},{}", 7, 'A');
}

std::array<std::string (*)(Context&), 256> opcode_strings() {
    std::array<std::string (*)(Context&), 256> ret;
    ret[0x00] = opcode_str_0x00;
    ret[0x01] = opcode_str_0x01;
    ret[0x02] = opcode_str_0x02;
    ret[0x03] = opcode_str_0x03;
    ret[0x04] = opcode_str_0x04;
    ret[0x05] = opcode_str_0x05;
    ret[0x06] = opcode_str_0x06;
    ret[0x07] = opcode_str_0x07;
    ret[0x08] = opcode_str_0x08;
    ret[0x09] = opcode_str_0x09;
    ret[0x0a] = opcode_str_0x0a;
    ret[0x0b] = opcode_str_0x0b;
    ret[0x0c] = opcode_str_0x0c;
    ret[0x0d] = opcode_str_0x0d;
    ret[0x0e] = opcode_str_0x0e;
    ret[0x0f] = opcode_str_0x0f;
    ret[0x10] = opcode_str_0x10;
    ret[0x11] = opcode_str_0x11;
    ret[0x12] = opcode_str_0x12;
    ret[0x13] = opcode_str_0x13;
    ret[0x14] = opcode_str_0x14;
    ret[0x15] = opcode_str_0x15;
    ret[0x16] = opcode_str_0x16;
    ret[0x17] = opcode_str_0x17;
    ret[0x18] = opcode_str_0x18;
    ret[0x19] = opcode_str_0x19;
    ret[0x1a] = opcode_str_0x1a;
    ret[0x1b] = opcode_str_0x1b;
    ret[0x1c] = opcode_str_0x1c;
    ret[0x1d] = opcode_str_0x1d;
    ret[0x1e] = opcode_str_0x1e;
    ret[0x1f] = opcode_str_0x1f;
    ret[0x20] = opcode_str_0x20;
    ret[0x21] = opcode_str_0x21;
    ret[0x22] = opcode_str_0x22;
    ret[0x23] = opcode_str_0x23;
    ret[0x24] = opcode_str_0x24;
    ret[0x25] = opcode_str_0x25;
    ret[0x26] = opcode_str_0x26;
    ret[0x27] = opcode_str_0x27;
    ret[0x28] = opcode_str_0x28;
    ret[0x29] = opcode_str_0x29;
    ret[0x2a] = opcode_str_0x2a;
    ret[0x2b] = opcode_str_0x2b;
    ret[0x2c] = opcode_str_0x2c;
    ret[0x2d] = opcode_str_0x2d;
    ret[0x2e] = opcode_str_0x2e;
    ret[0x2f] = opcode_str_0x2f;
    ret[0x30] = opcode_str_0x30;
    ret[0x31] = opcode_str_0x31;
    ret[0x32] = opcode_str_0x32;
    ret[0x33] = opcode_str_0x33;
    ret[0x34] = opcode_str_0x34;
    ret[0x35] = opcode_str_0x35;
    ret[0x36] = opcode_str_0x36;
    ret[0x37] = opcode_str_0x37;
    ret[0x38] = opcode_str_0x38;
    ret[0x39] = opcode_str_0x39;
    ret[0x3a] = opcode_str_0x3a;
    ret[0x3b] = opcode_str_0x3b;
    ret[0x3c] = opcode_str_0x3c;
    ret[0x3d] = opcode_str_0x3d;
    ret[0x3e] = opcode_str_0x3e;
    ret[0x3f] = opcode_str_0x3f;
    ret[0x40] = opcode_str_0x40;
    ret[0x41] = opcode_str_0x41;
    ret[0x42] = opcode_str_0x42;
    ret[0x43] = opcode_str_0x43;
    ret[0x44] = opcode_str_0x44;
    ret[0x45] = opcode_str_0x45;
    ret[0x46] = opcode_str_0x46;
    ret[0x47] = opcode_str_0x47;
    ret[0x48] = opcode_str_0x48;
    ret[0x49] = opcode_str_0x49;
    ret[0x4a] = opcode_str_0x4a;
    ret[0x4b] = opcode_str_0x4b;
    ret[0x4c] = opcode_str_0x4c;
    ret[0x4d] = opcode_str_0x4d;
    ret[0x4e] = opcode_str_0x4e;
    ret[0x4f] = opcode_str_0x4f;
    ret[0x50] = opcode_str_0x50;
    ret[0x51] = opcode_str_0x51;
    ret[0x52] = opcode_str_0x52;
    ret[0x53] = opcode_str_0x53;
    ret[0x54] = opcode_str_0x54;
    ret[0x55] = opcode_str_0x55;
    ret[0x56] = opcode_str_0x56;
    ret[0x57] = opcode_str_0x57;
    ret[0x58] = opcode_str_0x58;
    ret[0x59] = opcode_str_0x59;
    ret[0x5a] = opcode_str_0x5a;
    ret[0x5b] = opcode_str_0x5b;
    ret[0x5c] = opcode_str_0x5c;
    ret[0x5d] = opcode_str_0x5d;
    ret[0x5e] = opcode_str_0x5e;
    ret[0x5f] = opcode_str_0x5f;
    ret[0x60] = opcode_str_0x60;
    ret[0x61] = opcode_str_0x61;
    ret[0x62] = opcode_str_0x62;
    ret[0x63] = opcode_str_0x63;
    ret[0x64] = opcode_str_0x64;
    ret[0x65] = opcode_str_0x65;
    ret[0x66] = opcode_str_0x66;
    ret[0x67] = opcode_str_0x67;
    ret[0x68] = opcode_str_0x68;
    ret[0x69] = opcode_str_0x69;
    ret[0x6a] = opcode_str_0x6a;
    ret[0x6b] = opcode_str_0x6b;
    ret[0x6c] = opcode_str_0x6c;
    ret[0x6d] = opcode_str_0x6d;
    ret[0x6e] = opcode_str_0x6e;
    ret[0x6f] = opcode_str_0x6f;
    ret[0x70] = opcode_str_0x70;
    ret[0x71] = opcode_str_0x71;
    ret[0x72] = opcode_str_0x72;
    ret[0x73] = opcode_str_0x73;
    ret[0x74] = opcode_str_0x74;
    ret[0x75] = opcode_str_0x75;
    ret[0x76] = opcode_str_0x76;
    ret[0x77] = opcode_str_0x77;
    ret[0x78] = opcode_str_0x78;
    ret[0x79] = opcode_str_0x79;
    ret[0x7a] = opcode_str_0x7a;
    ret[0x7b] = opcode_str_0x7b;
    ret[0x7c] = opcode_str_0x7c;
    ret[0x7d] = opcode_str_0x7d;
    ret[0x7e] = opcode_str_0x7e;
    ret[0x7f] = opcode_str_0x7f;
    ret[0x80] = opcode_str_0x80;
    ret[0x81] = opcode_str_0x81;
    ret[0x82] = opcode_str_0x82;
    ret[0x83] = opcode_str_0x83;
    ret[0x84] = opcode_str_0x84;
    ret[0x85] = opcode_str_0x85;
    ret[0x86] = opcode_str_0x86;
    ret[0x87] = opcode_str_0x87;
    ret[0x88] = opcode_str_0x88;
    ret[0x89] = opcode_str_0x89;
    ret[0x8a] = opcode_str_0x8a;
    ret[0x8b] = opcode_str_0x8b;
    ret[0x8c] = opcode_str_0x8c;
    ret[0x8d] = opcode_str_0x8d;
    ret[0x8e] = opcode_str_0x8e;
    ret[0x8f] = opcode_str_0x8f;
    ret[0x90] = opcode_str_0x90;
    ret[0x91] = opcode_str_0x91;
    ret[0x92] = opcode_str_0x92;
    ret[0x93] = opcode_str_0x93;
    ret[0x94] = opcode_str_0x94;
    ret[0x95] = opcode_str_0x95;
    ret[0x96] = opcode_str_0x96;
    ret[0x97] = opcode_str_0x97;
    ret[0x98] = opcode_str_0x98;
    ret[0x99] = opcode_str_0x99;
    ret[0x9a] = opcode_str_0x9a;
    ret[0x9b] = opcode_str_0x9b;
    ret[0x9c] = opcode_str_0x9c;
    ret[0x9d] = opcode_str_0x9d;
    ret[0x9e] = opcode_str_0x9e;
    ret[0x9f] = opcode_str_0x9f;
    ret[0xa0] = opcode_str_0xa0;
    ret[0xa1] = opcode_str_0xa1;
    ret[0xa2] = opcode_str_0xa2;
    ret[0xa3] = opcode_str_0xa3;
    ret[0xa4] = opcode_str_0xa4;
    ret[0xa5] = opcode_str_0xa5;
    ret[0xa6] = opcode_str_0xa6;
    ret[0xa7] = opcode_str_0xa7;
    ret[0xa8] = opcode_str_0xa8;
    ret[0xa9] = opcode_str_0xa9;
    ret[0xaa] = opcode_str_0xaa;
    ret[0xab] = opcode_str_0xab;
    ret[0xac] = opcode_str_0xac;
    ret[0xad] = opcode_str_0xad;
    ret[0xae] = opcode_str_0xae;
    ret[0xaf] = opcode_str_0xaf;
    ret[0xb0] = opcode_str_0xb0;
    ret[0xb1] = opcode_str_0xb1;
    ret[0xb2] = opcode_str_0xb2;
    ret[0xb3] = opcode_str_0xb3;
    ret[0xb4] = opcode_str_0xb4;
    ret[0xb5] = opcode_str_0xb5;
    ret[0xb6] = opcode_str_0xb6;
    ret[0xb7] = opcode_str_0xb7;
    ret[0xb8] = opcode_str_0xb8;
    ret[0xb9] = opcode_str_0xb9;
    ret[0xba] = opcode_str_0xba;
    ret[0xbb] = opcode_str_0xbb;
    ret[0xbc] = opcode_str_0xbc;
    ret[0xbd] = opcode_str_0xbd;
    ret[0xbe] = opcode_str_0xbe;
    ret[0xbf] = opcode_str_0xbf;
    ret[0xc0] = opcode_str_0xc0;
    ret[0xc1] = opcode_str_0xc1;
    ret[0xc2] = opcode_str_0xc2;
    ret[0xc3] = opcode_str_0xc3;
    ret[0xc4] = opcode_str_0xc4;
    ret[0xc5] = opcode_str_0xc5;
    ret[0xc6] = opcode_str_0xc6;
    ret[0xc7] = opcode_str_0xc7;
    ret[0xc8] = opcode_str_0xc8;
    ret[0xc9] = opcode_str_0xc9;
    ret[0xca] = opcode_str_0xca;
    ret[0xcc] = opcode_str_0xcc;
    ret[0xcd] = opcode_str_0xcd;
    ret[0xce] = opcode_str_0xce;
    ret[0xcf] = opcode_str_0xcf;
    ret[0xd0] = opcode_str_0xd0;
    ret[0xd1] = opcode_str_0xd1;
    ret[0xd2] = opcode_str_0xd2;
    ret[0xd3] = opcode_str_0xd3;
    ret[0xd4] = opcode_str_0xd4;
    ret[0xd5] = opcode_str_0xd5;
    ret[0xd6] = opcode_str_0xd6;
    ret[0xd7] = opcode_str_0xd7;
    ret[0xd8] = opcode_str_0xd8;
    ret[0xd9] = opcode_str_0xd9;
    ret[0xda] = opcode_str_0xda;
    ret[0xdb] = opcode_str_0xdb;
    ret[0xdc] = opcode_str_0xdc;
    ret[0xdd] = opcode_str_0xdd;
    ret[0xde] = opcode_str_0xde;
    ret[0xdf] = opcode_str_0xdf;
    ret[0xe0] = opcode_str_0xe0;
    ret[0xe1] = opcode_str_0xe1;
    ret[0xe2] = opcode_str_0xe2;
    ret[0xe3] = opcode_str_0xe3;
    ret[0xe4] = opcode_str_0xe4;
    ret[0xe5] = opcode_str_0xe5;
    ret[0xe6] = opcode_str_0xe6;
    ret[0xe7] = opcode_str_0xe7;
    ret[0xe8] = opcode_str_0xe8;
    ret[0xe9] = opcode_str_0xe9;
    ret[0xea] = opcode_str_0xea;
    ret[0xeb] = opcode_str_0xeb;
    ret[0xec] = opcode_str_0xec;
    ret[0xed] = opcode_str_0xed;
    ret[0xee] = opcode_str_0xee;
    ret[0xef] = opcode_str_0xef;
    ret[0xf0] = opcode_str_0xf0;
    ret[0xf1] = opcode_str_0xf1;
    ret[0xf2] = opcode_str_0xf2;
    ret[0xf3] = opcode_str_0xf3;
    ret[0xf4] = opcode_str_0xf4;
    ret[0xf5] = opcode_str_0xf5;
    ret[0xf6] = opcode_str_0xf6;
    ret[0xf7] = opcode_str_0xf7;
    ret[0xf8] = opcode_str_0xf8;
    ret[0xf9] = opcode_str_0xf9;
    ret[0xfa] = opcode_str_0xfa;
    ret[0xfb] = opcode_str_0xfb;
    ret[0xfc] = opcode_str_0xfc;
    ret[0xfd] = opcode_str_0xfd;
    ret[0xfe] = opcode_str_0xfe;
    ret[0xff] = opcode_str_0xff;
    return ret;
}
std::array<std::string (*)(Context&), 256> cb_opcode_strings() {
    std::array<std::string (*)(Context&), 256> ret;
    ret[0]   = opcode_str_0xcb00;
    ret[1]   = opcode_str_0xcb01;
    ret[2]   = opcode_str_0xcb02;
    ret[3]   = opcode_str_0xcb03;
    ret[4]   = opcode_str_0xcb04;
    ret[5]   = opcode_str_0xcb05;
    ret[6]   = opcode_str_0xcb06;
    ret[7]   = opcode_str_0xcb07;
    ret[8]   = opcode_str_0xcb08;
    ret[9]   = opcode_str_0xcb09;
    ret[10]  = opcode_str_0xcb0a;
    ret[11]  = opcode_str_0xcb0b;
    ret[12]  = opcode_str_0xcb0c;
    ret[13]  = opcode_str_0xcb0d;
    ret[14]  = opcode_str_0xcb0e;
    ret[15]  = opcode_str_0xcb0f;
    ret[16]  = opcode_str_0xcb10;
    ret[17]  = opcode_str_0xcb11;
    ret[18]  = opcode_str_0xcb12;
    ret[19]  = opcode_str_0xcb13;
    ret[20]  = opcode_str_0xcb14;
    ret[21]  = opcode_str_0xcb15;
    ret[22]  = opcode_str_0xcb16;
    ret[23]  = opcode_str_0xcb17;
    ret[24]  = opcode_str_0xcb18;
    ret[25]  = opcode_str_0xcb19;
    ret[26]  = opcode_str_0xcb1a;
    ret[27]  = opcode_str_0xcb1b;
    ret[28]  = opcode_str_0xcb1c;
    ret[29]  = opcode_str_0xcb1d;
    ret[30]  = opcode_str_0xcb1e;
    ret[31]  = opcode_str_0xcb1f;
    ret[32]  = opcode_str_0xcb20;
    ret[33]  = opcode_str_0xcb21;
    ret[34]  = opcode_str_0xcb22;
    ret[35]  = opcode_str_0xcb23;
    ret[36]  = opcode_str_0xcb24;
    ret[37]  = opcode_str_0xcb25;
    ret[38]  = opcode_str_0xcb26;
    ret[39]  = opcode_str_0xcb27;
    ret[40]  = opcode_str_0xcb28;
    ret[41]  = opcode_str_0xcb29;
    ret[42]  = opcode_str_0xcb2a;
    ret[43]  = opcode_str_0xcb2b;
    ret[44]  = opcode_str_0xcb2c;
    ret[45]  = opcode_str_0xcb2d;
    ret[46]  = opcode_str_0xcb2e;
    ret[47]  = opcode_str_0xcb2f;
    ret[48]  = opcode_str_0xcb30;
    ret[49]  = opcode_str_0xcb31;
    ret[50]  = opcode_str_0xcb32;
    ret[51]  = opcode_str_0xcb33;
    ret[52]  = opcode_str_0xcb34;
    ret[53]  = opcode_str_0xcb35;
    ret[54]  = opcode_str_0xcb36;
    ret[55]  = opcode_str_0xcb37;
    ret[56]  = opcode_str_0xcb38;
    ret[57]  = opcode_str_0xcb39;
    ret[58]  = opcode_str_0xcb3a;
    ret[59]  = opcode_str_0xcb3b;
    ret[60]  = opcode_str_0xcb3c;
    ret[61]  = opcode_str_0xcb3d;
    ret[62]  = opcode_str_0xcb3e;
    ret[63]  = opcode_str_0xcb3f;
    ret[64]  = opcode_str_0xcb40;
    ret[65]  = opcode_str_0xcb41;
    ret[66]  = opcode_str_0xcb42;
    ret[67]  = opcode_str_0xcb43;
    ret[68]  = opcode_str_0xcb44;
    ret[69]  = opcode_str_0xcb45;
    ret[70]  = opcode_str_0xcb46;
    ret[71]  = opcode_str_0xcb47;
    ret[72]  = opcode_str_0xcb48;
    ret[73]  = opcode_str_0xcb49;
    ret[74]  = opcode_str_0xcb4a;
    ret[75]  = opcode_str_0xcb4b;
    ret[76]  = opcode_str_0xcb4c;
    ret[77]  = opcode_str_0xcb4d;
    ret[78]  = opcode_str_0xcb4e;
    ret[79]  = opcode_str_0xcb4f;
    ret[80]  = opcode_str_0xcb50;
    ret[81]  = opcode_str_0xcb51;
    ret[82]  = opcode_str_0xcb52;
    ret[83]  = opcode_str_0xcb53;
    ret[84]  = opcode_str_0xcb54;
    ret[85]  = opcode_str_0xcb55;
    ret[86]  = opcode_str_0xcb56;
    ret[87]  = opcode_str_0xcb57;
    ret[88]  = opcode_str_0xcb58;
    ret[89]  = opcode_str_0xcb59;
    ret[90]  = opcode_str_0xcb5a;
    ret[91]  = opcode_str_0xcb5b;
    ret[92]  = opcode_str_0xcb5c;
    ret[93]  = opcode_str_0xcb5d;
    ret[94]  = opcode_str_0xcb5e;
    ret[95]  = opcode_str_0xcb5f;
    ret[96]  = opcode_str_0xcb60;
    ret[97]  = opcode_str_0xcb61;
    ret[98]  = opcode_str_0xcb62;
    ret[99]  = opcode_str_0xcb63;
    ret[100] = opcode_str_0xcb64;
    ret[101] = opcode_str_0xcb65;
    ret[102] = opcode_str_0xcb66;
    ret[103] = opcode_str_0xcb67;
    ret[104] = opcode_str_0xcb68;
    ret[105] = opcode_str_0xcb69;
    ret[106] = opcode_str_0xcb6a;
    ret[107] = opcode_str_0xcb6b;
    ret[108] = opcode_str_0xcb6c;
    ret[109] = opcode_str_0xcb6d;
    ret[110] = opcode_str_0xcb6e;
    ret[111] = opcode_str_0xcb6f;
    ret[112] = opcode_str_0xcb70;
    ret[113] = opcode_str_0xcb71;
    ret[114] = opcode_str_0xcb72;
    ret[115] = opcode_str_0xcb73;
    ret[116] = opcode_str_0xcb74;
    ret[117] = opcode_str_0xcb75;
    ret[118] = opcode_str_0xcb76;
    ret[119] = opcode_str_0xcb77;
    ret[120] = opcode_str_0xcb78;
    ret[121] = opcode_str_0xcb79;
    ret[122] = opcode_str_0xcb7a;
    ret[123] = opcode_str_0xcb7b;
    ret[124] = opcode_str_0xcb7c;
    ret[125] = opcode_str_0xcb7d;
    ret[126] = opcode_str_0xcb7e;
    ret[127] = opcode_str_0xcb7f;
    ret[128] = opcode_str_0xcb80;
    ret[129] = opcode_str_0xcb81;
    ret[130] = opcode_str_0xcb82;
    ret[131] = opcode_str_0xcb83;
    ret[132] = opcode_str_0xcb84;
    ret[133] = opcode_str_0xcb85;
    ret[134] = opcode_str_0xcb86;
    ret[135] = opcode_str_0xcb87;
    ret[136] = opcode_str_0xcb88;
    ret[137] = opcode_str_0xcb89;
    ret[138] = opcode_str_0xcb8a;
    ret[139] = opcode_str_0xcb8b;
    ret[140] = opcode_str_0xcb8c;
    ret[141] = opcode_str_0xcb8d;
    ret[142] = opcode_str_0xcb8e;
    ret[143] = opcode_str_0xcb8f;
    ret[144] = opcode_str_0xcb90;
    ret[145] = opcode_str_0xcb91;
    ret[146] = opcode_str_0xcb92;
    ret[147] = opcode_str_0xcb93;
    ret[148] = opcode_str_0xcb94;
    ret[149] = opcode_str_0xcb95;
    ret[150] = opcode_str_0xcb96;
    ret[151] = opcode_str_0xcb97;
    ret[152] = opcode_str_0xcb98;
    ret[153] = opcode_str_0xcb99;
    ret[154] = opcode_str_0xcb9a;
    ret[155] = opcode_str_0xcb9b;
    ret[156] = opcode_str_0xcb9c;
    ret[157] = opcode_str_0xcb9d;
    ret[158] = opcode_str_0xcb9e;
    ret[159] = opcode_str_0xcb9f;
    ret[160] = opcode_str_0xcba0;
    ret[161] = opcode_str_0xcba1;
    ret[162] = opcode_str_0xcba2;
    ret[163] = opcode_str_0xcba3;
    ret[164] = opcode_str_0xcba4;
    ret[165] = opcode_str_0xcba5;
    ret[166] = opcode_str_0xcba6;
    ret[167] = opcode_str_0xcba7;
    ret[168] = opcode_str_0xcba8;
    ret[169] = opcode_str_0xcba9;
    ret[170] = opcode_str_0xcbaa;
    ret[171] = opcode_str_0xcbab;
    ret[172] = opcode_str_0xcbac;
    ret[173] = opcode_str_0xcbad;
    ret[174] = opcode_str_0xcbae;
    ret[175] = opcode_str_0xcbaf;
    ret[176] = opcode_str_0xcbb0;
    ret[177] = opcode_str_0xcbb1;
    ret[178] = opcode_str_0xcbb2;
    ret[179] = opcode_str_0xcbb3;
    ret[180] = opcode_str_0xcbb4;
    ret[181] = opcode_str_0xcbb5;
    ret[182] = opcode_str_0xcbb6;
    ret[183] = opcode_str_0xcbb7;
    ret[184] = opcode_str_0xcbb8;
    ret[185] = opcode_str_0xcbb9;
    ret[186] = opcode_str_0xcbba;
    ret[187] = opcode_str_0xcbbb;
    ret[188] = opcode_str_0xcbbc;
    ret[189] = opcode_str_0xcbbd;
    ret[190] = opcode_str_0xcbbe;
    ret[191] = opcode_str_0xcbbf;
    ret[192] = opcode_str_0xcbc0;
    ret[193] = opcode_str_0xcbc1;
    ret[194] = opcode_str_0xcbc2;
    ret[195] = opcode_str_0xcbc3;
    ret[196] = opcode_str_0xcbc4;
    ret[197] = opcode_str_0xcbc5;
    ret[198] = opcode_str_0xcbc6;
    ret[199] = opcode_str_0xcbc7;
    ret[200] = opcode_str_0xcbc8;
    ret[201] = opcode_str_0xcbc9;
    ret[202] = opcode_str_0xcbca;
    ret[203] = opcode_str_0xcbcb;
    ret[204] = opcode_str_0xcbcc;
    ret[205] = opcode_str_0xcbcd;
    ret[206] = opcode_str_0xcbce;
    ret[207] = opcode_str_0xcbcf;
    ret[208] = opcode_str_0xcbd0;
    ret[209] = opcode_str_0xcbd1;
    ret[210] = opcode_str_0xcbd2;
    ret[211] = opcode_str_0xcbd3;
    ret[212] = opcode_str_0xcbd4;
    ret[213] = opcode_str_0xcbd5;
    ret[214] = opcode_str_0xcbd6;
    ret[215] = opcode_str_0xcbd7;
    ret[216] = opcode_str_0xcbd8;
    ret[217] = opcode_str_0xcbd9;
    ret[218] = opcode_str_0xcbda;
    ret[219] = opcode_str_0xcbdb;
    ret[220] = opcode_str_0xcbdc;
    ret[221] = opcode_str_0xcbdd;
    ret[222] = opcode_str_0xcbde;
    ret[223] = opcode_str_0xcbdf;
    ret[224] = opcode_str_0xcbe0;
    ret[225] = opcode_str_0xcbe1;
    ret[226] = opcode_str_0xcbe2;
    ret[227] = opcode_str_0xcbe3;
    ret[228] = opcode_str_0xcbe4;
    ret[229] = opcode_str_0xcbe5;
    ret[230] = opcode_str_0xcbe6;
    ret[231] = opcode_str_0xcbe7;
    ret[232] = opcode_str_0xcbe8;
    ret[233] = opcode_str_0xcbe9;
    ret[234] = opcode_str_0xcbea;
    ret[235] = opcode_str_0xcbeb;
    ret[236] = opcode_str_0xcbec;
    ret[237] = opcode_str_0xcbed;
    ret[238] = opcode_str_0xcbee;
    ret[239] = opcode_str_0xcbef;
    ret[240] = opcode_str_0xcbf0;
    ret[241] = opcode_str_0xcbf1;
    ret[242] = opcode_str_0xcbf2;
    ret[243] = opcode_str_0xcbf3;
    ret[244] = opcode_str_0xcbf4;
    ret[245] = opcode_str_0xcbf5;
    ret[246] = opcode_str_0xcbf6;
    ret[247] = opcode_str_0xcbf7;
    ret[248] = opcode_str_0xcbf8;
    ret[249] = opcode_str_0xcbf9;
    ret[250] = opcode_str_0xcbfa;
    ret[251] = opcode_str_0xcbfb;
    ret[252] = opcode_str_0xcbfc;
    ret[253] = opcode_str_0xcbfd;
    ret[254] = opcode_str_0xcbfe;
    ret[255] = opcode_str_0xcbff;
    return ret;
}