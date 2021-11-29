#include "core/emuwrapper.hpp"
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
    EmuWrapper::EmuWrapper() = default;

    void EmuWrapper::new_game(const std::string& filepath, uint16_t entry, uint16_t addr,
                              bool paused) {
        using namespace std::chrono_literals;

        emu_paused = paused;

        proc.is_ready = false;
        std::this_thread::sleep_for(100ms);

        proc.reset_state();
        proc.ctx.r.PC = entry;

        read_file(filepath, addr, proc.ctx.m.memory.data());

        proc.is_ready = true;
    }

    // save current emu state
    void EmuWrapper::save_emu_state() noexcept { prev_r = proc.ctx.r; }

    // update changed registers etc
    void EmuWrapper::update_state() noexcept {}

    void EmuWrapper::set_destination(uint16_t addr) noexcept {
        destination = addr;
        debugging   = true;
    }

    // a debug cycle just wraps around normal cycle and updates debugger's state. reset_timer
    // will reset the timer on emu, meaning this cycle will happen instantly
    void EmuWrapper::debug_cycle([[maybe_unused]] bool reset_timer) noexcept {
        save_emu_state();

        proc.cycle();

        update_state();
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

        //get_next_instruction();
    };

    void EmuWrapper::unpause() noexcept {
        emu_paused = false;
        reset();
    }

    void EmuWrapper::single_step() noexcept {
        if (is_paused()) {
            debug_cycle(true);
            set_destination(proc.ctx.r.PC);
        }
    }

    void EmuWrapper::step_over() noexcept {
        // step over is a single step if it isn't a call
        if (is_paused()) {
            //get_next_instruction();

            if (false) {
                set_destination(proc.ctx.r.PC + 2);
                unpause();
            }
            else {
                single_step();
            }
        }
    }

    void EmuWrapper::step_out() noexcept {
        if (is_paused()) {
            /* if (!proc.stack.empty()) {
                set_destination(proc.stack.back() + 2);
                unpause();
            } */
        }
    }

    void EmuWrapper::cycle() noexcept {
        // if we hit a breakpoint, we've reached a destination

        proc.cycle();
    }

    bool EmuWrapper::reached_destination() const noexcept { return proc.ctx.r.PC == destination; }

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

    bool EmuWrapper::being_debugged() const noexcept { return debugging; }
    bool EmuWrapper::is_ready() const noexcept { return proc.is_ready; }
    bool EmuWrapper::is_paused() const noexcept { return emu_paused; }
    // only read values from this class if we are both paused and not being run by debugger
    bool EmuWrapper::is_readable() const noexcept { return is_paused() && !being_debugged(); }

    uint16_t& EmuWrapper::get_PC() noexcept { return proc.ctx.r.PC; }
    void      EmuWrapper::set_PC(uint8_t val) noexcept { proc.ctx.r.PC = val; }

    uint16_t EmuWrapper::get_opcode() const noexcept { return next_opcode; }
} // namespace core