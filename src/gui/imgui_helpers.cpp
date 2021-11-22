#include "gui/imgui_helpers.hpp"
#include <imgui.h>

namespace GUI {

    namespace helpers {

        void center_cursor(float item_size) {
            auto width = ImGui::GetContentRegionAvail().x;
            auto x     = ImGui::GetCursorPosX();

            ImGui::SetCursorPosX(x + ((width - item_size) / 2.0F));
        }

        bool center_button(const char* label, const ImVec2& size) {
            auto width = ImGui::CalcTextSize(label);
            center_cursor(width.x);
            return ImGui::Button(label, size);
        }

        // centers a string
        void center_text(const char* format) {
            center_cursor(ImGui::CalcTextSize(format).x);

            ImGui::Text("%s", format);
        }

        void colored_centered_text(const ImVec4& colour, bool cond, const char* format) {

            center_cursor(ImGui::CalcTextSize(format).x);

            if (cond) {
                ImGui::TextColored(colour, "%s", format);
            }
            else {
                ImGui::Text("%s", format);
            }
        }

        void colored_text(const ImVec4& colour, bool cond, const char* format) {

            if (cond) {
                ImGui::TextColored(colour, "%s", format);
            }
            else {
                ImGui::Text("%s", format);
            }
        }

        void disabled_centered_text(const char* format) {
            auto& style = ImGui::GetStyle();
            colored_centered_text(style.Colors[ImGuiCol_TextDisabled], true, format);
        }

        ImVec4 color_from_bytes(uint8_t r, uint8_t g, uint8_t b) {
            return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
        }
    } // namespace helpers

} // namespace GUI