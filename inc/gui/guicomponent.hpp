#ifndef GUICOMPONENT_HPP
#define GUICOMPONENT_HPP

namespace GUI {
    class GUIComponent {
    protected:
        bool  window_state;
        float font_size;

        GUIComponent() = delete;

    public:
        GUIComponent(float fs, bool start = false) : window_state{ start }, font_size{ fs } {}

        bool& is_enabled() { return window_state; }

        virtual void toggle_window() { window_state = !window_state; }
        virtual void draw_window() = 0;
    };
} // namespace GUI

#endif