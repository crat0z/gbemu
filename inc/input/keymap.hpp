#ifndef KEYMAP_HPP
#define KEYMAP_HPP
#include <vector>
#include <optional>
#include <SDL.h>

namespace input {

    class Keys {
        std::vector<std::pair<SDL_Keycode, uint8_t>> data;

    public:
        Keys();

        ~Keys();

        // set a key from old value to a new one, returns true
        // if successful, returns false otherwise
        bool change_key(SDL_Keycode from, SDL_Keycode to);

        std::pair<SDL_Keycode, uint8_t>& operator[](size_t v) noexcept;

        std::optional<uint8_t> translate_key(SDL_Keycode k) const noexcept;

        bool contains(SDL_Keycode k) const noexcept;

        bool contains(uint8_t k) const noexcept;

        void reset() noexcept;

        size_t size() const noexcept;
    };
} // namespace input

#endif