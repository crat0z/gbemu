#include "gui/debugger/memory_view.hpp"
#include "gui/imgui_helpers.hpp"
#include <cmath>
#include <fmt/format.h>

namespace GUI {

    MemoryView::MemoryView(core::EmuWrapper& e, float fs) : DbgComponent(e, fs) {}

    void MemoryView::draw_window() {
        static const float width      = ImGui::CalcTextSize("F").x;
        static const float base_width = ImGui::CalcTextSize("FFF").x * 1.35f;

        static uint16_t jump = 0;

        auto byte_to_printable = [&](char s) {
            if (s >= 0x20 && s <= 0x7E) {
                return s;
            }
            return '.';
        };

        ImGui::SetNextWindowSize({ 430, 500 }, ImGuiCond_FirstUseEver);

        ImGui::Begin("Memory viewer", &window_state, ImGuiWindowFlags_NoScrollbar);
        {
            auto avail = ImGui::GetContentRegionAvail();
            int  count = static_cast<int>(std::floor(avail.x / base_width)) + 1;

            // only 3 modes rlly
            if (count >= 19) {
                count = 19;
            }
            else if (count >= 11) {
                count = 11;
            }
            else {
                count = 7;
            }

            auto max = ImGui::GetContentRegionMax();

            ImVec2 outer = ImVec2(0.0f, max.y - 3.0 * font_size);

            if (ImGui::BeginTable("memory", count,
                                  ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp |
                                          ImGuiTableFlags_NoHostExtendX |
                                          ImGuiTableFlags_NoHostExtendY,
                                  outer)) {

                auto begin = 1;
                auto end   = count - 1;

                auto cols = count - 3;

                auto brk = (count - 3) / 2 + 1;

                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Base",
                                        ImGuiTableColumnFlags_NoHeaderLabel |
                                                ImGuiTableColumnFlags_WidthFixed,
                                        5 * width);

                auto real_count = 0;
                for (auto i = begin; i < end; ++i) {
                    if (i == brk) {
                        ImGui::TableSetupColumn("###break", ImGuiTableColumnFlags_WidthStretch);
                    }
                    else {
                        ImGui::TableSetupColumn(fmt::format("{0:01X}", real_count).c_str(),
                                                ImGuiTableColumnFlags_NoResize |
                                                        ImGuiTableColumnFlags_WidthFixed);
                        real_count++;
                    }
                }

                ImGui::TableSetupColumn("ASCII", ImGuiTableColumnFlags_NoHeaderLabel |
                                                         ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                ImGuiListClipper clipper;
                clipper.Begin(MAX_MEMORY / cols);

                std::string var;
                var.reserve(cols);

                while (clipper.Step()) {
                    for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                        // draw left number base thing
                        uint16_t base = i * cols;
                        ImGui::TableNextColumn();

                        ImGui::Text("%03X", base);

                        auto context_menu = [&](uint16_t addr, uint8_t v) {
                            ImGui::PushID(addr);
                            ImGui::Selectable(fmt::format("{0:02X}", v).c_str());

                            if (ImGui::BeginPopupContextItem()) {
                                if (ImGui::Selectable(
                                            fmt::format("View address {0:03X} in disassembly", addr)
                                                    .c_str())) {
                                    message =
                                            DbgMessage{ dbg_component::disassembly_view,
                                                        dbg_action::scroll, ScrollMessage{ addr } };
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }

                            ImGui::PopID();
                        };

                        auto real_index = 0;
                        for (auto i = begin; i < end; ++i) {
                            ImGui::TableNextColumn();
                            if (i == brk) {
                                ImGui::Text(" ");
                            }
                            else {
                                auto address = base + real_index;
                                auto val     = emu.get_memory()[base + real_index];

                                context_menu(address, val);

                                var += byte_to_printable(static_cast<char>(val));
                                real_index++;
                            }
                        }

                        ImGui::TableNextColumn();
                        helpers::center_text(var.c_str());
                        var.clear();
                    }
                }

                if (next_scroll) {
                    float item_pos_y =
                            clipper.StartPosY + clipper.ItemsHeight * (next_scroll / cols);
                    ImGui::SetScrollFromPosY(item_pos_y - ImGui::GetWindowPos().y);
                    next_scroll = 0;
                }

                ImGui::EndTable();
            };

            ImGui::SetNextItemWidth(5 * width);
            if (ImGui::InputScalar("###jump to", ImGuiDataType_U16, &jump, nullptr, nullptr, "%03X",
                                   ImGuiInputTextFlags_EnterReturnsTrue)) {
                next_scroll = jump;

                jump = 0;
            }
        }
        ImGui::End();
    }

    void MemoryView::process_dbg_msg(DbgMessage msg) {
        if (msg.act == dbg_action::scroll) {
            auto m      = std::any_cast<ScrollMessage>(msg.data);
            next_scroll = m.target_address;
        }
    }

} // namespace GUI