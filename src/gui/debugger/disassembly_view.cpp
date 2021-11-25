#include "gui/debugger/disassembly_view.hpp"
#include "gui/imgui_helpers.hpp"
#include <fmt/format.h>
#include <stack>
#include "gui/icons.hpp"
#include "global.hpp"

namespace {

    std::unordered_map<uint16_t, std::shared_ptr<core::basic_block>> done;

    // check if `val` is an opcode that breaks a basic block
    bool is_jump_or_ret(op val) {
        static std::array<op, 11> jumps = { op::JP,    op::JP_V0, op::SE_I, op::SE_R, op::SNE_I,
                                            op::SNE_R, op::SKP,   op::SKNP, op::RET,  op::UNKNOWN };

        for (auto j : jumps) {
            if (j == val) {
                return true;
            }
        }
        return false;
    }
} // namespace

namespace GUI {

    DisassemblyView::DisassemblyView(float fs, core::EmuWrapper& e)
            : DbgComponent(fs, e), target_addr{ 0 } {
        found_instructions.reserve(8192);

        for (auto i = 0; i < 8192; ++i) {
            found_instructions.emplace_back(i);
        }

        if (e.is_ready()) {
            first_analysis();
        }
    }

    void DisassemblyView::draw_window() {
        static uint16_t jump;

        // widths for text in the table for centering
        static float width = ImGui::CalcTextSize("F").x;

        ImGui::SetNextWindowSize({ 400, 500 }, ImGuiCond_FirstUseEver);

        ImGui::Begin("Disassembler", &window_state, ImGuiWindowFlags_NoScrollbar);

        // disassembly view
        {

            ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY |
                                    ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
                                    ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp |
                                    ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;

            auto max = ImGui::GetContentRegionMax();

            ImVec2 outer = ImVec2(0.0f, max.y - 3.0 * font_size);

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

                clipper.Begin(found_instructions.size());

                while (clipper.Step()) {
                    for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                        ImGui::TableNextRow();

                        auto& ins1 = found_instructions[i];

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
                        helpers::center_text(fmt::format("{:03X}", ins1.address).c_str());

                        // show value at this address
                        ImGui::TableNextColumn();
                        helpers::center_text(ins1.opcode_string().c_str());

                        // show instruction
                        ImGui::TableNextColumn();
                        if (ins1.operation != op::UNKNOWN) {

                            ImGui::PushID(ins1.address);

                            helpers::center_text(fmt::format("{0}", ins1.mnemonic).c_str());
                            ImGui::SameLine();
                            ImGui::Selectable("###dis", false, ImGuiSelectableFlags_SpanAllColumns);

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
                                        queue_scroll(imm12, true);
                                        ImGui::CloseCurrentPopup();
                                    }
                                }

                                ImGui::EndPopup();
                            }

                            ImGui::PopID();
                        }
                        else {
                            helpers::disabled_centered_text("???????");
                        }
                    }
                }

                // debugger tells us when we've reached a PC we should scroll to
                if (emu.reached_destination()) {
                    emu.recv_destination();
                    queue_scroll(emu.get_PC());
                }
                // save last_scroll_val in case we scroll next frame
                last_scroll_val = ImGui::GetScrollY();
                // scroll now to any targets we might have
                scroll_to_target();

                ImGui::EndTable();
            }

            // go back
            if (show_left()) {
                if (ImGui::ImageButton(global::icon_textures()[ARROW_LEFT],
                                       ImVec2(font_size, font_size))) {
                    go_back();
                }
            }
            else {
                ImGui::ImageButton(global::icon_textures()[ARROW_LEFT_INACTIVE],
                                   ImVec2(font_size, font_size));
            }
            ImGui::SameLine();
            // go forward
            if (show_right()) {
                if (ImGui::ImageButton(global::icon_textures()[ARROW_RIGHT],
                                       ImVec2(font_size, font_size))) {
                    go_forward();
                }
            }
            else {
                ImGui::ImageButton(global::icon_textures()[ARROW_RIGHT_INACTIVE],
                                   ImVec2(font_size, font_size));
            }
            ImGui::SameLine();
            // jump to address
            ImGui::SetNextItemWidth(5 * width);
            if (ImGui::InputScalar("###jump to", ImGuiDataType_U16, &jump, nullptr, nullptr, "%03X",
                                   ImGuiInputTextFlags_EnterReturnsTrue |
                                           ImGuiInputTextFlags_CharsHexadecimal)) {
                queue_scroll(jump);
                jump = 0;
            }
            // below not really needed anymore maybe remove
            /* ImGui::SameLine();
            if (ImGui::ImageButton(global::icon_textures()[REFRESH],
                                   ImVec2(font_size, font_size))) {
                first_analysis();
                queue_scroll(emu.get_entry(), true);
            } */

            ImGui::SameLine();

            if (ImGui::ImageButton(global::icon_textures()[PAUSE], ImVec2(font_size, font_size))) {
                emu.pause();
                queue_scroll(emu.get_PC(), true);
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

    // statically analyze the rom from entry and find all branches it can
    // note that indirect jumps or calls cannot be found in this manner
    void DisassemblyView::first_analysis() {

        if (!emu.is_ready())
            return;

        control_flow_graph.clear();
        done.clear();

        // FIX, not sure if this is entry of gb atm
        rec_cfg(0x100, 0);

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

        // remove invalid instructions in list
        std::vector<core::Instruction> newlist;
        newlist.reserve(8192);

        for (size_t i = 0; i < found_instructions.size();) {
            auto inc = found_instructions[i].length;
            newlist.emplace_back(std::move(found_instructions[i]));
            i += inc;
        }

        newlist.shrink_to_fit();

        found_instructions = std::move(newlist);
    }

    bool DisassemblyView::show_left() { return !bw_history.empty(); }
    bool DisassemblyView::show_right() { return !fw_history.empty(); }

    float DisassemblyView::fix_float(float v) {
        if (v == 0.0F) {
            return v += FLT_MIN;
        }
        return v;
    }
    uint16_t DisassemblyView::fix_u16(uint16_t v) {
        if (v == 0) {
            return v + 1;
        }
        return v;
    }
    // called to clear the forward stack, i.e., we've gone back, and decided
    // to go down another path of history
    void DisassemblyView::push() {

        while (!fw_history.empty())
            fw_history.pop();
        bw_history.push(fix_float(last_scroll_val));
    }

    // queue up a new scroll target. optionally destroys forward history we've kept
    void DisassemblyView::queue_scroll(uint16_t addr, bool save_to_history) {
        assert(addr <= 0xFFF);

        if (save_to_history) {
            push();
        }

        // return -1 if we're lower, 1 if we're higher, 0 if this is the instruction
        auto hit = [&](uint16_t a, const core::Instruction& ins) {
            if (a < ins.address) {
                return -1;
            }
            else {

                for (auto i = 0; i < ins.length; ++i) {
                    if (a == (ins.address + i)) {
                        return 0;
                    }
                }
                return 1;
            }
        };
        // do binary search on found_instructions array to find which index addr is,
        // pass valid first and last index
        auto find_position = [&](const std::vector<core::Instruction>& v, int begin, int end) {
            const auto find_position_impl = [&](const std::vector<core::Instruction>& v, int begin,
                                                int end, const auto& impl) -> uint16_t {
                auto center_index = (begin + end) / 2;

                const auto& ref = v[center_index];

                auto result = hit(addr, ref);

                switch (result) {
                case 1:
                    return impl(v, center_index + 1, end, impl);
                case 0:
                    return center_index;
                case -1:
                    return impl(v, begin, center_index - 1, impl);
                default: // should never hit here...
                    return 0;
                }
            };
            return find_position_impl(v, begin, end, find_position_impl);
        };

        set_value(find_position(found_instructions, 0, found_instructions.size() - 1));
    }

    void DisassemblyView::set_value(float v) { target_scroll = fix_float(v); }

    void DisassemblyView::set_value(uint16_t v) { target_addr = fix_u16(v); }

    // actually do the scrolling, call this at particular spot
    void DisassemblyView::scroll_to_target() {
        if (target_addr) {
            float item_pos_y = clipper.StartPosY + clipper.ItemsHeight * (target_addr);
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

    void DisassemblyView::go_back() {
        // get our last back point
        float ret = bw_history.top();
        bw_history.pop();

        // push our current one
        fw_history.push(fix_float(last_scroll_val));

        // make sure its not zero
        set_value(ret);
    }

    void DisassemblyView::go_forward() {
        float ret = fw_history.top();
        fw_history.pop();
        bw_history.push(fix_float(last_scroll_val));

        set_value(ret);
    }

    void DisassemblyView::process_message(GUIMessage msg) {
        if (msg.act == gui_action::scroll) {
            auto m = std::any_cast<ScrollMessage>(msg.data);
            queue_scroll(m.target_address, m.save_history);
        }
        else if (msg.act == gui_action::new_game) {
            first_analysis();
            // FIX, dont know if this is entry on gb atm
            queue_scroll(0x100, true);
        }
    }

} // namespace GUI