/*
* Created by Sarthak Rai on 3/31/25.
*/

#include "Window.hpp"
#include <GL/glew.h>
#include <iostream>
#include <ostream>

// Constructor & Destructor
Window::Window(const char* title, const int width, const int height, const Uint32 flags)
: title_(title), width_(width), height_(height), flags_(flags) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL_Init Failed! <" + std::string(SDL_GetError()) + ">");
    }

    const char *video_driver = SDL_GetCurrentVideoDriver();
    std::cout << "Video driver: " << video_driver << std::endl;

    window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                          width_, height_, SDL_WINDOW_OPENGL | flags_);

    if (!window_) {
        throw std::runtime_error("SDL_CreateWindow Failed! <" + std::string(SDL_GetError()) + ">");
    }

    // Create OpenGL context
    gl_context_ = SDL_GL_CreateContext(window_);
    if (!gl_context_) {
        throw std::runtime_error("SDL_GL_CreateContext Failed! <" + std::string(SDL_GetError()) + ">");
    }

    surface_ = get_surface();
}

Window::~Window() {
    if (gl_context_) { SDL_GL_DeleteContext(gl_context_); }
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
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "GL Error in swap: " << err << std::endl;
    }
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