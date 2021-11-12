#ifndef GUIMESSAGE_HPP
#define GUIMESSAGE_HPP

#include <any>

namespace GUI {
    enum class gui_component
    {
        debugger,
        game,
        emu_settings,
        launch_settings
    };

    enum class gui_action
    {
        update
    };

    struct GUIMessage {

        gui_component target;
        gui_action    act;
        std::any      data;
    };

} // namespace GUI

#endif