/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <functional>

class Input {
public:
    Input();
    ~Input();

    // Voids
    void process_events();
    void set_window_event_callback(std::function<void(const SDL_Event&)> callback);

    // Bools
    [[nodiscard]] bool is_key_down(SDL_Keycode key) const;
    [[nodiscard]] bool is_key_up(SDL_Keycode key) const;
    [[nodiscard]] bool is_key_held(SDL_Keycode key) const;
private:
    // Ints
    static constexpr int KEY_COUNT_ = 322;

    // Arrays
    std::array<bool, KEY_COUNT_> keys_{};
    std::array<bool, KEY_COUNT_> prev_keys_{};

    // Functions
    std::function<void(const SDL_Event&)> window_event_callback_;
};