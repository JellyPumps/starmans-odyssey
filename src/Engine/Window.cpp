/*
 * Created by Sarthak Rai on 01 Apr 2025.
*/

#include "Window.hpp"
#include <stdexcept>

namespace STARBORN {
  // ---- Constructor & Destructor ----
  Window::Window(int width, int height, const char *title) : width_(width), height_(height), title_(title) {
    init_GLFW();
    create_window();
    init_GLAD();
    set_viewport();
  }

  Window::~Window() { glfwDestroyWindow(window_); }

  // ---- Initialize GLFW ----
  void Window::init_GLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }

  // ---- Create A Window ----
  void Window::create_window() {
    window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);

    if (window_ == nullptr) {
      glfwTerminate();
      throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window_);
  }

  // ---- Initialize GLAD ----
  void Window::init_GLAD() {
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
      glfwDestroyWindow(window_);
      glfwTerminate();
      throw std::runtime_error("Failed to initialize GLAD");
    }
  }

  // ---- Set Viewport ----
  void Window::set_viewport() const {
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
  }

  // ---- Callback ----
  void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
  }

} // STARBORN