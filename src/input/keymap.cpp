#include "input/keymap.hpp"

namespace {
    std::vector<std::pair<SDL_Keycode, uint8_t>> default_keymap = {
        { SDLK_1, 0x1 }, { SDLK_2, 0x2 }, { SDLK_3, 0x3 }, { SDLK_4, 0xC },
        { SDLK_q, 0x4 }, { SDLK_w, 0x5 }, { SDLK_e, 0x6 }, { SDLK_r, 0xD },
        { SDLK_a, 0x7 }, { SDLK_s, 0x8 }, { SDLK_d, 0x9 }, { SDLK_f, 0xE },
        { SDLK_z, 0xA }, { SDLK_x, 0x0 }, { SDLK_c, 0xB }, { SDLK_v, 0xF }
    };
}

namespace input {

    Keys::Keys() { data = default_keymap; };

    Keys::~Keys() = default;

    // set a key from old value to a new one, returns true
    // if successful, returns false otherwise
    bool Keys::change_key(SDL_Keycode from, SDL_Keycode to) {
        for (auto& v : data) {
            if (v.first == from) {
                v.first = to;
                return true;
            }
        }
        return false;
    }

    std::pair<SDL_Keycode, uint8_t>& Keys::operator[](size_t v) noexcept { return data[v]; }

    std::optional<uint8_t> Keys::translate_key(SDL_Keycode k) const noexcept {
        for (auto& p : data) {
            if (p.first == k) {
                return p.second;
            }
        }
        return std::nullopt;
    }

    bool Keys::contains(SDL_Keycode k) const noexcept {
        for (auto& v : data) {
            if (k == v.first) {
                return true;
            }
        }
        return false;
    }

    bool Keys::contains(uint8_t k) const noexcept {
        for (auto& v : data) {
            if (k == v.second) {
                return true;
            }
        }
        return false;
    }

    // this is actually safe, since graphics and keyboard events are processed
    // in same thread
    void Keys::reset() noexcept { data = { default_keymap }; }

    size_t Keys::size() const noexcept { return data.size(); }

} // namespace input