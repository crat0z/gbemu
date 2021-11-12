#include "gui/debugger/memory_view.hpp"
#include "gui/imgui_helpers.hpp"
#include <cmath>
#include <fmt/format.h>

namespace GUI {

    MemoryView::MemoryView(core::EmuWrapper& e, float fs) : DbgComponent(e, fs) {}

    void MemoryView::draw_window() {
        static const float width      = ImGui::CalcTextSize("F").x;
        static const float base_width = ImGui::CalcTextSize("FFF").x * 1.35f;

        auto byte_to_printable = [&](char s) {
            if (s >= 0x20 && s <= 0x7E) {
                return s;
            }
            return '.';
        };

        ImGui::Begin("Memory viewer");
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

            helpers::center_text("Memory viewer");

            if (ImGui::BeginTable("memory", count,
                                  ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp |
                                          ImGuiTableFlags_NoHostExtendX |
                                          ImGuiTableFlags_NoHostExtendY)) {
                auto cols     = count - 3;
                auto cols_p1s = 1;
                auto cols_p2e = count - 1;

                auto b = (cols_p1s + cols_p2e) / 2;

                auto cols_p1e = b;
                auto cols_p2s = cols_p2e - b + 1;

                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Base",
                                        ImGuiTableColumnFlags_NoHeaderLabel |
                                                ImGuiTableColumnFlags_WidthFixed,
                                        5 * width);

                // first set
                for (auto i = cols_p1s; i < cols_p1e; ++i) {
                    ImGui::TableSetupColumn(fmt::format("{0:01X}", i - 1).c_str(),
                                            ImGuiTableColumnFlags_NoResize |
                                                    ImGuiTableColumnFlags_WidthFixed);
                }

                // center
                ImGui::TableSetupColumn("###break", ImGuiTableColumnFlags_WidthStretch);

                // second set
                for (auto i = cols_p2s; i < cols_p2e; ++i) {
                    ImGui::TableSetupColumn(fmt::format("{0:01X}", i - 2).c_str(),
                                            ImGuiTableColumnFlags_NoResize |
                                                    ImGuiTableColumnFlags_WidthFixed);
                }

                ImGui::TableSetupColumn("ASCII", ImGuiTableColumnFlags_NoHeaderLabel |
                                                         ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                ImGuiListClipper clipper;
                clipper.Begin(4096 / cols);

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

                        // draw rows
                        // do first half
                        auto real_index = 0;
                        for (auto j = cols_p1s; j < cols_p1e; ++j) {
                            ImGui::TableNextColumn();
                            auto address = base + real_index;
                            auto val     = emu.get_memory()[base + real_index];

                            context_menu(address, val);

                            var += byte_to_printable(static_cast<char>(val));
                            real_index++;
                        }

                        // do break
                        ImGui::TableNextColumn();
                        ImGui::Text(" ");

                        //do second half
                        for (auto j = cols_p2s; j < cols_p2e; ++j) {
                            ImGui::TableNextColumn();
                            auto address = base + real_index;
                            auto val     = emu.get_memory()[base + real_index];

                            context_menu(address, val);

                            var += byte_to_printable(static_cast<char>(val));
                            real_index++;
                        }

                        ImGui::TableNextColumn();
                        helpers::center_text(var.c_str());
                        var.clear();
                    }
                }

                ImGui::EndTable();
            };

            helpers::center_button("OK");
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