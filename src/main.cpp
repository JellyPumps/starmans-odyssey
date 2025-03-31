/*
* Created by Sarthak Rai on 3/31/25.
* Entry point
*/

#include "Window.hpp"
#include "Input.hpp"
#include <SDL2/SDL.h>

int main() {
    Window window("SDL2 Window", 800, 600, SDL_WINDOW_SHOWN);

    Input input;
    bool running = true;

    input.set_window_event_callback([&running](const SDL_Event& event){
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) { running = false; }
    });

    while (running) {
        // Events
        input.process_events();

        if (input.is_key_down(SDLK_ESCAPE)) { running = false; }

        // Updates
        window.update();

    }

    SDL_Quit();
    return 0;
}
