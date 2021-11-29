#include <chrono>
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
        CPUTimer.reset();
        DIVTimer.reset();
        cycle_count = 0;
    }

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

        //std::cout << ctx.print();

        if (!ctx.halted) {
            auto op = ctx.imm8();

            if (op == 0xCB) {
                op = ctx.imm8();
                //std::cout << op_cb_str_table[op](ctx);
                //std::cout << '\n';
                result = cb_table[op](ctx);
            }
            else {
                //std::cout << op_str_table[op](ctx);
                //std::cout << '\n';
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
                if (ctx.m.get_IF_VBlank()) {
                    ctx.m.reset_IF_VBlank();
                    jump_to_int(0x40);
                }
                else if (ctx.m.get_IF_STAT()) {
                    ctx.m.reset_IF_STAT();
                    jump_to_int(0x48);
                }
                else if (ctx.m.get_IF_Timer()) {
                    ctx.m.reset_IF_Timer();
                    jump_to_int(0x50);
                }
                else if (ctx.m.get_IF_Serial()) {
                    ctx.m.reset_IF_Serial();
                    jump_to_int(0x58);
                }
                else if (ctx.m.get_IF_Joypad()) {
                    ctx.m.reset_IF_Joypad();
                    jump_to_int(0x60);
                }
            }
        }
        cycle_count += result;
    }
} // namespace core
