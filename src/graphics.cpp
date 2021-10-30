#include "graphics.hpp"

// initialization
Graphics::Graphics() {
    // create window
    window = SDL_CreateWindow("emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 640,
                              SDL_WINDOW_SHOWN);

    // setup renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);
    // each square is 20x20, todo
    // allow for rescaling
    SDL_Rect rect;
    rect.w = 20;
    rect.h = 20;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (auto y = 0; y < 32; ++y) {
        for (auto x = 0; x < 64; ++x) {
            if (pixels[y][x]) {
                rect.y = y * 20;
                rect.x = x * 20;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}