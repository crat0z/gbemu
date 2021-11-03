#include <SDL.h>
#include <array>
#include <vector>

#include "chip8.hpp"

class GUI {
private:
    Chip8 emu;

    bool done   = false;
    bool paused = false;

    SDL_Window*   window;
    SDL_Renderer* renderer;

    std::vector<SDL_FRect> rects;

    int size_x, size_y;

    void prepare_imgui();
    void prepare_game_gfx();

    void draw();
    void handle_input();

public:
    GUI(const std::string& file_name);
    ~GUI();
    void run();
};