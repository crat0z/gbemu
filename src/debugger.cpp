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
void Debugger::get_changes() noexcept {
    for (auto i = 0; i < 16; ++i) {
        reg_changes[i] = (prev_V[i] != proc.V[i]);
    }

    I_change  = (prev_I != proc.I);
    PC_change = (prev_PC != proc.PC);
    dt_change = (prev_dt != proc.delay_timer);
    st_change = (prev_st != proc.sound_timer);
}

void Debugger::get_next_instruction() noexcept {
    // we can safely decode next instruction
    next_opcode = proc.fetch(proc.PC);

    // update our instruction info
    next_instruction = opcode_instruction(next_opcode);
    next_operation   = decode(next_opcode);
    next_description = opcode_description(next_operation);
}

void Debugger::single_step() {
    // save all the relevant state from before we execute
    save_emu_state();

    proc.timer.reset();
    proc.cycle();

    get_changes();

    get_next_instruction();
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

bool Debugger::is_paused() const noexcept { return proc.is_paused; }

// use a mutex here
void Debugger::pause() noexcept {
    using namespace std::chrono_literals;

    proc.is_paused = true;
    std::this_thread::sleep_for(10ms);

    reset();

    get_next_instruction();
};

void Debugger::run() noexcept {
    proc.is_paused = false;
    reset();
}

const char* Debugger::get_description() const noexcept { return next_description; }

const char* Debugger::get_instruction() const noexcept { return next_instruction.c_str(); }

uint16_t Debugger::get_opcode() const noexcept { return next_opcode; }

uint16_t Debugger::get_PC() const noexcept { return proc.PC; }
