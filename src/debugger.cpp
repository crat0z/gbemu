#include "debugger.hpp"
#include "opcodes.hpp"
#include <unordered_map>
#include <disassembler.hpp>
#include <iostream>
#include <fmt/ranges.h>

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
    auto next = proc.fetch(proc.PC);

    // update our instruction info
    next_instruction = opcode_instruction(next);
    next_opcode      = decode(next);
    next_description = opcode_description(next_opcode);
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

bool Debugger::is_paused() const noexcept { return paused; }

// use a mutex here
void Debugger::pause() noexcept { paused = true; };

const char* Debugger::get_description() const noexcept { return next_description; }

const std::string& Debugger::get_instruction() const noexcept { return next_instruction; }

void Debugger::analyze() {
    disassembler dis(proc);
    dis.analyze();

    int count = 0;

    for (auto block : dis.result) {
        fmt::print("block {}\n", count);
        fmt::print("\tblock start address: {0:#03x}\n\tblock end address: {1:#03x}\n",
                   block->start_address, block->end_address);

        fmt::print("\treferenced by {} blocks:\n\t", block->from_blocks.size());

        for (auto i = 0; i < block->from_blocks.size(); ++i) {
            fmt::print("{0:#03x} ", block->from_blocks[i]->end_address);
        }

        fmt::print("\n\tdisassembly:\n");

        for (auto& ins : block->instructions) {
            fmt::print("\t\t{0:#03x}\t{1}\n", ins.address, ins.mnemonic);
        }
        count++;
    }
}