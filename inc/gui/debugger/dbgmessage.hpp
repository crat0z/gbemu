#include <cstdint>
#include <any>

namespace GUI {
    enum class dbg_component
    {
        disassembly_view,
        memory_view,
        register_view,
        stack_view
    };

    enum class dbg_action
    {
        scroll,
        update
    };

    struct DbgMessage {

        dbg_component target;
        dbg_action    act;
        std::any      data;
    };

    struct ScrollMessage {
        uint16_t target_address;
        bool     save_history;
    };
} // namespace GUI