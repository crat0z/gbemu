#ifndef REGISTERS_HPP
#define REGISTERS_HPP
#include <cstdint>

namespace core {
    struct Registers {
        union {
            uint16_t AF;
            struct {
                uint8_t NOTHING        : 4;

                uint8_t CARRY_FLAG     : 1;
                uint8_t HALFCARRY_FLAG : 1;
                uint8_t SUB_FLAG       : 1;
                uint8_t ZERO_FLAG      : 1;

                uint8_t A;
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
        uint16_t SP = 0xFFFE;
        uint16_t PC;
    } __attribute__((aligned(16)));
} // namespace core
#endif