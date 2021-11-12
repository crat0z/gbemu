#include "gui/debugger/disassembly_view.hpp"
#include "gui/imgui_helpers.hpp"
#include <fmt/format.h>
#include <stack>
#include "gui/icons.hpp"
#include "global.hpp"

// FIX IMMEDIATELY!!!!!!!!
namespace {

    float last_scroll_val;

    struct d_scroll_handler {
        std::stack<float> bw_history;
        std::stack<float> fw_history;

        ImGuiListClipper clipper;

        uint16_t target_addr;
        float    target_scroll;

        bool show_left() { return !bw_history.empty(); }
        bool show_right() { return !fw_history.empty(); }

        float fix_float(float v) {
            if (v == 0.0f) {
                return v += FLT_MIN;
            }
            else {
                return v;
            }
        }
        uint16_t fix_u16(uint16_t v) {
            if (v == 0) {
                return v + 1;
            }
            else {
                return v;
            }
        }
        // called to clear the forward stack, i.e., we've gone back, and decided
        // to go down another path of history
        void push() {

            while (!fw_history.empty())
                fw_history.pop();
            bw_history.push(fix_float(last_scroll_val));
        }

        // queue up a new scroll target. optionally destroys forward history we've kept
        void queue_scroll(uint16_t addr, bool save_to_history = false) {
            if (save_to_history) {
                push();
            }
            set_value(addr);
        }

        void set_value(float v) { target_scroll = fix_float(v); }

        void set_value(uint16_t v) { target_addr = fix_u16(v); }

        // actually do the scrolling, call this at particular spot
        void scroll_to_target() {
            if (target_addr) {
                float item_pos_y = clipper.StartPosY + clipper.ItemsHeight * (target_addr * 0.5f);
                ImGui::SetScrollFromPosY(item_pos_y - ImGui::GetWindowPos().y);
                target_addr = 0;
            }
            else if (target_scroll) {
                ImGui::SetScrollY(target_scroll);
                target_scroll = 0.0f;
            }
            //reset clipper now
            clipper = ImGuiListClipper();
        }

        void go_back() {
            // get our last back point
            float ret = bw_history.top();
            bw_history.pop();

            // push our current one
            fw_history.push(fix_float(last_scroll_val));

            // make sure its not zero
            set_value(ret);
        }

        void go_forward() {
            float ret = fw_history.top();
            fw_history.pop();
            bw_history.push(fix_float(last_scroll_val));

            set_value(ret);
        }
    };

    d_scroll_handler ds_handler;

    std::unordered_map<uint16_t, std::shared_ptr<core::basic_block>> done;

    bool is_jump_or_ret(op val) {
        static std::array<op, 11> jumps = { op::JP,    op::JP_V0, op::SE_I, op::SE_R, op::SNE_I,
                                            op::SNE_R, op::SKP,   op::SKNP, op::RET,  op::UNKNOWN };

        for (auto j : jumps) {
            if (j == val)
                return true;
        }
        return false;
    }
} // namespace

namespace GUI {

    DisassemblyView::DisassemblyView(core::EmuWrapper& e, float fs) : DbgComponent(e, fs) {}

