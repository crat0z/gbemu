#include <SDL2/SDL.h>
#include <array>

class Graphics {
private:
    SDL_Window*   window;
    SDL_Renderer* renderer;

    std::array<std::array<bool, 64>, 32> pixels = {};

public:
    Graphics();

    void draw();

    void clear();

    // other overloads too cuz why not
    auto operator()(int y, int x) const { return pixels[y][x]; }

    auto& operator()(int y, int x) { return pixels[y][x]; }

    auto& operator[](int y) { return pixels[y]; }
};