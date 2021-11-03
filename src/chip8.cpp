#include <iostream>
#include <bitset>
#include <fstream>
#include <ctime>
#include <thread>
#include <future>

#include "chip8.hpp"

constexpr uint16_t entry = 0x200;

const uint8_t fontset[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80 // F
};

uint16_t swap_byte_order(uint16_t s) { return (s >> 8) | (s << 8); }

Chip8::Chip8(const std::string& file_name) : PC(entry) {
    std::srand(std::time(nullptr));

    read_file(file_name);

    // copy font data
    for (auto i = 0; i < 80; ++i) {
        memory[i] = fontset[i];
    }
}

void Chip8::read_file(const std::string& name) {
    std::ifstream file;
    file.open(name, std::ios_base::binary);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(memory.data() + entry), size);
    file.close();
}

void Chip8::fetch() {
    opcode = *reinterpret_cast<uint16_t*>(&memory[PC]);

    // CHIP8 is big endian, swap byte order
    opcode = swap_byte_order(opcode);

    // store each 4 bits of opcode into array
    for (auto i = 0; i < 4; ++i) {
        val[i] = (opcode >> (4 * (3 - i))) & 0xF;
    }
}

op Chip8::decode() {

    switch (val[0]) {
    case 0x0: {
        if (val[3] == 0xE)
            return op::RET;
        else if (val[2] == 0xE)
            return op::CLEAR;
        else
            return op::CALL_M;
    }
    case 0x1: {
        return op::JMP;
    }
    case 0x2: {
        return op::CALL;
    }
    case 0x3: {
        return op::CONDI;
    }
    case 0x4: {
        return op::NCONDI;
    }
    case 0x5: {
        return op::CONDR;
    }
    case 0x6: {
        return op::MOVI;
    }
    case 0x7: {
        return op::ADDI;
    }
    case 0x8: {
        switch (val[3]) {
        case 0x0: {
            return op::MOVR;
        }
        case 0x1: {
            return op::OR;
        }
        case 0x2: {
            return op::AND;
        }
        case 0x3: {
            return op::XOR;
        }
        case 0x4: {
            return op::ADDR;
        }
        case 0x5: {
            return op::SUBR;
        }
        case 0x6: {
            return op::RSHIFT;
        }
        case 0x7: {
            return op::NSUBR;
        }
        case 0xE: {
            return op::LSHIFT;
        }
        default: {
            return op::UNKNOWN;
        }
        }
        break;
    }
    case 0x9: {
        return op::NCONDR;
    }
    case 0xA: {
        return op::SETI;
    }
    case 0xB: {
        return op::JMP_OFFSET;
    }
    case 0xC: {
        return op::RAND;
    }
    case 0xD: {
        return op::DRAW;
    }
    case 0xE: {
        switch (val[3]) {
        case 0xE: {
            return op::CONDKEY;
        }
        case 0x1: {
            return op::NCONDKEY;
        }
        default: {
            return op::UNKNOWN;
        }
        }
        break;
    }
    case 0xF: {
        switch (val[3]) {
        case 0x7: {
            return op::DELAY;
        }
        case 0xA: {
            return op::GET_KEY;
        }
        case 0x8: {
            return op::SETS;
        }
        case 0xE: {
            return op::ADD_I;
        }
        case 0x9: {
            return op::SETIFONT;
        }
        case 0x3: {
            return op::BCD;
        }
        case 0x5: {
            switch (val[2]) {
            case 0x1: {
                return op::SETD;
            }
            case 0x5: {
                return op::DUMP;
            }
            case 0x6: {
                return op::LOAD;
            }
            default: {
                return op::UNKNOWN;
            }
            }
            break;
        }
        default: {
            return op::UNKNOWN;
        }
        }
        break;
    }
    }
    return op::UNKNOWN;
}

