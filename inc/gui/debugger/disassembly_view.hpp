#ifndef DISASSEMBLY_VIEW_HPP
#define DISASSEMBLY_VIEW_HPP

#include "gui/debugger/dbgcomponent.hpp"
#include "core/basicblock.hpp"
#include <unordered_map>
#include <stack>
#include <imgui.h>

namespace GUI {
    class DisassemblyView : public DbgComponent {

        std::shared_ptr<core::basic_block> rec_cfg(uint16_t start_address, uint16_t from_address);
        std::vector<std::shared_ptr<core::basic_block>> control_flow_graph;

        std::vector<core::Instruction> found_instructions;

        std::stack<float> bw_history;
        std::stack<float> fw_history;

        ImGuiListClipper clipper;

        uint16_t target_addr;
        float    target_scroll;

        float last_scroll_val;

        void analyze();

        bool  show_left();
        bool  show_right();
        float fix_float(float v);

        uint16_t fix_u16(uint16_t v);
        void     push();

        // queue up a new scroll target. optionally destroys forward history we've kept
        void queue_scroll(uint16_t addr, bool save_to_history = false);

        void set_value(float v);

        void set_value(uint16_t v);

        // actually do the scrolling, call this at particular spot
        void scroll_to_target();

        void go_back();

        void go_forward();

    public:
        DisassemblyView(core::EmuWrapper& e, float fs);

        void draw_window() override;

        void process_dbg_msg(DbgMessage m) override;
    };
} // namespace GUI
#endif