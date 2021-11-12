#include "core/emuwrapper.hpp"
#include "core/opcodes.hpp"
#include <unordered_map>
#include <iostream>
#include <fmt/ranges.h>
#include <thread>
#include <fstream>

namespace {

    void read_file(const std::string& name, uint16_t addr, uint8_t* data) {
        std::ifstream file;
        file.open(name, std::ios_base::binary);
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(data + addr), size);
        file.close();
    }
} // namespace

namespace core {
    EmuWrapper::EmuWrapper() {}

    void EmuWrapper::new_game(const std::string& filepath, uint16_t addr, uint16_t entry,
                              bool paused) {
        using namespace std::chrono_literals;

        proc.is_ready = false;
        std::this_thread::sleep_for(100ms);

        proc.reset_state();
        proc.PC = entry;
        read_file(filepath, addr, proc.memory.data());

        proc.base_address = addr;
        proc.entry_point  = entry;

        proc.is_ready = true;
    }

    std::array<std::array<bool, X_PIXELS>, Y_PIXELS>& EmuWrapper::frame_buffer() noexcept {
        return proc.framebuffer;
    }

    // save current emu state
    void EmuWrapper::save_emu_state() noexcept {
        prev_V      = proc.V;
        prev_memory = proc.memory;
        prev_PC     = proc.PC;
        prev_I      = proc.I;
        prev_dt     = proc.delay_timer;
        prev_st     = proc.sound_timer;
    }

    // update changed registers etc
    void EmuWrapper::update_state() noexcept {
        for (auto i = 0; i < 16; ++i) {
            reg_changes[i] = (prev_V[i] != proc.V[i]);
        }

        I_change  = (prev_I != proc.I);
        PC_change = (prev_PC != proc.PC);
        dt_change = (prev_dt != proc.delay_timer);
        st_change = (prev_st != proc.sound_timer);
    }

    void EmuWrapper::set_destination(uint16_t addr) noexcept {
        destination = addr;
        debugging   = true;
    }

    // a debug cycle just wraps around normal cycle and updates debugger's state. reset_timer
    // will reset the timer on emu, meaning this cycle will happen instantly
    void EmuWrapper::debug_cycle(bool reset_timer) noexcept {
        save_emu_state();

        if (reset_timer) {
            proc.timer.reset();
        }

        proc.cycle();

        update_state();
    }

    void EmuWrapper::get_next_instruction() noexcept {
        // we can safely decode next instruction
        next_opcode = proc.fetch(proc.PC);

        // update our instruction info
        next_operation = decode(next_opcode);
    }

    void EmuWrapper::reset() noexcept {
        for (auto i = 0; i < 16; ++i) {
            reg_changes[i] = false;
        }

        I_change  = false;
        PC_change = false;
        dt_change = false;
        st_change = false;
    }

    // use a mutex here
    void EmuWrapper::pause() noexcept {
        using namespace std::chrono_literals;

        emu_paused = true;
        debugging  = false;
        std::this_thread::sleep_for(10ms);

        reset();

        get_next_instruction();
    };

    void EmuWrapper::unpause() noexcept {
        emu_paused = false;
        reset();
    }

    void EmuWrapper::single_step() noexcept {
        if (is_paused()) {
            debug_cycle(true);
            set_destination(proc.PC);
        }
    }

    void EmuWrapper::step_over() noexcept {
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

    void EmuWrapper::step_out() noexcept {
        if (is_paused()) {
            if (!proc.stack.empty()) {
                set_destination(proc.stack.back() + 2);
                unpause();
            }
        }
    }

    void EmuWrapper::cycle() noexcept {
        // if we hit a breakpoint, we've reached a destination
        if (breakpoints[proc.PC]) {
            destination = proc.PC;
            pause();
        }
        else if (reached_destination()) {
            pause();
        }
        else if (being_debugged()) {
            debug_cycle();
        }
        else {
            proc.cycle();
        }
    }

    bool EmuWrapper::reached_destination() const noexcept { return proc.PC == destination; }

    void EmuWrapper::recv_destination() noexcept {
        destination = 0xffff;
        debugging   = false;
    }

    void EmuWrapper::continue_emu() noexcept {
        destination = 0xffff;
        unpause();
    }

    void EmuWrapper::set_breakpoint(uint16_t addr) noexcept { breakpoints[addr] = true; }

    void EmuWrapper::remove_breakpoint(uint16_t addr) noexcept { breakpoints[addr] = false; }

    bool EmuWrapper::is_breakpoint_set(uint16_t addr) const noexcept { return breakpoints[addr]; }

    uint16_t EmuWrapper::get_entry() const noexcept { return proc.entry_point; }

    bool EmuWrapper::being_debugged() const noexcept { return debugging; }
    bool EmuWrapper::is_ready() const noexcept { return proc.is_ready; }
    bool EmuWrapper::is_paused() const noexcept { return emu_paused; }
    // only read values from this class if we are both paused and not being run by debugger
    bool EmuWrapper::is_readable() const noexcept { return is_paused() && !being_debugged(); }

    uint16_t EmuWrapper::fetch(uint16_t addr) noexcept { return proc.fetch(addr); }
    op       EmuWrapper::decode(uint16_t opc) noexcept { return proc.decode(opc); }

    Stack<uint16_t>& EmuWrapper::get_stack() noexcept { return proc.stack; }

    uint16_t EmuWrapper::get_V(uint8_t reg) noexcept { return proc.V[reg]; }
    void     EmuWrapper::set_V(uint8_t reg, uint8_t val) noexcept { proc.V[reg] = val; }

    uint16_t EmuWrapper::get_I() noexcept { return proc.I; }
    void     EmuWrapper::set_I(uint8_t val) noexcept { proc.I = val; }

    uint16_t EmuWrapper::get_ST() noexcept { return proc.sound_timer; }
    void     EmuWrapper::set_ST(uint8_t val) noexcept { proc.sound_timer = val; }

    uint16_t EmuWrapper::get_DT() noexcept { return proc.delay_timer; }
    void     EmuWrapper::set_DT(uint8_t val) noexcept { proc.delay_timer = val; }

    uint16_t EmuWrapper::get_PC() noexcept { return proc.PC; }
    void     EmuWrapper::set_PC(uint8_t val) noexcept { proc.PC = val; }

    std::array<uint8_t, MAX_MEMORY>& EmuWrapper::get_memory() noexcept { return proc.memory; }

    std::array<bool, 16>& EmuWrapper::get_keys() noexcept { return proc.keys; }

    void EmuWrapper::reset_timer() noexcept { proc.timer.reset(); }

    uint16_t EmuWrapper::get_opcode() const noexcept { return next_opcode; }
} // namespace core