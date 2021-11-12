#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include "gui/guicomponent.hpp"
#include <imgui.h>
#include "imgui-filebrowser/imfilebrowser.h"
#include "core/emuwrapper.hpp"

namespace GUI {

    class Launcher : public GUIComponent {
        ImGui::FileBrowser filebrowser;

        core::EmuWrapper& emu;

    public:
        Launcher(float fs, core::EmuWrapper& e);

        virtual void draw_window() override;
    };

} // namespace GUI

#endif