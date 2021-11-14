#ifndef MEMORY_VIEW_HPP
#define MEMORY_VIEW_HPP

#include "gui/debugger/dbgcomponent.hpp"

namespace GUI {
    class MemoryView : public DbgComponent {
    private:
        uint16_t next_scroll;

    public:
        MemoryView(float fs, core::EmuWrapper& e);

        void draw_window() override;

        void process_message(GUIMessage m) override;
    };
} // namespace GUI
#endif