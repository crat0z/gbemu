#include "debugger.hpp"
#include "opcodes.hpp"

Debugger::Debugger(Chip8& p) : proc{ p } {}

void Debugger::single_step() {
    // save all the relevant state from before we execute
    prev_V      = proc.V;
    prev_memory = proc.memory;
    prev_PC     = proc.PC;
    prev_I      = proc.I;
    prev_dt     = proc.delay_timer;
    prev_st     = proc.sound_timer;

    proc.timer.reset();
    proc.cycle();

    // update changed registers
    for (auto i = 0; i < 16; ++i) {
        reg_changes[i] = (prev_V[i] != proc.V[i]);
    }

    I_change  = (prev_I != proc.I);
    PC_change = (prev_PC != proc.PC);
    dt_change = (prev_dt != proc.delay_timer);
    st_change = (prev_st != proc.sound_timer);

    // we can safely decode next instruction
    proc.fetch();
}

void Debugger::reset() {
    for (auto i = 0; i < 16; ++i) {
        reg_changes[i] = false;
    }

    I_change  = false;
    PC_change = false;
    dt_change = false;
    st_change = false;
}