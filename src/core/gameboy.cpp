#include <iostream>
#include <bitset>
#include <fstream>
#include <ctime>
#include <thread>
#include <future>
#include "core/gameboy.hpp"
#include "core/gameboy_opcodes.hpp"
#include "core/str_opcodes.hpp"

namespace core {
    Gameboy::Gameboy()
            : op_table(create_table()),
              cb_table(create_cb_table()),
              op_str_table(opcode_strings()),
              op_cb_str_table(cb_opcode_strings()) {
        is_ready = false;
    }

    void Gameboy::reset_state() {
        // just in case in future
        is_ready = false;

        ctx = {};

        cycle_count = 0;
    }

    uint16_t Gameboy::fetch(uint16_t addr) {

        //auto code = *reinterpret_cast<uint16_t*>(&ctx.m[addr]);

        return addr;
    }

    op Gameboy::decode([[maybe_unused]] uint16_t opc) { return op::UNKNOWN; }

    void Gameboy::execute() {}

    void Gameboy::cycle() {
        static auto last_cycle = 0;
        static auto result     = 0;

        while (result) {
            // wait for it
            while (!CPUTimer.update()) {}
            result--;
        }

        // check timers
        if (DIVTimer.update()) {
            ctx.DIV_inc();
        }
        ctx.TIMA_update(last_cycle);

        if (!ctx.halted) {
            auto op = ctx.imm8();

            if (op == 0xCB) {
                op     = ctx.imm8();
                result = cb_table[op](ctx);
            }
            else {
                result = op_table[op](ctx);
            }
            last_cycle = result;
        }

        // if EI/DI set this cycle, we don't check for interrupts
        if (ctx.interrupt_modified) {
            ctx.interrupt_modified = false;
        }
        else if (ctx.interrupt_pending()) {
            /*
                If no interrupt is pending, halt executes as normal, and the 
                CPU resumes regular execution as soon as an interrupt becomes pending. 
                However, since IME=0, the interrupt is not handled.
            */
            ctx.halted = false;
            // only handle if IME is enabled
            if (ctx.r.IME) {

                // pandocs says ISR setup lasts 5 m-cycles
                result += 5;
                last_cycle += 5;

                ctx.r.IME = false;

                auto jump_to_int = [&](uint8_t val) {
                    ctx.r.SP -= 2;
                    ctx.m.set16(ctx.r.SP, ctx.r.PC);
                    ctx.r.PC = val;
                };
                // priority is VBlank > ... > Joypad
                if (ctx.m.IF.VBlank) {
                    ctx.m.IF.VBlank = 0;
                    jump_to_int(0x40);
                }
                else if (ctx.m.IF.LCD_STAT) {
                    ctx.m.IF.VBlank = 0;
                    jump_to_int(0x48);
                }
                else if (ctx.m.IF.Timer) {
                    ctx.m.IF.Timer = 0;
                    jump_to_int(0x50);
                }
                else if (ctx.m.IF.Serial) {
                    ctx.m.IF.Serial = 0;
                    jump_to_int(0x58);
                }
                else if (ctx.m.IF.Joypad) {
                    ctx.m.IF.Joypad = 0;
                    jump_to_int(0x60);
                }
            }
        }
    }
} // namespace core
