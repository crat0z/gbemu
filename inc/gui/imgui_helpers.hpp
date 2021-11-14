#include <imgui.h>
#include <cstdint>

namespace GUI {
    namespace helpers {

        void center_cursor(float item_size);

        bool center_button(const char* label, const ImVec2& size = ImVec2(0.0f, 0.0f));

        // centers a string
        void center_text(const char* format);

        void colored_centered_text(const ImVec4& colour, bool cond, const char* format);

        void disabled_centered_text(const char* format);

        void colored_text(const ImVec4& colour, bool cond, const char* format);

        ImVec4 color_from_bytes(uint8_t r, uint8_t g, uint8_t b);

    } // namespace helpers

} // namespace GUI
