#include "core/memory.hpp"

namespace {
    constexpr uint8_t VBlank = 0x1;
    constexpr uint8_t STAT   = 0x2;
    constexpr uint8_t Timer  = 0x4;
    constexpr uint8_t Serial = 0x8;
    constexpr uint8_t Joypad = 0x16;

    //constexpr uint8_t TAC_select = 0x3;
    constexpr uint8_t TAC_enable = 0x4;
} // namespace

namespace core {

    uint8_t Memory::get8(uint16_t addr) { return memory[addr]; }

    uint16_t Memory::get16(uint16_t addr) {
        auto ret = *reinterpret_cast<uint16_t*>(&memory[addr]);
        return ret;
    }

    bool Memory::get_IE_VBlank() const { return (IERegister & VBlank) != 0; }
    bool Memory::get_IE_STAT() const { return (IERegister & STAT) != 0; }
    bool Memory::get_IE_Timer() const { return (IERegister & Timer) != 0; }
    bool Memory::get_IE_Serial() const { return (IERegister & Serial) != 0; }
    bool Memory::get_IE_Joypad() const { return (IERegister & Joypad) != 0; }

    void Memory::reset_IE_VBlank() { IERegister &= static_cast<uint8_t>(~(VBlank)); }
    void Memory::reset_IE_STAT() { IERegister &= static_cast<uint8_t>(~(STAT)); }
    void Memory::reset_IE_Timer() { IERegister &= static_cast<uint8_t>(~(Timer)); }
    void Memory::reset_IE_Serial() { IERegister &= static_cast<uint8_t>(~(Serial)); }
    void Memory::reset_IE_Joypad() { IERegister &= static_cast<uint8_t>(~(Joypad)); }

    bool Memory::get_IF_VBlank() const { return (IFRegister & VBlank) != 0; }
    bool Memory::get_IF_STAT() const { return (IFRegister & STAT) != 0; }
    bool Memory::get_IF_Timer() const { return (IFRegister & Timer) != 0; }
    bool Memory::get_IF_Serial() const { return (IFRegister & Serial) != 0; }
    bool Memory::get_IF_Joypad() const { return (IFRegister & Joypad) != 0; }

    void Memory::reset_IF_VBlank() { IFRegister &= static_cast<uint8_t>(~(VBlank)); }
    void Memory::reset_IF_STAT() { IFRegister &= static_cast<uint8_t>(~(STAT)); }
    void Memory::reset_IF_Timer() { IFRegister &= static_cast<uint8_t>(~(Timer)); }
    void Memory::reset_IF_Serial() { IFRegister &= static_cast<uint8_t>(~(Serial)); }
    void Memory::reset_IF_Joypad() { IFRegister &= static_cast<uint8_t>(~(Joypad)); }

    void Memory::set_IF_Timer() { IFRegister |= Timer; }

    bool Memory::get_TAC_enabled() const { return (TAC & TAC_enable) != 0; }

} // namespace core
