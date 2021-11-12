#include "gui/debugger/debugger.hpp"

namespace GUI {

    Debugger::Debugger(float fs, core::EmuWrapper& e)
            : GUIComponent(fs), dv(e, fs), mv(e, fs), rv(e, fs), sv(e, fs) {}

    void Debugger::draw_window() {

        if (dv.is_enabled()) {
            dv.draw_window();
        }
        if (mv.is_enabled()) {
            mv.draw_window();
        }
        if (rv.is_enabled()) {
            rv.draw_window();
        }
        if (sv.is_enabled()) {
            sv.draw_window();
        }

        process_messages();
    }

    void Debugger::send_message(std::optional<DbgMessage> msg) {
        if (msg.has_value()) {
            auto m = msg.value();
            switch (m.target) {
            case dbg_component::disassembly_view:
                dv.process_dbg_msg(m);
            case dbg_component::memory_view:
                mv.process_dbg_msg(m);
            case dbg_component::register_view:
                rv.process_dbg_msg(m);
            case dbg_component::stack_view:
                sv.process_dbg_msg(m);
            }
        }
    }

    void Debugger::process_messages() {
        send_message(dv.get_message());
        send_message(mv.get_message());
        send_message(rv.get_message());
        send_message(sv.get_message());
    }

    void Debugger::toggle_window() {
        if (window_state) {
            dv.is_enabled() = false;
            mv.is_enabled() = false;
            rv.is_enabled() = false;
            sv.is_enabled() = false;

            window_state = false;
        }
        else {
            dv.is_enabled() = true;
            mv.is_enabled() = true;
            rv.is_enabled() = true;
            sv.is_enabled() = true;

            window_state = true;
        }
    }

    bool& Debugger::get_dv() { return dv.is_enabled(); }
    bool& Debugger::get_mv() { return mv.is_enabled(); }
    bool& Debugger::get_rv() { return rv.is_enabled(); }
    bool& Debugger::get_sv() { return sv.is_enabled(); }

} // namespace GUI