#include "chip8.hpp"
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <bitset>
#include <fstream>

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

uint8_t  get8(uint16_t op) { return op & 0xFF; }
uint16_t get12(uint16_t op) { return op & 0xFFF; }
uint16_t swap_byte_order(uint16_t s) { return (s >> 8) | (s << 8); }

void CPU::init(const std::string& file_name) {

    read_file(file_name);

    // more init stuff i guess
    for (auto i = 0; i < 80; ++i) {
        memory[i] = fontset[i];
    }

    window = SDL_CreateWindow("emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 640,
                              SDL_WINDOW_SHOWN);

    // Setup renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void CPU::read_file(const std::string& name) {
    std::ifstream file;
    file.open(name, std::ios_base::binary);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(&memory[0x200]), size);
    file.close();
}

void CPU::draw_graphics() {
    should_draw = false;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);
    // each square is 20x20
    SDL_Rect rect;
    rect.w = 20;
    rect.h = 20;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (auto y = 0; y < 32; ++y) {
        for (auto x = 0; x < 64; ++x) {
            if (graphics[y][x]) {
                rect.y = y * 20;
                rect.x = x * 20;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

char getch() {
    char           buf = 0;
    struct termios old = { 0 };
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN]  = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

uint8_t get_key() {
    while (true) {
        char c = getch();
        switch (c) {
        case '1':
            return 0x0;
        case '2':
            return 0x1;
        case '3':
            return 0x2;
        case '4':
            return 0x3;
        case 'q':
            return 0x4;
        case 'w':
            return 0x5;
        case 'e':
            return 0x6;
        case 'r':
            return 0x7;
        case 'a':
            return 0x8;
        case 's':
            return 0x9;
        case 'd':
            return 0xA;
        case 'f':
            return 0xB;
        case 'z':
            return 0xC;
        case 'x':
            return 0xD;
        case 'c':
            return 0xE;
        case 'v':
            return 0xF;
        }
    }
}

void CPU::fetch() {
    opcode = *reinterpret_cast<uint16_t*>(&memory[PC]);

    // CHIP8 is big endian, swap byte order
    opcode = swap_byte_order(opcode);

    // store each 4 bits of opcode into array
    for (auto i = 0; i < 4; ++i) {
        val[i] = (opcode >> (4 * (3 - i))) & 0xF;
    }
}

op CPU::decode() {

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

void CPU::execute() {
    auto imm8  = get8(opcode);
    auto imm12 = get12(opcode);
    switch (operation) {
        // for call/jump instructions, we unconditionally add 2 to PC every cycle
        // so by adjusting by -2, we can skip a conditional check
    case op::CALL: {
        stack.emplace(PC);
        PC = (imm12)-2;
        break;
    }
    case op::CLEAR: {
        // clear screen TODO
        for (auto& line : graphics) {
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
        if (V[val[1]] == imm8) {
            PC += 2;
        }
        break;
    }
    case op::IF_NOT_IMM: {
        if (V[val[1]] != imm8) {
            PC += 2;
        }
        break;
    }
    case op::IF_REG: {
        if (V[val[1]] == V[val[2]]) {
            PC += 2;
        }
        break;
    }
    case op::SET_IMM: {
        V[val[1]] = imm8;
        break;
    }
    case op::ADD_IMM: {
        V[val[1]] += imm8;
        break;
    }
    case op::SET_REG: {
        V[val[1]] = V[val[2]];
        break;
    }
    case op::OR: {
        V[val[1]] |= V[val[2]];
        break;
    }
    case op::AND: {
        V[val[1]] &= V[val[2]];
        break;
    }
    case op::XOR: {
        V[val[1]] ^= V[val[2]];
        break;
    }
    case op::ADD_REG: {
        auto original = V[val[1]];
        V[val[1]] += V[val[2]];
        // no overflow case
        if (original < V[val[1]]) {
            V[15] = 0;
        }
        else {
            V[15] = 1;
        }
        break;
    }
    case op::SUB_REG: {
        auto original = V[val[1]];
        V[val[1]] -= V[val[2]];
        // no borrow case
        if (original > V[val[1]]) {
            V[15] = 1;
        }
        else {
            V[15] = 0;
        }
        break;
    }
    case op::RSHIFT: {
        V[15] = V[val[1]] & 0x1;
        V[val[1]] >>= 1;
        break;
    }
    case op::SUB_REG2: {
        auto original = V[val[2]];
        V[val[2]] -= V[val[1]];
        // no borrow case
        if (original > V[val[2]]) {
            V[15] = 1;
        }
        else {
            V[15] = 0;
        }
        break;
    }
    case op::LSHIFT: {
        V[15] = V[val[1]] >> 7;
        V[val[1]] <<= 1;
        break;
    }
    case op::IF_NOT_REG: {
        if (V[val[1]] != V[val[2]]) {
            PC += 2;
        }
        break;
    }
    case op::SET_I: {
        I = imm12;
        break;
    }
    case op::JMP_OFFSET: {
        PC = V[0] + imm12;
        break;
    }
    case op::RAND_BITSET: {
        uint8_t r = std::rand() & 0xFF;
        V[val[1]] = r & imm8;
        break;
    }
    case op::DRAW: {

        auto x = V[val[1]];
        auto y = V[val[2]];
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
                if (!pixel_unset && graphics[currY][currX] && line[7 - j]) {
                    pixel_unset = true;
                }
                graphics[currY][currX] ^= line[7 - j];
            }
        }

        if (pixel_unset)
            V[15] = 1;
        else
            V[15] = 0;

        should_draw = true;
        break;
    }
    case op::SKIP_KEY: {
        if (keys[val[1]]) {
            PC += 2;
        }
        break;
    }
    case op::SKIP_NOT_KEY: {
        if (!keys[val[1]]) {
            PC += 2;
        }
        break;
    }
    case op::DELAY: {
        V[val[1]] = delay_timer;
        break;
    }
    case op::GET_KEY: {
        bool wait = true;
        while (wait) {
            update_keys();
            for (auto i = 0; i < 16; ++i) {
                if (keys[i]) {
                    V[val[1]] = static_cast<uint8_t>(i);
                    wait      = false;
                    break;
                }
            }
        }
        break;
    }
    case op::SET_DELAY: {
        delay_timer = V[val[1]];
        break;
    }
    case op::SET_SOUND: {
        sound_timer = V[val[1]];
        break;
    }
    case op::ADD_I: {
        I += V[val[1]];
        break;
    }
    case op::SET_I_SPRITE: {
        uint8_t c = V[val[1]];
        I         = c * 5;
        break;
    }
    case op::STORE_BCD: {
        uint8_t first  = V[val[1]] / 100;
        uint8_t second = (V[val[1]] / 10) % 10;
        uint8_t third  = V[val[1]] % 10;

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

void CPU::update_keys() {
    // handle keys
    static SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
            case SDLK_1: {
                keys[0] = true;
                break;
            }
            case SDLK_2: {
                keys[1] = true;
                break;
            }
            case SDLK_3: {
                keys[2] = true;
                break;
            }
            case SDLK_4: {
                keys[3] = true;
                break;
            }
            case SDLK_q: {
                keys[4] = true;
                break;
            }
            case SDLK_w: {
                keys[5] = true;
                break;
            }
            case SDLK_e: {
                keys[6] = true;
                break;
            }
            case SDLK_r: {
                keys[7] = true;
                break;
            }
            case SDLK_a: {
                keys[8] = true;
                break;
            }
            case SDLK_s: {
                keys[9] = true;
                break;
            }
            case SDLK_d: {
                keys[10] = true;
                break;
            }
            case SDLK_f: {
                keys[11] = true;
                break;
            }
            case SDLK_z: {
                keys[12] = true;
                break;
            }
            case SDLK_x: {
                keys[13] = true;
                break;
            }
            case SDLK_c: {
                keys[14] = true;
                break;
            }
            case SDLK_v: {
                keys[15] = true;
                break;
            }
            default: {
                break;
            }
            }
            break;
        }
        case SDL_KEYUP: {
            switch (event.key.keysym.sym) {
            case SDLK_1: {
                keys[0] = false;
                break;
            }
            case SDLK_2: {
                keys[1] = false;
                break;
            }
            case SDLK_3: {
                keys[2] = false;
                break;
            }
            case SDLK_4: {
                keys[3] = false;
                break;
            }
            case SDLK_q: {
                keys[4] = false;
                break;
            }
            case SDLK_w: {
                keys[5] = false;
                break;
            }
            case SDLK_e: {
                keys[6] = false;
                break;
            }
            case SDLK_r: {
                keys[7] = false;
                break;
            }
            case SDLK_a: {
                keys[8] = false;
                break;
            }
            case SDLK_s: {
                keys[9] = false;
                break;
            }
            case SDLK_d: {
                keys[10] = false;
                break;
            }
            case SDLK_f: {
                keys[11] = false;
                break;
            }
            case SDLK_z: {
                keys[12] = false;
                break;
            }
            case SDLK_x: {
                keys[13] = false;
                break;
            }
            case SDLK_c: {
                keys[14] = false;
                break;
            }
            case SDLK_v: {
                keys[15] = false;
                break;
            }
            default: {
                break;
            }
            }
            break;
        }
        }
    }
}

void CPU::cycle() {
    // fetch
    fetch();
    // decode
    operation = decode();
    // execute
    execute();

    if (should_draw)
        draw_graphics();

    update_keys();

    PC += 2;
}