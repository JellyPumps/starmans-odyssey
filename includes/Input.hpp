/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

#include <SDL2/SDL.h>
#include <array>
#include <functional>
#include <glm/glm.hpp>

class Input {
public:
    Input();
    ~Input();

    // ---- Event Processing ----
    void process_events();
    void set_window_event_callback(std::function<void(const SDL_Event&)> callback);

    // ---- Keyboard Input ----
    [[nodiscard]] bool is_key_down(SDL_Keycode key) const;
    [[nodiscard]] bool is_key_up(SDL_Keycode key) const;
    [[nodiscard]] bool is_key_held(SDL_Keycode key) const;

    // ---- Mouse Input ----
    [[nodiscard]] bool is_mouse_button_down(uint8_t button) const;
    [[nodiscard]] bool is_mouse_button_up(uint8_t button) const;
    [[nodiscard]] bool is_mouse_button_held(uint8_t button) const;

    [[nodiscard]] int get_mouse_x() const;
    [[nodiscard]] int get_mouse_y() const;
    [[nodiscard]] glm::vec2 get_mouse_position() const;
    [[nodiscard]] glm::vec2 get_mouse_delta() const;
private:
    // ---- Keyboard State ----
    static constexpr int KEY_COUNT_ = 322;
    std::array<bool, KEY_COUNT_> keys_{};
    std::array<bool, KEY_COUNT_> prev_keys_{};

    // ---- Mouse State ----
    std::array<bool, 5> mouse_buttons_{};
    std::array<bool, 5> prev_mouse_buttons_{};
    glm::vec2 mouse_position_ = glm::vec2(0, 0);
    glm::vec2 mouse_delta_ = glm::vec2(0, 0);

    // Callback
    std::function<void(const SDL_Event&)> window_event_callback_;
};