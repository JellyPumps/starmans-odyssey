/*
* Created by Sarthak Rai on 3/31/25.
*/

#include "Input.hpp"
#include <iostream>


// Constructor & Destructor
Input::Input() {
    keys_.fill(false);
    prev_keys_.fill(false);
    mouse_buttons_.fill(false);
    prev_mouse_buttons_.fill(false);

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

Input::~Input() {
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

// Voids
void Input::process_events() {
    SDL_Event event;

    prev_keys_ = keys_;
    prev_mouse_buttons_ = mouse_buttons_;
    mouse_delta_ = glm::vec2(0, 0);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            // ---- Keyboard Events ----
            case SDL_KEYDOWN:
                if (event.key.keysym.sym < KEY_COUNT_) {
                    keys_[event.key.keysym.sym] = true;
                }
                break;

            case SDL_KEYUP:
                if (event.key.keysym.sym < KEY_COUNT_) {
                    keys_[event.key.keysym.sym] = false;
                }
                break;

            // ---- Mouse Events ----
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button <= 5) { // SDL only supports 5 mouse buttons
                    mouse_buttons_[event.button.button - 1] = true;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button <= 5) {
                    mouse_buttons_[event.button.button - 1] = false;
                }
                break;

            case SDL_MOUSEMOTION:
                mouse_position_ = glm::vec2(event.motion.x, event.motion.y);
                mouse_delta_ = glm::vec2(event.motion.xrel, event.motion.yrel);
                break;

            // ---- Window Events ----
            case SDL_WINDOWEVENT:
                if (window_event_callback_) { window_event_callback_(event); }
                break;

            default:
                break;
        }
    }
}

void Input::set_window_event_callback(std::function<void(const SDL_Event&)> callback) {
    window_event_callback_ = std::move(callback);
}

// ---- Keyboard Input ----
bool Input::is_key_down(const SDL_Keycode key) const {
    return key < KEY_COUNT_ && keys_[key];
}

bool Input::is_key_up(const SDL_Keycode key) const {
    return key < KEY_COUNT_ && !keys_[key] && prev_keys_[key];
}

bool Input::is_key_held(const SDL_Keycode key) const {
    return key < KEY_COUNT_&& keys_[key] && prev_keys_[key];
}

// ---- Mouse Input ----
bool Input::is_mouse_button_down(uint8_t button) const {
    return button <= 5 && mouse_buttons_[button - 1] && !prev_mouse_buttons_[button - 1];
}

bool Input::is_mouse_button_up(uint8_t button) const {
    return button <= 5 && !mouse_buttons_[button - 1] && prev_mouse_buttons_[button - 1];
}

bool Input::is_mouse_button_held(uint8_t button) const {
    return button <= 5 && mouse_buttons_[button - 1] && prev_mouse_buttons_[button - 1];
}

glm::vec2 Input::get_mouse_position() const {
    return mouse_position_;
}

glm::vec2 Input::get_mouse_delta() const {
    return mouse_delta_;
}