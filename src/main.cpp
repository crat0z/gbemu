#include "chip8.hpp"
#include <bitset>
#include <iostream>

const char* file = "test_opcode.ch8";

int main(int argc, char** argv) {

    CPU emu;
    emu.init(file);

    while (true) {
        emu.cycle();
    }
}