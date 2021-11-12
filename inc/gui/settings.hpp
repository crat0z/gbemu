#ifndef SETTINGS_H
#define SETTINGS_H

#include "gui/guicomponent.hpp"

namespace GUI {

    class Settings : public GUIComponent {

    public:
        Settings(float fs);

        virtual void draw_window() override;
    };

} // namespace GUI

#endif