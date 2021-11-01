#include "graphics.hpp"
#include <cmath>

constexpr float X_PIXELS = 64.0;
constexpr float Y_PIXELS = 32.0;

// initialization
Graphics::Graphics(int x, int y) : size_x{ x }, size_y{ y } {

    /* 
       don't actually make it what user wants instead make it close to what
       they want 
    */

    auto x_scale_sq = (x / X_PIXELS) * (x / X_PIXELS);
    auto y_scale_sq = (y / Y_PIXELS) * (y / Y_PIXELS);

    scale = std::sqrt((x_scale_sq + y_scale_sq) / 2);

    // buffer for pixels when we draw
    rects.reserve(32 * 64);

    // create window
    window = SDL_CreateWindow("chip8emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              64 * scale, 32 * scale, SDL_WINDOW_SHOWN);

    // setup renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void Graphics::clear() {
    for (auto& line : pixels) {
        for (auto& pixel : line) {
            pixel = false;
        }
    }
}

void Graphics::draw() {

    rects.clear();

    for (auto y = 0; y < 32; ++y) {
        for (auto x = 0; x < 64; ++x) {
            if (pixels[y][x]) {
                rects.emplace_back(x * scale, y * scale, scale, scale);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    SDL_RenderFillRectsF(renderer, rects.data(), rects.size());

    SDL_RenderPresent(renderer);
}