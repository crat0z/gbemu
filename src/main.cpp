#include "chip8.hpp"
#include <bitset>
#include <iostream>

const char* file = "c8_test.c8";

int main(int argc, char** argv) {

    Chip8 emu(argv[1]);

    emu.run();
}