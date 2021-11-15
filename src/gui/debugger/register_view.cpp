#include "gui/debugger/register_view.hpp"
#include "gui/imgui_helpers.hpp"
#include <fmt/format.h>

namespace GUI {

    RegisterView::RegisterView(float fs, core::EmuWrapper& e) : DbgComponent(fs, e) {}

    void RegisterView::draw_window() {
        static const float width = ImGui::CalcTextSize("F").x;

        static const float pair_width = 4.5 * width;

        auto draw_I = [&]() {
            ImGui::TableNextColumn();
            helpers::center_text("I");

            if (emu.is_readable()) {
                ImGui::SameLine();
                ImGui::Selectable("###I", false, ImGuiSelectableFlags_SpanAllColumns);

                if (ImGui::BeginPopupContextItem()) {
                    uint16_t value = emu.get_I() & 0xFFF;
                    if (ImGui::Selectable(
                                fmt::format("View {0:03X} in disassembly", value).c_str())) {
                        message = GUIMessage{ gui_component::disassembly_view, gui_action::scroll,
                                              ScrollMessage{ value, true } };
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::Selectable(fmt::format("View {0:03X} in memory", value).c_str())) {
                        message = GUIMessage{ gui_component::memory_view, gui_action::scroll,
                                              ScrollMessage{ value } };
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                ImGui::TableNextColumn();

                helpers::colored_centered_text({ 255, 0, 0, 255 }, emu.I_change,
                                               fmt ::format("{:03X}", emu.get_I()).c_str());
            }
            else {
                ImGui::TableNextColumn();
                // fix this
                helpers::disabled_centered_text("???");
            }
        };

        auto draw_PC = [&]() {
            ImGui::TableNextColumn();
            helpers::center_text("PC");

            if (emu.is_readable()) {
                ImGui::SameLine();
                ImGui::Selectable("###PC", false, ImGuiSelectableFlags_SpanAllColumns);

                if (ImGui::BeginPopupContextItem()) {
                    uint16_t value = emu.get_PC() & 0xFFF;
                    if (ImGui::Selectable(
                                fmt::format("View {0:03X} in disassembly", value).c_str())) {
                        message = GUIMessage{ gui_component::disassembly_view, gui_action::scroll,
                                              ScrollMessage{ value } };
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                ImGui::TableNextColumn();
                helpers::colored_centered_text({ 255, 0, 0, 255 }, emu.PC_change,
                                               fmt::format("{:03X}", emu.get_PC()).c_str());
            }
            else {
                ImGui::TableNextColumn();
                helpers::disabled_centered_text("???");
            }
        };

        auto draw_D = [&]() {
            ImGui::TableNextColumn();
            helpers::center_text("D");
            ImGui::TableNextColumn();
            if (emu.is_readable()) {
                helpers::colored_centered_text({ 255, 0, 0, 255 }, emu.dt_change,
                                               fmt::format("{:02X}", emu.get_DT()).c_str());
            }
            else {
                helpers::disabled_centered_text("??");
            }
        };

        auto draw_S = [&]() {
            ImGui::TableNextColumn();
            helpers::center_text("S");
            ImGui::TableNextColumn();

            if (emu.is_readable()) {
                helpers::colored_centered_text({ 255, 0, 0, 255 }, emu.st_change,
                                               fmt::format("{:02X}", emu.get_ST()).c_str());
            }
            else {
                helpers::disabled_centered_text("??");
            }
        };

        ImGui::SetNextWindowSize({ 300, 300 }, ImGuiCond_FirstUseEver);

        // we check one time to calculate how much space we will need for our pair of register/values
        ImGui::Begin("Registers", &window_state);

        // calculate how many pairs we can draw horizontally
        auto avail = ImGui::GetContentRegionAvail();
        int  count = static_cast<int>(std::floor(avail.x / pair_width));

        if (count == 0) {
            count = 1;
        }

        // draw outer table
        if (ImGui::BeginTable("allregisters", count,
                              ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingStretchSame |
                                      ImGuiTableFlags_Borders | ImGuiTableFlags_NoPadInnerX)) {
            for (auto index = 0; index < 16; ++index) {
                ImGui::TableNextColumn();

                helpers::center_cursor(pair_width);
                // draw each pair
                if (ImGui::BeginTable(
                            "pair", 2,
                            ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_NoHostExtendX |
                                    ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX,
                            { pair_width, 0.0f })) {
                    ImGui::TableNextColumn();
                    helpers::center_text(fmt::format("V{:01x}", index).c_str());
                    ImGui::TableNextColumn();

                    if (emu.is_readable()) {
                        helpers::colored_centered_text(
                                { 255, 0, 0, 255 }, emu.reg_changes[index],
                                fmt::format("{:02X}", emu.get_V(index)).c_str());
                    }
                    else {
                        helpers::disabled_centered_text("??");
                    }

                    ImGui::EndTable();
                }
            }
            ImGui::EndTable();
        }
        // three cases, we draw vertically, horizontally, or in a table if we're square
        // vertical case
        if (count == 1) {
            if (ImGui::BeginTable("special registers", 2,
                                  ImGuiTableFlags_NoHostExtendX |
                                          ImGuiTableFlags_SizingStretchSame |
                                          ImGuiTableFlags_Borders | ImGuiTableFlags_NoPadInnerX)) {
                draw_I();
                draw_PC();
                draw_D();
                draw_S();
                ImGui::EndTable();
            }
        }
        // horizontal case
        else if (count > 7) {
            if (ImGui::BeginTable("special registers", 8,
                                  ImGuiTableFlags_NoHostExtendX |
                                          ImGuiTableFlags_SizingStretchSame |
                                          ImGuiTableFlags_Borders | ImGuiTableFlags_NoPadInnerX)) {
                draw_I();
                draw_PC();
                draw_D();
                draw_S();
                ImGui::EndTable();
            }
        }
        // square
        else {
            if (ImGui::BeginTable("special registers", 2,
                                  ImGuiTableFlags_NoHostExtendX |
                                          ImGuiTableFlags_SizingStretchSame |
                                          ImGuiTableFlags_Borders | ImGuiTableFlags_NoPadInnerX)) {
                ImGui::TableNextColumn();
                helpers::center_text("Special");
                ImGui::TableNextColumn();
                helpers::center_text("Timers");

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                if (ImGui::BeginTable(
                            "Special", 2,
                            ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX |
                                    ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX)) {

                    draw_I();

                    draw_PC();

                    ImGui::EndTable();
                }

                ImGui::TableNextColumn();

                if (ImGui::BeginTable(
                            "Timers", 2,
                            ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX |
                                    ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX)) {

                    draw_D();
                    draw_S();

                    ImGui::EndTable();
                }

                ImGui::EndTable();
            }
        }

        ImGui::End();
    }

    // dont do shit at all bruh

} // namespace GUI