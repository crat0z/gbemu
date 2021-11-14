#ifndef STACK_VIEW_HPP
#define STACK_VIEW_HPP

#include "gui/debugger/dbgcomponent.hpp"

namespace GUI {

    class StackView : public DbgComponent {

    public:
        StackView(float fs, core::EmuWrapper& e);
        void draw_window() override;

        void process_message(GUIMessage m) override;
    };
} // namespace GUI

#endif