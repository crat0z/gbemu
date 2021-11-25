#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <array>
#include <cstdint>

namespace core {
    struct Memory {
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
        uint8_t  imm8(uint16_t& PC);
        uint16_t imm16(uint16_t& PC);

        uint8_t  get8(uint16_t addr);
        uint16_t get16(uint16_t addr);

        void set16(uint16_t addr, uint16_t val);

        uint8_t& operator[](uint16_t index);
    } __attribute__((aligned(65536)));
} // namespace core
#endif