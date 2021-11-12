#include <SDL.h>
#include <imgui.h>
#include "core/emuwrapper.hpp"
#include "gui/guicomponent.hpp"
#include <memory>

namespace GUI {
    class Main {
    private:
        SDL_Window*   window;
        SDL_Renderer* renderer;

        core::EmuWrapper emu;

        std::vector<std::shared_ptr<GUIComponent>> windows;

        bool done = false;

        void style();

        void prepare_imgui();

        void draw();
        void handle_input();

    public:
        Main();
        ~Main();
        void run();
    };
} // namespace GUI