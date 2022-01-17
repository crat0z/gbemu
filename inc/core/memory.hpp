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

    struct Memory {
        union {
            struct {
                // 0000
                std::array<uint8_t, 0x4000> ROMbank0;
                // 4000
                std::array<uint8_t, 0x4000> ROMbank1;
                // 8000
                std::array<uint8_t, 0x2000> VideoRAM;
                // A000
                std::array<uint8_t, 0x2000> ExternalRAM;
                // C000
                std::array<uint8_t, 0x1000> WRAM0;
                // D000
                std::array<uint8_t, 0x1000> WRAM1;
                // E000
                std::array<uint8_t, 0x1E00> ECHORAM;
                // FE00
                std::array<uint8_t, 0xA0> SpriteAttributeTable;
                // FEA0
                std::array<uint8_t, 0x60> NotUsable;

                // FF00
                union {
                    struct {
                        uint8_t Controller;
                        uint8_t SB;
                        uint8_t SC;
                        uint8_t UNKNOWN_IO;
                        uint8_t DIV;
                        uint8_t TIMA;
                        uint8_t TMA;
                        uint8_t TAC;
                        // FF08
                        std::array<uint8_t, 0x7> IO_NOTHING;
                        // FF0F
                        uint8_t IFRegister;
                        // FF10
                        std::array<uint8_t, 0x17> SoundRegisters;
                        // FF27
                        std::array<uint8_t, 0x9> IO_NOTHING2;
                        // FF30
                        std::array<uint8_t, 0x10> WaveformRAM;
                        // FF40
                        std::array<uint8_t, 0xc> LCD;
                        // FF4C
                        std::array<uint8_t, 3> IO_NOTHING3;
                        // FF4F
                        uint8_t VRAM_Bank_Select;
                        // FF50
                        uint8_t Disable_Boot_ROM;
                        // FF51
                        std::array<uint8_t, 5> VRAM_DMA;
                        // FF56
                        std::array<uint8_t, 0x12> IO_NOTHING4;
                        // FF68
                        std::array<uint8_t, 0x2> BG_OBJ_palettes;
                        // FF70
                        uint8_t WRAM_Bank_Select;
                        // FF71
                        std::array<uint8_t, 0x15> IO_NOTHING5;
                    };
                    std::array<uint8_t, 0x80> IORegisters;
                };
                // FF80
                std::array<uint8_t, 0x7F> HighRAM;
                // FFFF
                uint8_t IERegister;
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

        bool get_IE_VBlank() const;
        bool get_IE_STAT() const;
        bool get_IE_Timer() const;
        bool get_IE_Serial() const;
        bool get_IE_Joypad() const;

        void reset_IE_VBlank();
        void reset_IE_STAT();
        void reset_IE_Timer();
        void reset_IE_Serial();
        void reset_IE_Joypad();

        bool get_IF_VBlank() const;
        bool get_IF_STAT() const;
        bool get_IF_Timer() const;
        bool get_IF_Serial() const;
        bool get_IF_Joypad() const;

        void reset_IF_VBlank();
        void reset_IF_STAT();
        void reset_IF_Timer();
        void reset_IF_Serial();
        void reset_IF_Joypad();

        void set_IF_Timer();

        bool get_TAC_enabled() const;

        uint8_t& operator[](uint16_t index);
    };

} // namespace core
#endif