#include "gui/debugger/stack_view.hpp"
#include "gui/imgui_helpers.hpp"
#include <fmt/format.h>

namespace GUI {

    StackView::StackView(float fs, core::EmuWrapper& e) : DbgComponent(fs, e) {}

    void StackView::draw_window() {

        ImGui::SetNextWindowSize({ 300, 200 }, ImGuiCond_FirstUseEver);

        ImGui::Begin("Stack view", &window_state);
        {
            ImGui::BeginChild("stacks");
            /* if (emu.is_readable()) {

                for (auto i = 15; i >= 0; --i) {

                    if (static_cast<size_t>(i) < stack.size()) {
                        helpers::center_text(fmt::format("{0:03X}", stack[i]).c_str());
                    }
                    else {
                        helpers::disabled_centered_text("???");
                    }
                }
            }
            else {
                for (auto i = 15; i >= 0; --i) {
                    helpers::disabled_centered_text("???");
                }
            }
 */
            ImGui::EndChild();
        }
        ImGui::End();
    }

} // namespace GUI