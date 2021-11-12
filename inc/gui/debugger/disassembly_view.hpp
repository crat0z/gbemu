#ifndef DISASSEMBLY_VIEW_HPP
#define DISASSEMBLY_VIEW_HPP

#include "gui/debugger/dbgcomponent.hpp"
#include "core/basicblock.hpp"
#include <unordered_map>

namespace GUI {
    class DisassemblyView : public DbgComponent {

        std::shared_ptr<core::basic_block> rec_cfg(uint16_t start_address, uint16_t from_address);
        std::vector<std::shared_ptr<core::basic_block>> control_flow_graph;
        std::unordered_map<uint16_t, core::Instruction> found_instructions;

        void analyze();

    public:
        DisassemblyView(core::EmuWrapper& e, float fs);

        void draw_window() override;

        void process_dbg_msg(DbgMessage m) override;
    };
} // namespace GUI
#endif