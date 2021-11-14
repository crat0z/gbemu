#ifndef REGISTER_VIEW_HPP
#define REGISTER_VIEW_HPP

#include "gui/debugger/dbgcomponent.hpp"

namespace GUI {
    class RegisterView : public DbgComponent {
    public:
        RegisterView(float fs, core::EmuWrapper& e);
        void draw_window() override;

        void process_message(GUIMessage m) override;
    };
} // namespace GUI
#endif