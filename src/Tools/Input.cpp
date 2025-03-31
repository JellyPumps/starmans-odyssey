/*
* Created by Sarthak Rai on 3/31/25.
*/

#include "Input.hpp"
#include <iostream>


// Constructor & Destructor
Input::Input() {
    keys_.fill(false);
    prev_keys_.fill(false);
}

Input::~Input() = default;

// Voids
void Input::process_events() {
    SDL_Event event;

    prev_keys_ = keys_;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
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

// Bools
bool Input::is_key_down(const SDL_Keycode key) const {
    return key < KEY_COUNT_ && keys_[key];
}

bool Input::is_key_up(const SDL_Keycode key) const {
    return key < KEY_COUNT_ && !keys_[key] && prev_keys_[key];
}

bool Input::is_key_held(const SDL_Keycode key) const {
    return key < KEY_COUNT_&& keys_[key] && prev_keys_[key];
}
