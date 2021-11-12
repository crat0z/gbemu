#include "gui/imgui_helpers.hpp"
#include <imgui.h>

namespace GUI {

    namespace helpers {

        void center_cursor(float item_size) {
            auto width = ImGui::GetContentRegionAvail().x;
            auto x     = ImGui::GetCursorPosX();

            ImGui::SetCursorPosX(x + ((width - item_size) / 2.0f));
        }

        void right_align_cursor(float item_size, float offset) {
            auto  width = ImGui::GetContentRegionAvail().x;
            auto  x     = ImGui::GetCursorPosX();
            auto& style = ImGui::GetStyle();
            ImGui::SetCursorPosX(x + (width - item_size - offset));
        }

        void right_align_text(const char* format, float offset) {

            right_align_cursor(ImGui::CalcTextSize(format).x, offset);
            ImGui::Text(format);
        }

        // centers text horizontally, if it knows the size.
        void center_text_known(const char* format, float size, ...) {

            center_cursor(size);

            va_list args;
            va_start(args, size);
            ImGui::TextV(format, args);
            va_end(args);
        }

        bool center_button(const char* label) {
            auto width = ImGui::CalcTextSize(label);
            center_cursor(width.x);
            return ImGui::Button(label);
        }

        // centers a string
        void center_text(const char* format) {
            center_cursor(ImGui::CalcTextSize(format).x);

            ImGui::Text(format);
        }

        void colored_centered_text_known(const ImVec4& colour, bool cond, float size,
                                         const char* format, ...) {
            va_list args;
            va_start(args, format);

            center_cursor(size);

            if (cond) {
                ImGui::TextColoredV(colour, format, args);
            }
            else {
                ImGui::TextV(format, args);
            }
            va_end(args);
        }

        void colored_text(const ImVec4& colour, bool cond, const char* format, ...) {

            va_list args;
            va_start(args, format);
            if (cond) {
                ImGui::TextColoredV(colour, format, args);
            }
            else {
                ImGui::TextV(format, args);
            }
            va_end(args);
        }

        ImVec4 color_from_bytes(uint8_t r, uint8_t g, uint8_t b) {
            return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
        }
    } // namespace helpers

} // namespace GUI