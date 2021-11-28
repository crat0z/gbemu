#ifndef REGISTERS_HPP
#define REGISTERS_HPP
#include <cstdint>

namespace core {
#ifdef GBEMU_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#ifdef GBEMU_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

    struct Registers {
        union {
            uint16_t AF;
            struct {
                uint8_t F;
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
        bool     IME;

        void xor_carry();
        void set_carry(bool cond = true);
        void reset_carry();

        void set_halfcarry(bool cond = true);
        void reset_halfcarry();

        void set_sub(bool cond = true);
        void reset_sub();

        void set_zero(bool cond = true);
        void reset_zero();

        bool get_carry() const;
        bool get_halfcarry() const;
        bool get_sub() const;
        bool get_zero() const;
    };

#ifdef GBEMU_CLANG
#pragma clang diagnostic pop
#pragma clang diagnostic pop
#endif

#ifdef GBEMU_GCC
#pragma GCC diagnostic pop
#endif
} // namespace core
#endif