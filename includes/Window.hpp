/*
* Created by Sarthak Rai on 3/31/25.
*/

#pragma once

#include <SDL2/SDL.h>

class Window {
public:
  Window(const char *title, int width, int height, Uint32 flags);
  ~Window();

  // Voids
  void toggle_fullscreen();
  void resize(int width, int height);
  void swap_buffers() const;

  // SDL
  [[nodiscard]] SDL_Window *get_context() const;
  [[nodiscard]] SDL_Surface *get_surface() const;

  // Ints
  [[nodiscard]] int get_width() const;
  [[nodiscard]] int get_height() const;

  // Bools
  [[nodiscard]] bool is_fullscreen() const;
private:
  // Strings
  const char *title_;

  // SDL
  SDL_Window *window_;
  SDL_Surface *surface_;
  SDL_DisplayMode display_mode_{};

  // Ints
  int width_;
  int height_;

  // Bools
  bool fullscreen_ = false;

  // Uint32
  Uint32 flags_;
};
