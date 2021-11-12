#ifndef DBGCOMPONENT_HPP
#define DBGCOMPONENT_HPP

#include "gui/guicomponent.hpp"
#include "gui/debugger/dbgmessage.hpp"
#include "core/emuwrapper.hpp"
#include <optional>

namespace GUI {
    class DbgComponent : public GUIComponent {
    protected:
        core::EmuWrapper& emu;

        std::optional<DbgMessage> message;

        DbgComponent() = delete;

    public:
        DbgComponent(core::EmuWrapper& e, float fs) : GUIComponent(fs), emu{ e } {}

        std::optional<DbgMessage> get_message() {
            auto ret = std::move(message);
            message.reset();
            return ret;
        }

        virtual void process_dbg_msg(DbgMessage msg) = 0;
    };
} // namespace GUI
#endif