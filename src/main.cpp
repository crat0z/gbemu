#include "chip8.hpp"
#include <bitset>
#include <iostream>

const char* file = "test_opcode.ch8";

int main(int argc, char** argv) {

    Chip8 emu(file);

    emu.run();
}