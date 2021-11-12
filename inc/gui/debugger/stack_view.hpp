#ifndef STACK_VIEW_HPP
#define STACK_VIEW_HPP

#include "gui/debugger/dbgcomponent.hpp"

namespace GUI {

    class StackView : public DbgComponent {

    public:
        StackView(core::EmuWrapper& e, float fs);
        void draw_window() override;

        void process_dbg_msg(DbgMessage m) override;
    };
} // namespace GUI

#endif