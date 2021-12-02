#include <chrono>
#include <iostream>
#include <fstream>
#include <future>
#include "core/gameboy.hpp"
#include "core/gameboy_opcodes.hpp"

namespace core {
    Gameboy::Gameboy()
            : op_table(create_table()),
              cb_table(create_cb_table()),
              memory(std::make_shared<Memory>()),
              ctx(memory) {}

    void Gameboy::reset_state() {
        // just in case in future
        is_ready = false;

        *memory = {};
        CPUTimer.reset();
        DIVTimer.reset();
        cycle_count = 0;
    }

    void Gameboy::cycle() {
        static auto last_cycle = 0;
        static auto result     = 0;

        // check if PC changes at end of this cycle, if not, we're done
        auto PC = ctx.r.PC;

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
                op = ctx.imm8();
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
                    ctx.write16(ctx.r.SP, ctx.r.PC);
                    ctx.r.PC = val;
                };
                // priority is VBlank > ... > Joypad
                if (ctx.m->get_IF_VBlank()) {
                    ctx.m->reset_IF_VBlank();
                    jump_to_int(0x40);
                }
                else if (ctx.m->get_IF_STAT()) {
                    ctx.m->reset_IF_STAT();
                    jump_to_int(0x48);
                }
                else if (ctx.m->get_IF_Timer()) {
                    ctx.m->reset_IF_Timer();
                    jump_to_int(0x50);
                }
                else if (ctx.m->get_IF_Serial()) {
                    ctx.m->reset_IF_Serial();
                    jump_to_int(0x58);
                }
                else if (ctx.m->get_IF_Joypad()) {
                    ctx.m->reset_IF_Joypad();
                    jump_to_int(0x60);
                }
            }
        }
        cycle_count += static_cast<size_t>(result);

        // ppu logic here

        if (PC == ctx.r.PC) {
            is_ready = false;
        }
    }
} // namespace core
