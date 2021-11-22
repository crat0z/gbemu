#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include "core/bytes.hpp"
#include "core/timer.hpp"
#include "core/opcodes.hpp"
#include "core/stack.hpp"

struct Registers {
    union {

        struct {
            uint8_t NOTHING        : 4;

            uint8_t CARRY_FLAG     : 1;
            uint8_t HALFCARRY_FLAG : 1;
            uint8_t SUB_FLAG       : 1;
            uint8_t ZERO_FLAG      : 1;

            uint8_t A;
        };

        struct {
            uint16_t AF;
        };
    };
    union {
        uint16_t BC;
        struct {
            uint8_t C;
            uint8_t B;
        };
    };
    union {
        uint16_t DE;
        struct {
            uint8_t E;
            uint8_t D;
        };
    };
    union {
        uint16_t HL;
        struct {
            uint8_t L;
            uint8_t H;
        };
    };
    uint16_t SP;
    uint16_t PC;
};

struct RAM {
    union {
        struct {
            std::array<uint8_t, 0x4000> ROMbank0;
            std::array<uint8_t, 0x4000> ROMbank1;
            std::array<uint8_t, 0x2000> VideoRAM;
            std::array<uint8_t, 0x2000> ExternalRAM;
            std::array<uint8_t, 0x1000> WRAM0;
            std::array<uint8_t, 0x1000> WRAM1;
            std::array<uint8_t, 0x1E00> ECHORAM;
            std::array<uint8_t, 0xA0>   SpriteAttributeTable;
            std::array<uint8_t, 0x60>   NotUsable;
            std::array<uint8_t, 0x80>   IORegisters;
            std::array<uint8_t, 0x7F>   HighRAM;
            uint8_t                     IERegister;
        };
        std::array<uint8_t, 65536> memory;
    };
    uint8_t imm8(uint16_t& PC) {
        auto ret = memory[PC];
        PC += 1;
        return ret;
    }
    uint16_t imm16(uint16_t& PC) {
        auto ret = *reinterpret_cast<uint16_t*>(&memory[PC]);
        PC += 2;
        return swap_byte_order(ret);
    }
    uint8_t& operator[](uint16_t index) { return memory[index]; }
} __attribute__((aligned(65536)));

namespace core {
    class Gameboy {

    private:
        friend class EmuWrapper;

        RAM       ram;
        Registers r;

        std::array<int (*)(Registers&, RAM&), 256> op_table;
        std::array<int (*)(Registers&, RAM&), 256> cb_table;

        op       operation;
        uint16_t opcode;
        size_t   cycle_count;
        bool     is_ready;

        uint16_t fetch(uint16_t addr);
        op       decode(uint16_t opc);
        void     execute();

        void cycle();

        // read file on filesystem with name, write to emu memory beginning @ addr
        void read_file(const std::string& name, uint16_t addr);
        void reset_state();

    public:
        Gameboy();
    };
} // namespace core

#endif