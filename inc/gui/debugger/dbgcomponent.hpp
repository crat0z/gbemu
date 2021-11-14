#ifndef DBGCOMPONENT_HPP
#define DBGCOMPONENT_HPP

#include "gui/guicomponent.hpp"
#include "core/emuwrapper.hpp"

namespace GUI {
    class DbgComponent : public GUIComponent {
    protected:
        core::EmuWrapper& emu;

        DbgComponent() = delete;

    public:
        DbgComponent(float fs, core::EmuWrapper& e) : GUIComponent(fs), emu{ e } {}
    };
} // namespace GUI
#endif