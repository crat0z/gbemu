#include "chip8.hpp"
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <bitset>
#include <fstream>
#include <ctime>
#include <unordered_map>

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

std::unordered_map<int, uint8_t> keybinds = { { SDLK_1, 0x1 }, { SDLK_2, 0x2 }, { SDLK_3, 0x3 },
                                              { SDLK_4, 0xC }, { SDLK_q, 0x4 }, { SDLK_w, 0x5 },
                                              { SDLK_e, 0x6 }, { SDLK_r, 0xD }, { SDLK_a, 0x7 },
                                              { SDLK_s, 0x8 }, { SDLK_d, 0x9 }, { SDLK_f, 0xE },
                                              { SDLK_z, 0xA }, { SDLK_x, 0x0 }, { SDLK_c, 0xB },
                                              { SDLK_v, 0xF } };

// helpful functions
uint8_t  get8(uint16_t op) { return op & 0xFF; }
uint16_t get12(uint16_t op) { return op & 0xFFF; }
uint16_t swap_byte_order(uint16_t s) { return (s >> 8) | (s << 8); }

Chip8::Chip8(const std::string& file_name, size_t f, int x, int y)
        : memory({}),
          V({ 0 }),
          PC(0x200),
          val({ 0 }),
          keys({ 0 }),
          timer{ 60 },
          freq{ f },
          graphics(x, y) {
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
    file.read(reinterpret_cast<char*>(memory.data() + 0x200), size);
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
            return op::CALL;
    }
    case 0x1: {
        return op::GOTO;
    }
    case 0x2: {
        return op::CALL_SUB;
    }
    case 0x3: {
        return op::IF_IMM;
    }
    case 0x4: {
        return op::IF_NOT_IMM;
    }
    case 0x5: {
        return op::IF_REG;
    }
    case 0x6: {
        return op::SET_IMM;
    }
    case 0x7: {
        return op::ADD_IMM;
    }
    case 0x8: {
        switch (val[3]) {
        case 0x0: {
            return op::SET_REG;
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
            return op::ADD_REG;
        }
        case 0x5: {
            return op::SUB_REG;
        }
        case 0x6: {
            return op::RSHIFT;
        }
        case 0x7: {
            return op::SUB_REG2;
        }
        case 0xE: {
            return op::LSHIFT;
        }
        default: {
            return op::UNKNOWN;
        }
        }
    }
    case 0x9: {
        return op::IF_NOT_REG;
    }
    case 0xA: {
        return op::SET_I;
    }
    case 0xB: {
        return op::JMP_OFFSET;
    }
    case 0xC: {
        return op::RAND_BITSET;
    }
    case 0xD: {
        return op::DRAW;
    }
    case 0xE: {
        switch (val[3]) {
        case 0xE: {
            return op::SKIP_KEY;
        }
        case 0x1: {
            return op::SKIP_NOT_KEY;
        }
        }
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
            return op::SET_SOUND;
        }
        case 0xE: {
            return op::ADD_I;
        }
        case 0x9: {
            return op::SET_I_SPRITE;
        }
        case 0x3: {
            return op::STORE_BCD;
        }
        case 0x5: {
            switch (val[2]) {
            case 0x1: {
                return op::SET_DELAY;
            }
            case 0x5: {
                return op::REG_DUMP;
            }
            case 0x6: {
                return op::REG_LOAD;
            }
            default: {
                return op::UNKNOWN;
            }
            }
        }
        default: {
            return op::UNKNOWN;
        }
        }
    }
    }
    return op::UNKNOWN;
}

