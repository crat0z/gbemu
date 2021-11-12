#ifndef GAME_HPP
#define GAME_HPP

#include "gui/guicomponent.hpp"
#include "core/emuwrapper.hpp"

namespace GUI {
    class Game : public GUIComponent {
        core::EmuWrapper& emu;

    public:
        Game(core::EmuWrapper& e);

        virtual void draw_window() override;
    };
} // namespace GUI

#endif