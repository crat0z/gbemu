#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include "disassembly_view.hpp"
#include "memory_view.hpp"
#include "register_view.hpp"
#include "stack_view.hpp"

namespace GUI {

    // a small class that doesn't actually draw any window, but mediates between debugger
    // components
    class Debugger : public GUIComponent {
    private:
        DisassemblyView dv;
        MemoryView      mv;
        RegisterView    rv;
        StackView       sv;

        void send_message(std::optional<DbgMessage> msg);

        void process_messages();

    public:
        Debugger(float fs, core::EmuWrapper& e);

        virtual void draw_window() override;
        virtual void toggle_window() override;

        bool& get_dv();
        bool& get_mv();
        bool& get_rv();
        bool& get_sv();
    };

} // namespace GUI

#endif