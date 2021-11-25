#include "gui/launcher.hpp"
#include "gui/imgui_helpers.hpp"

namespace GUI {

    Launcher::Launcher(float fs, core::EmuWrapper& e) : GUIComponent(fs), emu(e) {}

    void Launcher::draw_window() {
        static uint16_t entry_setting = 0x100;
        static uint16_t base_address  = 0x0;

        static bool launch_paused = false;

        static ImGuiWindowFlags launch_window_settings =
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking;

        static std::string last_file_name;

        filebrowser.Display();

        if (filebrowser.HasSelected()) {
            last_file_name = filebrowser.GetSelected().string();
            filebrowser.ClearSelected();
        }

        ImGui::Begin("Launch settings", &window_state, launch_window_settings);

        if (ImGui::Button("Choose file")) {
            filebrowser.Open();
        }

        ImGui::TextWrapped("file chosen: %s", last_file_name.c_str());

        ImGui::Separator();
        ImGui::SetNextItemWidth(150.0F);

        ImGui::InputScalar(
                "entry point (hex)", ImGuiDataType_U16, &entry_setting, nullptr, nullptr, "%03x",
                ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_AutoSelectAll);
        ImGui::SetNextItemWidth(150.0F);

        ImGui::InputScalar(
                "base address (hex)", ImGuiDataType_U16, &base_address, nullptr, nullptr, "%03x",
                ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_AutoSelectAll);

        ImGui::Separator();

        ImGui::Checkbox("launch paused", &launch_paused);

        if (helpers::center_button("OK")) {
            // new game
            emu.new_game(last_file_name, entry_setting, base_address, launch_paused);

            if (launch_paused) {
                emu.pause();

                message = GUIMessage(gui_component::all, gui_action::new_game);
            }

            window_state = false;
        }

        ImGui::End();
    }

} // namespace GUI