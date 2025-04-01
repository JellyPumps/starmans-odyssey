/*
* Created by Sarthak Rai on 3/31/25.
*/

#include "Window.hpp"

#include <iostream>
#include <ostream>

// Constructor & Destructor
Window::Window(const char* title, const int width, const int height, const Uint32 flags)
: title_(title), width_(width), height_(height), flags_(flags) {
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO);

    const char *video_driver = SDL_GetCurrentVideoDriver();
    std::cout << "Video driver: " << video_driver << std::endl;

    window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, flags_);

    if (!window_) {
        throw std::runtime_error("SDL_CreateWindow Failed! <" + std::string(SDL_GetError()) + ">");
    }

    surface_ = get_surface();
}

Window::~Window() {
    if (window_) { SDL_DestroyWindow(window_); }
}

// Setters
void Window::toggle_fullscreen() {
    fullscreen_ = !fullscreen_;
    if (fullscreen_) {
        SDL_SetWindowFullscreen(window_,  SDL_WINDOW_FULLSCREEN);
    } else {
        SDL_SetWindowFullscreen(window_, 0);
        SDL_SetWindowSize(window_, width_, height_);
        SDL_SetWindowBordered(window_, SDL_TRUE);
    }
}

void Window::resize(const int width, const int height) {
    height_ = height;
    width_ = width;
    SDL_SetWindowSize(window_, width, height);
}

void Window::swap_buffers() const {
    SDL_GL_SwapWindow(window_);
}

// Getters
SDL_Window *Window::get_context() const {
    return window_;
}

SDL_Surface *Window::get_surface() const {
    return SDL_GetWindowSurface(window_);
}

int Window::get_width() const {
    return width_;
}

int Window::get_height() const {
    return height_;
}

bool Window::is_fullscreen() const {
    return fullscreen_;
}