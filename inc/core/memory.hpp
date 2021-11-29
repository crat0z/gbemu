#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <array>
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

    struct Interrupts {
        uint8_t VBlank   : 1;
        uint8_t LCD_STAT : 1;
        uint8_t Timer    : 1;
        uint8_t Serial   : 1;
        uint8_t Joypad   : 1;
    };

    struct TAC {
        uint8_t InputClockSelect : 2;
        uint8_t TimerEnable      : 1;
    };

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

                union {
                    struct {
                        std::array<uint8_t, 0x4> IO_NOTDONE1;

                        uint8_t DIV;
                        uint8_t TIMA;
                        uint8_t TMA;
                        TAC     TAC;

                        std::array<uint8_t, 0x7> IO_NOTDONE3;
                        union {
                            Interrupts IF;
                            uint8_t    IFRegister;
                        };
                        std::array<uint8_t, 0x70> IO_NOTDONE2;
                    };
                    std::array<uint8_t, 0x80> IORegisters;
                };

                std::array<uint8_t, 0x7F> HighRAM;
                union {
                    Interrupts IE;
                    uint8_t    IERegister;
                };
            };
            std::array<uint8_t, 65536> memory;
        };

#ifdef GBEMU_CLANG
#pragma clang diagnostic pop
#pragma clang diagnostic pop
#endif

#ifdef GBEMU_GCC
#pragma GCC diagnostic pop
#endif

        uint8_t  imm8(uint16_t& PC);
        uint16_t imm16(uint16_t& PC);

        uint8_t  get8(uint16_t addr);
        uint16_t get16(uint16_t addr);

        void set16(uint16_t addr, uint16_t val);

        uint8_t& operator[](uint16_t index);
    } __attribute__((aligned(65536)));

} // namespace core
#endif