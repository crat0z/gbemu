#ifndef REGISTER_VIEW_HPP
#define REGISTER_VIEW_HPP

#include "gui/debugger/dbgcomponent.hpp"

namespace GUI {
    class RegisterView : public DbgComponent {
    public:
        RegisterView(core::EmuWrapper& e, float fs);
        void draw_window() override;

        void process_dbg_msg(DbgMessage m) override;
    };
} // namespace GUI
#endif