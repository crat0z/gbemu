#ifndef GUIMESSAGE_HPP
#define GUIMESSAGE_HPP

#include <cstdint>
#include <any>

namespace GUI {
    enum class gui_component
    {
        all,
        disassembly_view,
        memory_view,
        register_view,
        stack_view,
        launcher,
        settings
    };

    enum class gui_action
    {
        scroll,
        new_game
    };

    struct GUIMessage {

        gui_component target;
        gui_action    act;
        std::any      data;

        GUIMessage(gui_component t, gui_action a, std::any d = {})
                : target{ t }, act{ a }, data{ d } {}
    };

    struct ScrollMessage {
        uint16_t target_address = 0;
        bool     save_history   = false;
    };
} // namespace GUI

#endif