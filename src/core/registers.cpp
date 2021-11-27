#include "core/registers.hpp"

constexpr uint8_t zero_flag      = 0x80;
constexpr uint8_t sub_flag       = 0x40;
constexpr uint8_t halfcarry_flag = 0x20;
constexpr uint8_t carry_flag     = 0x10;

namespace core {

    void Registers::xor_carry() { F ^= carry_flag; }

    void Registers::set_carry(bool cond) {
        if (cond) {
            F |= carry_flag;
        }
        else {
            reset_carry();
        }
    }
    void Registers::reset_carry() { F &= static_cast<uint8_t>(~carry_flag); }

    void Registers::set_halfcarry(bool cond) {
        if (cond) {
            F |= halfcarry_flag;
        }
        else {
            reset_halfcarry();
        }
    }
    void Registers::reset_halfcarry() { F &= static_cast<uint8_t>(~halfcarry_flag); }

    void Registers::set_sub(bool cond) {
        if (cond) {
            F |= sub_flag;
        }
        else {
            reset_sub();
        }
    }
    void Registers::reset_sub() { F &= static_cast<uint8_t>(~sub_flag); }

    void Registers::set_zero(bool cond) {
        if (cond) {
            F |= zero_flag;
        }
        else {
            reset_zero();
        }
    }
    void Registers::reset_zero() { F &= static_cast<uint8_t>(~zero_flag); }

    bool Registers::get_carry() const { return (F & carry_flag) != 0; }
    bool Registers::get_halfcarry() const { return (F & halfcarry_flag) != 0; }
    bool Registers::get_sub() const { return (F & sub_flag) != 0; }
    bool Registers::get_zero() const { return (F & zero_flag) != 0; }
} // namespace core