void Chip8::execute() {
    // immediates commonly used
    auto imm8  = get8(opcode);
    auto imm12 = get12(opcode);

    // references to typical Vx, Vy parameters
    auto& Vx = V[val[1]];
    auto& Vy = V[val[2]];

    switch (operation) {
        // for call/jump instructions, we unconditionally add 2 to PC every cycle
        // so by adjusting by -2, we can skip a conditional check
    case op::CALL: {
        stack.emplace(PC);
        PC = (imm12)-2;
        break;
    }
    case op::CLEAR: {

        graphics.clear();

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
    case op::GOTO: {
        PC = (imm12)-2;
        break;
    }
    case op::CALL_SUB: {
        // save PC
        stack.emplace(PC);
        PC = (imm12)-2;
        break;
    }
    case op::IF_IMM: {
        if (Vx == imm8) {
            PC += 2;
        }
        break;
    }
    case op::IF_NOT_IMM: {
        if (Vx != imm8) {
            PC += 2;
        }
        break;
    }
    case op::IF_REG: {
        if (Vx == Vy) {
            PC += 2;
        }
        break;
    }
    case op::SET_IMM: {
        Vx = imm8;
        break;
    }
    case op::ADD_IMM: {
        Vx += imm8;
        break;
    }
    case op::SET_REG: {
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
    case op::ADD_REG: {

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
    case op::SUB_REG: {
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
    case op::SUB_REG2: {
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
    case op::IF_NOT_REG: {
        if (Vx != Vy) {
            PC += 2;
        }
        break;
    }
    case op::SET_I: {
        I = imm12;
        break;
    }
    case op::JMP_OFFSET: {
        PC = V[0x0] + imm12;
        break;
    }
    case op::RAND_BITSET: {
        uint8_t r = std::rand() & 0xFF;
        Vx        = r & imm8;
        break;
    }
    case op::DRAW: {

        auto x = Vx;
        auto y = Vy;
        auto n = val[3];

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
                if (!pixel_unset && graphics(currY, currX) && line[7 - j]) {
                    pixel_unset = true;
                }
                graphics(currY, currX) ^= line[7 - j];
            }
        }

        if (pixel_unset)
            V[0xF] = 1;
        else
            V[0xF] = 0;

        should_draw = true;
        break;
    }
    case op::SKIP_KEY: {
        if (keys[Vx]) {
            PC += 2;
        }
        break;
    }
    case op::SKIP_NOT_KEY: {
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
            update_keys();
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
    case op::SET_DELAY: {
        delay_timer = Vx;
        break;
    }
    case op::SET_SOUND: {
        sound_timer = Vx;
        break;
    }
    case op::ADD_I: {
        I += Vx;
        break;
    }
    case op::SET_I_SPRITE: {
        uint8_t c = Vx;
        I         = c * 5;
        break;
    }
    case op::STORE_BCD: {
        uint8_t first  = Vx / 100;
        uint8_t second = (Vx / 10) % 10;
        uint8_t third  = Vx % 10;

        memory[I]     = (first);
        memory[I + 1] = (second);
        memory[I + 2] = (third);
        break;
    }
    case op::REG_DUMP: {
        auto ptr = I;
        for (auto i = 0; i <= val[1]; ++i) {
            memory[ptr++] = (V[i]);
        }
        break;
    }
    case op::REG_LOAD: {
        auto ptr = I;
        for (auto i = 0; i <= val[1]; ++i) {
            V[i] = static_cast<uint8_t>(memory[ptr++]);
        }
        break;
    }
    case op::UNKNOWN: {
        std::cout << "Unknown opcode: " << std::hex << opcode;
        break;
    }
    }
}

bool Chip8::update_keys() {
    // handle keys
    static SDL_Event event;

    bool keep_running = true;

    while (SDL_PollEvent(&event)) {
        // check to see if each event is one of our keys

        auto res = keybinds.find(event.key.keysym.sym);

        if (res != keybinds.end()) {
            if (event.type == SDL_KEYDOWN) {
                keys[res->second] = true;
            }
            else if (event.type == SDL_KEYUP) {
                keys[res->second] = false;
            }
        }

        // exit condition
        else if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
            keep_running = false;
        }
    }
    return keep_running;
}

void Chip8::update_timers() {
    if (timer.update()) {
        if (delay_timer > 0) {
            delay_timer--;
        }
        if (sound_timer > 0) {
            sound_timer--;
        }
    }
}

bool Chip8::cycle() {

    // maybe put update_timers() into loop
    // but then we'd ever so slightly delay execution
    while (!freq.update())
        ;

    update_timers();

    // fetch
    fetch();
    // decode
    operation = decode();
    // execute
    execute();

    if (should_draw) {
        should_draw = false;
        graphics.draw();
    }

    PC += 2;

    return update_keys();
}

void Chip8::run() {
    while (cycle())
        ;
}