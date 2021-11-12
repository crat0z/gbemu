#ifndef EMUWRAPPER_HPP
#define EMUWRAPPER_HPP

#include "core/chip8.hpp"
#include <vector>

// a wrapper around Chip8 class to allow for debugging functionality
// without cluttering chip8 class itself

namespace core {

    class EmuWrapper {
        std::array<bool, 4096> breakpoints = {};

        std::array<uint8_t, 4096> prev_memory = {};
        std::array<uint8_t, 16>   prev_V      = {};

        uint16_t prev_I  = 0;
        uint16_t prev_PC = 0;

        uint8_t prev_dt = 0;
        uint8_t prev_st = 0;

        Chip8 proc;

        op       next_operation;
        uint16_t next_opcode;

        bool emu_paused;
        bool debugging;
        // some debugger functions e.g. step out have a "destination", this variable
        // will be set by those functions, and we can check every cycle if we've reached,
        // and if so, signal to caller (usually GUI windows) of this event, and change
        // state of debugger
        uint16_t destination;

        void get_next_instruction() noexcept;
        void save_emu_state() noexcept;
        void update_state() noexcept;
        void set_destination(uint16_t addr) noexcept;
        void debug_cycle(bool reset_timer = true) noexcept;

    public:
        std::array<bool, 16> reg_changes = {};

        bool I_change  = false;
        bool PC_change = false;
        bool dt_change = false;
        bool st_change = false;

        EmuWrapper();

        void new_game(const std::string& filepath, uint16_t entry, uint16_t addr, bool paused);

        std::array<std::array<bool, 64>, 32>& frame_buffer() noexcept;

        Stack<uint16_t>& get_stack() noexcept;

        uint16_t get_V(uint8_t reg) noexcept;
        void     set_V(uint8_t reg, uint8_t val) noexcept;

        uint16_t get_I() noexcept;
        void     set_I(uint8_t val) noexcept;

        uint16_t get_ST() noexcept;
        void     set_ST(uint8_t val) noexcept;

        uint16_t get_DT() noexcept;
        void     set_DT(uint8_t val) noexcept;

        uint16_t get_PC() noexcept;
        void     set_PC(uint8_t val) noexcept;

        std::array<uint8_t, 4096>& get_memory() noexcept;

        std::array<bool, 16>& get_keys() noexcept;

        void reset_timer() noexcept;

        // debugger functions
        void single_step() noexcept;
        void step_over() noexcept;
        void step_out() noexcept;
        void continue_emu() noexcept;

        void reset() noexcept;
        void pause() noexcept;
        void unpause() noexcept;

        void cycle() noexcept;
        bool reached_destination() const noexcept;
        void recv_destination() noexcept;

        const char* get_description() const noexcept;
        const char* get_instruction() const noexcept;
        uint16_t    get_opcode() const noexcept;

        uint16_t get_entry() const noexcept;

        bool is_paused() const noexcept;
        bool is_ready() const noexcept;
        bool being_debugged() const noexcept;
        bool is_readable() const noexcept;

        // breakpoint functions
        void set_breakpoint(uint16_t address) noexcept;
        void remove_breakpoint(uint16_t address) noexcept;
        bool is_breakpoint_set(uint16_t address) const noexcept;

        // delegates to Chip8 in case of changes in future
        uint16_t fetch(uint16_t addr) noexcept;
        op       decode(uint16_t opc) noexcept;
    };
} // namespace core

#endif