#include <SDL.h>
#include <imgui.h>
#include "imgui-filebrowser/imfilebrowser.h"
#include "chip8.hpp"
#include "debugger.hpp"
#include "disassembler.hpp"

enum icons
{
    PAUSE,
    STEP_INTO,
    CONTINUE
};

class GUI {
private:
    Chip8        emu;
    Debugger     debugger;
    Disassembler disassembler;

    ImGui::FileBrowser filebrowser;

    SDL_Window*   window;
    SDL_Renderer* renderer;

    std::array<SDL_Texture*, 3> icon_textures;

    ImVec4 white_vec;
    ImVec4 black_vec;

    bool done = false;

    void style();

    // various windows
    bool emulator_settings();
    bool debugger_window();
    bool launch_settings();

    void game_window();

    void prepare_imgui();

    void draw();
    void handle_input();

public:
    GUI();
    ~GUI();
    void run();
};