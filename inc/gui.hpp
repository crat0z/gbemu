#include <SDL.h>
#include <array>
#include <vector>
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

    bool done = false;

    SDL_Window*   window;
    SDL_Renderer* renderer;

    std::vector<SDL_FRect> rects;

    ImVec4 white_vec;
    ImVec4 black_vec;

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