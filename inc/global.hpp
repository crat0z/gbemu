#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#include <SDL.h>
#include <imgui.h>
#include <vector>
#include "input/keymap.hpp"

class global {

    std::vector<SDL_Texture*> m_icon_texts;

    input::Keys m_keymap;

    ImVec4 white;
    ImVec4 black;

    ImGuiID m_dock_id;

private:
    global() = default;

    global(const global& other) = delete;
    global operator=(const global& other) = delete;

public:
    static global& get() {
        static global g;
        return g;
    }

    static std::vector<SDL_Texture*>& icon_textures() { return get().m_icon_texts; }

    static ImVec4& white_vec() { return get().white; }
    static ImVec4& black_vec() { return get().black; }

    static input::Keys& keymap() { return get().m_keymap; }

    static ImGuiID& dock_id() { return get().m_dock_id; }
};

#endif