#include <SDL.h>
#include <imgui.h>
#include "core/emuwrapper.hpp"
#include "gui/guicomponent.hpp"
#include "gui/game.hpp"
#include <memory>

namespace GUI {
    class Main {
    private:
        SDL_Window*   window;
        SDL_Renderer* renderer;

        float font_size = 20.0f;

        core::EmuWrapper emu;

        Game game_window;

        std::vector<std::unique_ptr<GUIComponent>> windows;

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