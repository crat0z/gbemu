#include "core/memory.hpp"

namespace {
    constexpr uint8_t VBlank = 0x1;
    constexpr uint8_t STAT   = 0x2;
    constexpr uint8_t Timer  = 0x4;
    constexpr uint8_t Serial = 0x8;
    constexpr uint8_t Joypad = 0x16;

    constexpr uint8_t TAC_select = 0x3;
    constexpr uint8_t TAC_enable = 0x4;
} // namespace

namespace core {

    uint8_t Memory::get8(uint16_t addr) { return memory[addr]; }

    uint16_t Memory::get16(uint16_t addr) {
        auto ret = *reinterpret_cast<uint16_t*>(&memory[addr]);
        return ret;
    }

    bool Memory::get_IE_VBlank() const { return IERegister & VBlank; }
    bool Memory::get_IE_STAT() const { return IERegister & STAT; }
    bool Memory::get_IE_Timer() const { return IERegister & Timer; }
    bool Memory::get_IE_Serial() const { return IERegister & Serial; }
    bool Memory::get_IE_Joypad() const { return IERegister & Joypad; }

    void Memory::reset_IE_VBlank() { IERegister &= ~(VBlank); }
    void Memory::reset_IE_STAT() { IERegister &= ~(STAT); }
    void Memory::reset_IE_Timer() { IERegister &= ~(Timer); }
    void Memory::reset_IE_Serial() { IERegister &= ~(Serial); }
    void Memory::reset_IE_Joypad() { IERegister &= ~(Joypad); }

    bool Memory::get_IF_VBlank() const { return IFRegister & VBlank; }
    bool Memory::get_IF_STAT() const { return IFRegister & STAT; }
    bool Memory::get_IF_Timer() const { return IFRegister & Timer; }
    bool Memory::get_IF_Serial() const { return IFRegister & Serial; }
    bool Memory::get_IF_Joypad() const { return IFRegister & Joypad; }

    void Memory::reset_IF_VBlank() { IFRegister &= ~(VBlank); }
    void Memory::reset_IF_STAT() { IFRegister &= ~(STAT); }
    void Memory::reset_IF_Timer() { IFRegister &= ~(Timer); }
    void Memory::reset_IF_Serial() { IFRegister &= ~(Serial); }
    void Memory::reset_IF_Joypad() { IFRegister &= ~(Joypad); }

    void Memory::set_IF_Timer() { IFRegister |= Timer; }

    bool Memory::get_TAC_enabled() { return TAC & TAC_enable; }

} // namespace core
