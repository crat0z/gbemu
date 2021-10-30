#include "graphics.hpp"
#include <vector>

// initialization
Graphics::Graphics() {

    rects.reserve(32 * 64);

    // create window
    window = SDL_CreateWindow("emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 640,
                              SDL_WINDOW_SHOWN);

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
                rects.emplace_back(x * 20, y * 20, 20, 20);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    SDL_RenderFillRects(renderer, rects.data(), rects.size());

    SDL_RenderPresent(renderer);
}