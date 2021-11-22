#include "gui/icons.hpp"
#include <array>
#include <fmt/format.h>
#include <SDL_image.h>
// all icon info is here

using namespace GUI;

namespace {
    constexpr std::array<std::pair<icons, const char*>, 12> svg_pairs = {
        { { PAUSE,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path d='M4.5 3H6v10H4.5V3zm7 0v10H10V3h1.5z'/></svg>" },
          { STEP_INTO,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M8 9.532h.542l3.905-3.905-1.061-1.06-2.637 2.61V1H7.251v6.177l-2.637-2.61-1.061 1.06 3.905 3.905H8zm1.956 3.481a2 2 0 1 1-4 0 2 2 0 0 1 4 0z'/></svg>" },
          { STEP_OVER,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M14.25 5.75v-4h-1.5v2.542c-1.145-1.359-2.911-2.209-4.84-2.209-3.177 0-5.92 2.307-6.16 5.398l-.02.269h1.501l.022-.226c.212-2.195 2.202-3.94 4.656-3.94 1.736 0 3.244.875 4.05 2.166h-2.83v1.5h4.163l.962-.975V5.75h-.004zM8 14a2 2 0 1 0 0-4 2 2 0 0 0 0 4z'/></svg>" },
          { CONTINUE,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M2.5 2H4v12H2.5V2zm4.936.39L6.25 3v10l1.186.61 7-5V7.39l-7-5zM12.71 8l-4.96 3.543V4.457L12.71 8z'/></svg>" },
          { REFRESH,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M4.681 3H2V2h3.5l.5.5V6H5V4a5 5 0 1 0 4.53-.761l.302-.954A6 6 0 1 1 4.681 3z'/></svg>" },
          { STEP_OUT,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M8 1h-.542L3.553 4.905l1.061 1.06 2.637-2.61v6.177h1.498V3.355l2.637 2.61 1.061-1.06L8.542 1H8zm1.956 12.013a2 2 0 1 1-4 0 2 2 0 0 1 4 0z'/></svg>" },
          { BREAKPOINT,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='red'><path d='M8 4c.367 0 .721.048 1.063.145a3.943 3.943 0 0 1 1.762 1.031 3.944 3.944 0 0 1 1.03 1.762c.097.34.145.695.145 1.062 0 .367-.048.721-.145 1.063a3.94 3.94 0 0 1-1.03 1.765 4.017 4.017 0 0 1-1.762 1.031C8.72 11.953 8.367 12 8 12s-.721-.047-1.063-.14a4.056 4.056 0 0 1-1.765-1.032A4.055 4.055 0 0 1 4.14 9.062 3.992 3.992 0 0 1 4 8c0-.367.047-.721.14-1.063a4.02 4.02 0 0 1 .407-.953A4.089 4.089 0 0 1 5.98 4.546a3.94 3.94 0 0 1 .957-.401A3.89 3.89 0 0 1 8 4z'/></svg>" },
          { ARROW_LEFT,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M7 3.093l-5 5V8.8l5 5 .707-.707-4.146-4.147H14v-1H3.56L7.708 3.8 7 3.093z'/></svg>" },
          { ARROW_RIGHT,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='white'><path fill-rule='evenodd' clip-rule='evenodd' d='M9 13.887l5-5V8.18l-5-5-.707.707 4.146 4.147H2v1h10.44L8.292 13.18l.707.707z'/></svg>" },
          { ARROW_LEFT_INACTIVE,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='currentcolor'><path fill-rule='evenodd' clip-rule='evenodd' d='M7 3.093l-5 5V8.8l5 5 .707-.707-4.146-4.147H14v-1H3.56L7.708 3.8 7 3.093z'/></svg>" },
          { ARROW_RIGHT_INACTIVE,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='currentcolor'><path fill-rule='evenodd' clip-rule='evenodd' d='M9 13.887l5-5V8.18l-5-5-.707.707 4.146 4.147H2v1h10.44L8.292 13.18l.707.707z'/></svg>" },
          { ARROW_RIGHT_PC,
            "<svg width='{0}' height='{0}' viewBox='0 0 16 16' xmlns='http://www.w3.org/2000/svg' fill='green'><path fill-rule='evenodd' clip-rule='evenodd' d='M9 13.887l5-5V8.18l-5-5-.707.707 4.146 4.147H2v1h10.44L8.292 13.18l.707.707z'/></svg>" } }
    };
}

namespace GUI {
    std::vector<SDL_Texture*> generate_icons(int font_size, SDL_Renderer* renderer) {

        std::vector<SDL_Texture*> ret;

        for (auto& pair : svg_pairs) {
            auto  data = fmt::vformat(pair.second, fmt::make_format_args(font_size));
            auto* tmp  = SDL_RWFromConstMem(data.c_str(), data.size());
            auto* tmp2 = IMG_Load_RW(tmp, 1);
            ret.push_back(SDL_CreateTextureFromSurface(renderer, tmp2));
        }
        return ret;
    }
} // namespace GUI