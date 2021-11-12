#ifndef MEMORY_VIEW_HPP
#define MEMORY_VIEW_HPP

#include "gui/debugger/dbgcomponent.hpp"

namespace GUI {
    class MemoryView : public DbgComponent {
    private:
        uint16_t next_scroll;

    public:
        MemoryView(core::EmuWrapper& e, float fs);

        void draw_window() override;

        void process_dbg_msg(DbgMessage m) override;
    };
} // namespace GUI
#endif