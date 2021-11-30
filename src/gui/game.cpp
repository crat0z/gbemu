#include "gui/game.hpp"
#include "gui/imgui_helpers.hpp"
#include "global.hpp"

namespace GUI {

    Game::Game(core::EmuWrapper& e) : GUIComponent(0, true), emu(e) { emu.reset(); }

    void Game::draw_window() {

        /* ImU32 white = ImColor(global::white_vec());
        ImU32 black = ImColor(global::black_vec());

        // vars
        ImGui::PushStyleColor(ImGuiCol_ChildBg, global::black_vec());

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGuiWindowFlags win_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav |
                                     ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse |
                                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration |
                                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoInputs |
                                     ImGuiWindowFlags_NoBringToFrontOnFocus;

        auto vp = ImGui::GetMainViewport();

        //FIX!!!!!!!!!!!
        ImGui::SetNextWindowDockID(global::dock_id());

        ImGui::SetNextWindowPos(vp->WorkPos, ImGuiCond_Once);
        ImGui::SetNextWindowSize(vp->WorkSize, ImGuiCond_Once);

        ImGui::Begin("game window", nullptr, win_flags);

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        ImVec2 size = { vMax.x - vMin.x, vMax.y - vMin.y };

        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;

        float scale;
        float x_offset;
        float y_offset;

        if (size.x / size.y >= 2.0f) {
            scale    = size.y / Y_PIXELS;
            x_offset = (size.x - scale * X_PIXELS) / 2.0f;
            y_offset = 0.0f;
        }
        else {
            scale    = size.x / X_PIXELS;
            x_offset = 0.0f;
            y_offset = (size.y - scale * Y_PIXELS) / 2.0f;
        }

        vMin.x += x_offset;
        vMin.y += y_offset;

        for (auto i = 0; i < Y_PIXELS; ++i) {
            auto y = vMin.y + i * scale;
            for (auto j = 0; j < X_PIXELS; ++j) {
                auto x = vMin.x + j * scale;
                if (emu.frame_buffer()[i][j]) {
                    draw_list->AddRectFilled(ImVec2{ x, y }, ImVec2{ x + scale, y + scale }, white,
                                             0.0f, ImDrawFlags_RoundCornersNone);
                }
                else {
                    draw_list->AddRectFilled(ImVec2{ x, y }, ImVec2{ x + scale, y + scale }, black,
                                             0.0f, ImDrawFlags_RoundCornersNone);
                }
            }
        }

        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(); */
    }

} // namespace GUI