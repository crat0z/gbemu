#include <imgui.h>
#include <cstdint>

namespace GUI {
    namespace helpers {

        void center_cursor(float item_size);

        void right_align_cursor(float item_size, float offset);

        void right_align_text(const char* format, float offset = 0.0f);

        // centers text horizontally, if it knows the size.
        void center_text_known(const char* format, float size, ...);

        bool center_button(const char* label);

        // centers a string
        void center_text(const char* format);

        void colored_centered_text_known(const ImVec4& colour, bool cond, float size,
                                         const char* format, ...);

        void colored_text(const ImVec4& colour, bool cond, const char* format, ...);

        ImVec4 color_from_bytes(uint8_t r, uint8_t g, uint8_t b);

    } // namespace helpers

} // namespace GUI
