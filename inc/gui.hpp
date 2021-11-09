#include <SDL.h>
#include <imgui.h>
#include "imgui-filebrowser/imfilebrowser.h"
#include "chip8.hpp"
#include "debugger.hpp"
#include "disassembler.hpp"

class GUI {
private:
    Chip8        emu;
    Debugger     debugger;
    Disassembler disassembler;

    ImGui::FileBrowser filebrowser;

    SDL_Window*   window;
    SDL_Renderer* renderer;

    std::array<SDL_Texture*, 15> icon_textures;

    std::array<bool, 20> window_state = {};

    ImVec4 white_vec;
    ImVec4 black_vec;

    ImGuiID dock_id;

    bool done = false;

    void style();

    // various windows

    void register_viewer();
    void disassembly();
    void stack_viewer();
    void memory_viewer();

    void emulator_settings();
    void debugger_window();
    void launch_settings();

    void game_window();

    void prepare_imgui();

    void draw();
    void handle_input();

public:
    GUI();
    ~GUI();
    void run();
};