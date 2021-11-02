#include <imgui.h>
#include <SDL.h>

class GUI {
    SDL_Window*   window;
    SDL_Renderer* renderer;

    int current_x;
    int current_y;

    void draw_window();

public:
    GUI();
    ~GUI();

    void run();
};