void Chip8::execute() {
    // immediates commonly used
    auto imm4  = opcode & 0xF;
    auto imm8  = opcode & 0xFF;
    auto imm12 = opcode & 0xFFF;

    // references to typical Vx, Vy parameters
    auto& Vx = V[val[1]];
    auto& Vy = V[val[2]];

    switch (operation) {
        // for call/jump instructions, we unconditionally add 2 to PC every cycle
        // so by adjusting by -2, we can skip a conditional check
    case op::CALL_M: {
        stack.emplace(PC);
        PC = (imm12)-2;
        break;
    }
    case op::CLEAR: {

        for (auto& line : framebuffer) {
            for (auto& pixel : line) {
                pixel = false;
            }
        }

        should_draw = true;
        break;
    }
    case op::RET: {
        // get last PC from stack
        PC = stack.top();
        /*
            currently PC points to last CALL_SUB instruction, but we unconditionally
            increment anyway
         */
        stack.pop();
        break;
    }
    case op::JMP: {
        PC = (imm12)-2;
        break;
    }
    case op::CALL: {
        // save PC
        stack.emplace(PC);
        PC = (imm12)-2;
        break;
    }
    case op::CONDI: {
        if (Vx == imm8) {
            PC += 2;
        }
        break;
    }
    case op::NCONDI: {
        if (Vx != imm8) {
            PC += 2;
        }
        break;
    }
    case op::CONDR: {
        if (Vx == Vy) {
            PC += 2;
        }
        break;
    }
    case op::MOVI: {
        Vx = imm8;
        break;
    }
    case op::ADDI: {
        Vx += imm8;
        break;
    }
    case op::MOVR: {
        Vx = Vy;
        break;
    }
    case op::OR: {
        Vx |= Vy;
        break;
    }
    case op::AND: {
        Vx &= Vy;
        break;
    }
    case op::XOR: {
        Vx ^= Vy;
        break;
    }
    case op::ADDR: {

        auto original = Vx;
        Vx += Vy;
        // no overflow case
        if (original < Vx) {
            V[0xF] = 0;
        }
        else {
            V[0xF] = 1;
        }
        break;
    }
    case op::SUBR: {
        if (Vx > Vy) {
            V[0xF] = 1;
        }
        else {
            V[0xF] = 0;
        }
        Vx -= Vy;
        break;
    }
    case op::RSHIFT: {
        V[0xF] = Vx & 0x1;
        Vx >>= 1;
        break;
    }
    case op::NSUBR: {
        if (Vy > Vx) {
            V[0xF] = 1;
        }
        else {
            V[0xF] = 0;
        }
        Vx = Vy - Vx;
        break;
    }
    case op::LSHIFT: {
        V[0xF] = Vx >> 7;
        Vx <<= 1;
        break;
    }
    case op::NCONDR: {
        if (Vx != Vy) {
            PC += 2;
        }
        break;
    }
    case op::SETI: {
        I = imm12;
        break;
    }
    case op::JMP_OFFSET: {
        PC = V[0x0] + imm12;
        break;
    }
    case op::RAND: {
        uint8_t r = std::rand() & 0xFF;
        Vx        = r & imm8;
        break;
    }
    case op::DRAW: {

        auto x = Vx;
        auto y = Vy;
        auto n = imm4;

        if (n == 0)
            n = 16;

        bool pixel_unset = false;

        // N lines down
        for (uint8_t i = 0; i < n; ++i) {
            std::bitset<8> line = memory[I + i];

            uint8_t currY = (y + i) % 32;

            // 8 pixels per line, to right
            for (uint8_t j = 0; j < 8; ++j) {

                uint8_t currX = (x + j) % 64;

                // 7 - j because 0 actually is least significant bit
                if (!pixel_unset && framebuffer[currY][currX] && line[7 - j]) {
                    pixel_unset = true;
                }
                framebuffer[currY][currX] ^= line[7 - j];
            }
        }

        if (pixel_unset)
            V[0xF] = 1;
        else
            V[0xF] = 0;

        should_draw = true;
        break;
    }
    case op::CONDKEY: {
        if (keys[Vx]) {
            PC += 2;
        }
        break;
    }
    case op::NCONDKEY: {
        if (!keys[Vx]) {
            PC += 2;
        }
        break;
    }
    case op::DELAY: {
        Vx = delay_timer;
        break;
    }
    case op::GET_KEY: {
        bool wait = true;
        while (wait) {

            for (auto i = 0; i < 16; ++i) {
                if (keys[i]) {
                    Vx   = static_cast<uint8_t>(i);
                    wait = false;
                    break;
                }
            }
        }
        break;
    }
    case op::SETD: {
        delay_timer = Vx;
        break;
    }
    case op::SETS: {
        sound_timer = Vx;
        break;
    }
    case op::ADD_I: {
        I += Vx;
        break;
    }
    case op::SETIFONT: {
        uint8_t c = Vx;
        I         = c * 5;
        break;
    }
    case op::BCD: {
        uint8_t first  = Vx / 100;
        uint8_t second = (Vx / 10) % 10;
        uint8_t third  = Vx % 10;

        memory[I]     = (first);
        memory[I + 1] = (second);
        memory[I + 2] = (third);
        break;
    }
    case op::DUMP: {
        auto ptr = I;
        for (auto i = 0; i <= val[1]; ++i) {
            memory[ptr++] = (V[i]);
        }
        break;
    }
    case op::LOAD: {
        auto ptr = I;
        for (auto i = 0; i <= val[1]; ++i) {
            V[i] = static_cast<uint8_t>(memory[ptr++]);
        }
        break;
    }
    case op::UNKNOWN: {
        std::cout << "Unknown opcode: " << std::hex << opcode << '\n';
        break;
    }
    }
}
void Chip8::update_timers() {
    if (delay_timer > 0) {
        delay_timer--;
    }
    if (sound_timer > 0) {
        sound_timer--;
    }
}

void Chip8::cycle() {

    // 600Hz
    while (!timer.update())
        ;

    if (cycle_count % 10 == 1) {
        update_timers();
    }

    // fetch
    fetch();
    // decode
    operation = decode();
    // execute
    execute();

    PC += 2;

    cycle_count++;
}
