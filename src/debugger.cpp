#include "debugger.hpp"
#include "opcodes.hpp"
#include <unordered_map>
#include <disassembler.hpp>
#include <iostream>
#include <fmt/ranges.h>
#include <thread>

Debugger::Debugger(Chip8& p) : proc{ p } {}

// save current emu state
void Debugger::save_emu_state() noexcept {
    prev_V      = proc.V;
    prev_memory = proc.memory;
    prev_PC     = proc.PC;
    prev_I      = proc.I;
    prev_dt     = proc.delay_timer;
    prev_st     = proc.sound_timer;
}

// update changed registers etc
void Debugger::update_state() noexcept {
    for (auto i = 0; i < 16; ++i) {
        reg_changes[i] = (prev_V[i] != proc.V[i]);
    }

    I_change  = (prev_I != proc.I);
    PC_change = (prev_PC != proc.PC);
    dt_change = (prev_dt != proc.delay_timer);
    st_change = (prev_st != proc.sound_timer);
}

void Debugger::set_destination(uint16_t addr) noexcept {
    destination = addr;
    debugging   = true;
}

// a debug cycle just wraps around normal cycle and updates debugger's state. reset_timer
// will reset the timer on emu, meaning this cycle will happen instantly
void Debugger::debug_cycle(bool reset_timer) noexcept {
    save_emu_state();

    if (reset_timer) {
        proc.timer.reset();
    }

    proc.cycle();

    update_state();
}

void Debugger::get_next_instruction() noexcept {
    // we can safely decode next instruction
    next_opcode = proc.fetch(proc.PC);

    // update our instruction info
    next_operation = decode(next_opcode);
}

void Debugger::reset() noexcept {
    for (auto i = 0; i < 16; ++i) {
        reg_changes[i] = false;
    }

    I_change  = false;
    PC_change = false;
    dt_change = false;
    st_change = false;
}

// use a mutex here
void Debugger::pause() noexcept {
    using namespace std::chrono_literals;

    emu_paused = true;
    debugging  = false;
    std::this_thread::sleep_for(10ms);

    reset();

    get_next_instruction();
};

void Debugger::unpause() noexcept {
    emu_paused = false;
    reset();
}

void Debugger::single_step() noexcept {
    if (is_paused()) {
        debug_cycle(true);
        set_destination(proc.PC);
    }
}

void Debugger::step_over() noexcept {
    // step over is a single step if it isn't a call
    if (is_paused()) {
        get_next_instruction();

        if (next_operation == op::CALL || next_operation == op::SYS) {
            set_destination(proc.PC + 2);
            unpause();
        }
        else {
            single_step();
        }
    }
}

void Debugger::step_out() noexcept {
    if (is_paused()) {
        if (!proc.stack.empty()) {
            set_destination(proc.stack.back() + 2);
            unpause();
        }
    }
}

void Debugger::cycle() noexcept {
    if (breakpoints[proc.PC] || reached_destination()) {
        pause();
    }
    else if (being_debugged()) {
        debug_cycle();
    }
    else {
        proc.cycle();
    }
}

bool Debugger::reached_destination() const noexcept { return proc.PC == destination; }

void Debugger::recv_destination() noexcept {
    destination = 0xffff;
    debugging   = false;
}

void Debugger::continue_emu() noexcept {
    destination = 0xffff;
    unpause();
}

void Debugger::set_breakpoint(uint16_t addr) noexcept { breakpoints[addr] = true; }

void Debugger::remove_breakpoint(uint16_t addr) noexcept { breakpoints[addr] = false; }

bool Debugger::is_breakpoint_set(uint16_t addr) const noexcept { return breakpoints[addr]; }

uint16_t Debugger::get_entry() const noexcept { return proc.entry_point; }

bool Debugger::being_debugged() const noexcept { return debugging; }
bool Debugger::is_ready() const noexcept { return proc.is_ready; }
bool Debugger::is_paused() const noexcept { return emu_paused; }
// only read values from this class if we are both paused and not being run by debugger
bool Debugger::is_readable() const noexcept { return is_paused() && !being_debugged(); }

uint16_t Debugger::fetch(uint16_t addr) noexcept { return proc.fetch(addr); }
op       Debugger::decode(uint16_t opc) noexcept { return proc.decode(opc); }

uint16_t Debugger::get_V(uint8_t reg) const noexcept { return proc.V[reg]; }
void     Debugger::set_V(uint8_t reg, uint8_t val) const noexcept { proc.V[reg] = val; }

uint16_t Debugger::get_I() const noexcept { return proc.I; }
void     Debugger::set_I(uint8_t val) const noexcept { proc.I = val; }

uint16_t Debugger::get_ST() const noexcept { return proc.sound_timer; }
void     Debugger::set_ST(uint8_t val) const noexcept { proc.sound_timer = val; }

uint16_t Debugger::get_DT() const noexcept { return proc.delay_timer; }
void     Debugger::set_DT(uint8_t val) const noexcept { proc.delay_timer = val; }

uint16_t Debugger::get_PC() const noexcept { return proc.PC; }
void     Debugger::set_PC(uint8_t val) const noexcept { proc.PC = val; }

void Debugger::reset_timer() const noexcept { proc.timer.reset(); }

uint16_t Debugger::get_opcode() const noexcept { return next_opcode; }
