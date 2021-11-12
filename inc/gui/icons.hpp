#ifndef ICONS_HPP
#define ICONS_HPP

#include <vector>
#include <SDL.h>

namespace GUI {

    enum icons
    {
        PAUSE,
        STEP_INTO,
        STEP_OVER,
        CONTINUE,
        REFRESH,
        STEP_OUT,
        BREAKPOINT,
        ARROW_LEFT,
        ARROW_RIGHT,
        ARROW_LEFT_INACTIVE,
        ARROW_RIGHT_INACTIVE,
        ARROW_RIGHT_PC
    };

    std::vector<SDL_Texture*> generate_icons(int font_size, SDL_Renderer* renderer);
} // namespace GUI

#endif