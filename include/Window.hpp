/*
 * Created by Sarthak Rai on 01 Apr 2025.
*/

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace STARBORN {

class Window {
private:
  // ---- Variables ----
  int width_;
  int height_;
  const char *title_;
  GLFWwindow *window_{};

  // ---- Private Methods ----
  static void init_GLFW();
  void init_GLAD();
  void create_window();
  void set_viewport() const;
  static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
public:
  // ---- Constructor & Destructor ----
  Window(int width, int height, const char *title);
  ~Window();

  // ---- Setters & Getters ----
  void set_title(const char *title) { title_ = title; }
  void set_size(const int width, const int height) { width_ = width; height_ = height; }

  [[nodiscard]] int get_width() const { return width_; }
  [[nodiscard]] int get_height() const { return height_; }
  [[nodiscard]] GLFWwindow *get_window() const { return window_; }
};

} // STARBORN