    void DisassemblyView::draw_window() {
        static uint16_t jump;

        // widths for text in the table for centering
        float t3_width = ImGui::CalcTextSize("fff").x;
        float t4_width = ImGui::CalcTextSize("ffff").x;

        ImGui::Begin("Disassembler", &window_state, ImGuiWindowFlags_NoScrollbar);

        // disassembly view
        {

            ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY |
                                    ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
                                    ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp |
                                    ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;

            auto test = ImGui::GetContentRegionMax();

            ImVec2 outer = ImVec2(0.0f, test.y - 4.5 * font_size);

            if (ImGui::BeginTable("text_table", 5, flags, outer)) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn(
                        "PC",
                        ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHeaderWidth |
                                ImGuiTableColumnFlags_NoHeaderLabel |
                                ImGuiTableColumnFlags_NoReorder | ImGuiTableColumnFlags_NoResize,
                        font_size);
                ImGui::TableSetupColumn(
                        "BP",
                        ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHeaderWidth |
                                ImGuiTableColumnFlags_NoHeaderLabel |
                                ImGuiTableColumnFlags_NoReorder | ImGuiTableColumnFlags_NoResize,
                        font_size);
                ImGui::TableSetupColumn("Address");
                ImGui::TableSetupColumn("Value");
                ImGui::TableSetupColumn("Instruction");

                ImGui::TableHeadersRow();

                ds_handler.clipper.Begin(2048);

                ImGui::PushStyleColor(ImGuiCol_Header, helpers::color_from_bytes(80, 80, 80));

                ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, { 0.5f, 0.0f });

                while (ds_handler.clipper.Step()) {
                    for (auto i = ds_handler.clipper.DisplayStart;
                         i < ds_handler.clipper.DisplayEnd; i++) {
                        ImGui::TableNextRow();

                        auto& ins1 = found_instructions[2 * i];

                        ImGui::TableNextColumn();

                        // PC icon
                        if (emu.is_readable()) {
                            if (emu.get_PC() == ins1.address) {
                                ImGui::Image(global::icon_textures()[ARROW_RIGHT_PC],
                                             ImVec2(font_size, font_size));
                            }
                        }

                        // breakpoint icon
                        ImGui::TableNextColumn();
                        if (emu.is_breakpoint_set(ins1.address)) {
                            ImGui::Image(global::icon_textures()[BREAKPOINT],
                                         ImVec2(font_size, font_size));
                        }

                        ImGui::TableNextColumn();
                        // show address
                        helpers::center_text_known("%03X", t3_width, ins1.address);

                        // show value at this address
                        ImGui::TableNextColumn();
                        helpers::center_text_known("%04X", t4_width, ins1.opcode);

                        // show instruction
                        ImGui::TableNextColumn();
                        if (ins1.operation != op::UNKNOWN) {

                            ImGui::PushID(ins1.address);
                            ImGui::Selectable(fmt::format("{0}", ins1.mnemonic).c_str(), false,
                                              ImGuiSelectableFlags_SpanAllColumns);

                            // menu for right clicks
                            if (ImGui::BeginPopupContextItem()) {
                                // display different text depending on if bp is set
                                if (emu.is_breakpoint_set(ins1.address)) {
                                    if (ImGui::Selectable(
                                                fmt::format("Remove breakpoint at address 0x{0:03X}",
                                                            ins1.address)
                                                        .c_str())) {
                                        emu.remove_breakpoint(ins1.address);
                                        ImGui::CloseCurrentPopup();
                                    }
                                }
                                else {
                                    if (ImGui::Selectable(
                                                fmt::format("Add breakpoint at address 0x{0:03X}",
                                                            ins1.address)
                                                        .c_str())) {
                                        emu.set_breakpoint(ins1.address);
                                        ImGui::CloseCurrentPopup();
                                    }
                                }
                                // follow to jump target in disassembly
                                if (is_followable(ins1.operation)) {
                                    uint16_t imm12 = ins1.opcode & 0xFFF;
                                    if (ImGui::Selectable(
                                                fmt::format("Follow to address 0x{0:03X}", imm12)
                                                        .c_str())) {
                                        ds_handler.queue_scroll(imm12, true);
                                        ImGui::CloseCurrentPopup();
                                    }
                                }

                                ImGui::EndPopup();
                            }

                            ImGui::PopID();
                        }
                        else {
                            ImGui::Selectable(fmt::format("???????", ins1.address).c_str(), false,
                                              ImGuiSelectableFlags_SpanAllColumns |
                                                      ImGuiSelectableFlags_Disabled);
                        }
                    }
                }

                ImGui::PopStyleVar();

                ImGui::PopStyleColor();

                // debugger tells us when we've reached a PC we should scroll to
                if (emu.reached_destination()) {
                    emu.recv_destination();
                    ds_handler.queue_scroll(emu.get_PC());
                }
                // save last_scroll_val in case we scroll next frame
                last_scroll_val = ImGui::GetScrollY();
                // scroll now to any targets we might have
                ds_handler.scroll_to_target();

                ImGui::EndTable();

                if (ImGui::ImageButton(global::icon_textures()[REFRESH],
                                       ImVec2(font_size, font_size))) {
                    analyze();
                    ds_handler.queue_scroll(emu.get_entry(), true);
                }

                ImGui::SameLine();

                if (ImGui::ImageButton(global::icon_textures()[PAUSE],
                                       ImVec2(font_size, font_size))) {
                    emu.pause();
                    ds_handler.queue_scroll(emu.get_PC(), true);
                }
                ImGui::SameLine();

                if (ImGui::ImageButton(global::icon_textures()[STEP_OVER],
                                       ImVec2(font_size, font_size))) {
                    emu.step_over();
                }

                ImGui::SameLine();
                if (ImGui::ImageButton(global::icon_textures()[STEP_INTO],
                                       ImVec2(font_size, font_size))) {
                    emu.single_step();
                }

                ImGui::SameLine();
                if (ImGui::ImageButton(global::icon_textures()[STEP_OUT],
                                       ImVec2(font_size, font_size))) {
                    emu.step_out();
                }

                ImGui::SameLine();
                if (ImGui::ImageButton(global::icon_textures()[CONTINUE],
                                       ImVec2(font_size, font_size))) {
                    emu.continue_emu();
                }

                ImGui::SameLine();

                ImGui::SetNextItemWidth(3 * font_size);
                if (ImGui::InputScalar("", ImGuiDataType_U16, &jump, nullptr, nullptr, "%03X",
                                       ImGuiInputTextFlags_EnterReturnsTrue)) {
                    ds_handler.queue_scroll(jump);
                    jump = 0;
                }

                if (ds_handler.show_left()) {
                    if (ImGui::ImageButton(global::icon_textures()[ARROW_LEFT],
                                           ImVec2(font_size, font_size))) {
                        ds_handler.go_back();
                    }
                }
                else {
                    ImGui::ImageButton(global::icon_textures()[ARROW_LEFT_INACTIVE],
                                       ImVec2(font_size, font_size));
                }
                ImGui::SameLine();
                if (ds_handler.show_right()) {
                    if (ImGui::ImageButton(global::icon_textures()[ARROW_RIGHT],
                                           ImVec2(font_size, font_size))) {
                        ds_handler.go_forward();
                    }
                }
                else {
                    ImGui::ImageButton(global::icon_textures()[ARROW_RIGHT_INACTIVE],
                                       ImVec2(font_size, font_size));
                }
            }
        }

        ImGui::End();
    }

    std::shared_ptr<core::basic_block> DisassemblyView::rec_cfg(uint16_t start_address,
                                                                uint16_t from_address) {
        // see if we have already processed this address
        auto found = done.find(start_address);

        // if we havent seen this instruction yet, then we're ok to start a new basic block
        if (found == done.end()) {
            std::shared_ptr curr = std::make_shared<core::basic_block>();

            control_flow_graph.push_back(curr);

            /*
            when analyzing a new basic block, it is clear that it will at least have a point
            where it came from, unless it is the entry/call
        */
            if (from_address != 0) {
                curr->add_reference(from_address, start_address);
            }
            // start adding instructions to this basic block
            auto current_address = start_address;
            auto current_opcode  = emu.fetch(current_address);

            // current instruction isnt a jump, add it to list for this block
            while (!is_jump_or_ret(decode(current_opcode))) {

                // make sure we arent overrunning into a previous block
                auto check = done.try_emplace(current_address, curr);
                if (check.second) {

                    core::Instruction ins(current_address, current_opcode);
                    /* 
                    add this current address and its associated basic block to our map
                    in case in the future, we find a basic block that jumps to here
                */
                    done[current_address] = curr;

                    // check to see if it is a call (no branch), process it
                    if (decode(current_opcode) == op::CALL || decode(current_opcode) == op::SYS) {

                        auto call_address = current_opcode & 0xFFF;
                        rec_cfg(call_address, current_address);
                    }

                    // go to next instruction
                    current_address += ins.length;
                    curr->append(std::move(ins));
                    current_opcode = emu.fetch(current_address);
                }
                else {
                    // we add a reference and end our block
                    auto other = done.at(current_address);

                    curr->to_block_true = other;
                    // FIXME figure out this and below
                    curr->end_address = current_address - 2;

                    other->add_reference(curr->end_address, other->start_address);

                    return curr;
                }
            }

            // now current_opcode is a jump, add it manually

            curr->append(current_address, current_opcode);
            done[current_address] = curr;

            // this is the end for this block
            curr->end_address = current_address;

            switch (decode(current_opcode)) {
            // for unconditional jump, we just process location
            case op::JP: {
                auto jump_address = current_opcode & 0xFFF;

                auto result = rec_cfg(jump_address, current_address);

                done[current_address]->to_block_true = result;
                return done[current_address];
            }

            case op::SE_I:
            case op::SE_R:
            case op::SNE_I:
            case op::SNE_R:
            case op::SKP:
            case op::SKNP: {
                // do in this order, in case false case is not a jump. that way,
                // we split the block

                auto false_result = rec_cfg(current_address + 2, current_address);
                done[current_address]->to_block_false = false_result;
                auto true_result = rec_cfg(current_address + 4, current_address);
                done[current_address]->to_block_true = true_result;

                return done[current_address];
            }

            // indirect jump and unknown opcodes kinda just screw it up..
            case op::UNKNOWN:
            case op::JP_V0:
            case op::RET:
            // nothing should ever hit default case, its just to make compiler happy, since
            // decode will return op::UNKNOWN in any case
            default: {
                return done[current_address];
            }
            }
        }
        // otherwise, we're jumping into a basic block already processed
        else {
            auto orig_block = done.at(start_address);

            // always true in this case
            auto from_block = done.at(from_address);

            // if we're jumping to start of block, just add a reference from_address to start_address
            if (orig_block->start_address == start_address) {
                orig_block->add_reference(from_address, start_address);
                return orig_block;
            }
            else {

                /* 
                otherwise, we must split the block. consider a block like
                
                orig_block
                    INS 1 = orig_block.start_address
                    INS 2
                    INS 3
                    INS 4
                    CONDITIONAL = orig_block.end_address

                suppose our start_address points to INS 4. then, we want to split like

                new_block
                    INS 1 = new_block.start_address
                    INS 2
                    INS 3 = new_block.end_address
                    to_block_true -> orig_block
                    from_blocks = original orig_block.from_blocks
                
                orig_block
                    INS 4 = orig_block.start_address
                    CONDITIONAL = orig_block.end_address
                    from_blocks contains new_block
                    to_block_* are left in original state

                return new_block

                
            */
                auto new_block = orig_block->split(start_address, orig_block);

                control_flow_graph.push_back(new_block);

                // make sure everything in new block points to itself in map
                for (auto& i : new_block->instructions) {
                    done[i.address] = new_block;
                }

                return orig_block;
            }
        }
    }
    /*
    still implementing, in particular calls will ruin things
*/
    void DisassemblyView::analyze() {

        if (!emu.is_ready())
            return;

        control_flow_graph.clear();
        done.clear();

        rec_cfg(emu.get_entry(), 0);

        std::sort(control_flow_graph.begin(), control_flow_graph.end(),
                  [](std::shared_ptr<core::basic_block> lhs,
                     std::shared_ptr<core::basic_block> rhs) {
                      return (lhs->start_address < rhs->start_address);
                  });

        for (auto bb : control_flow_graph) {
            for (auto& i : bb->instructions) {
                found_instructions[i.address] = i;
            }
        }
    }

    void DisassemblyView::process_dbg_msg(DbgMessage msg) {
        if (msg.act == dbg_action::scroll) {
            auto m = std::any_cast<ScrollMessage>(msg.data);
            ds_handler.queue_scroll(m.target_address, m.save_history);
        }
    }

} // namespace GUI