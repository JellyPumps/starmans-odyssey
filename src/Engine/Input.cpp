/*
 * Created by Sarthak Rai on 06 Apr 2025.
*/

#include "Input.hpp"

namespace STARBORN {
  // ---- Initialization ----
  void Input::init(GLFWwindow* window) {
    window_ = window;

    // ---- Set Callbacks ----
    glfwSetKeyCallback(window_, key_callback);
    glfwSetMouseButtonCallback(window_, mouse_button_callback);
    glfwSetCursorPosCallback(window_, cursor_position_callback);
    glfwSetScrollCallback(window_, scroll_callback);

    // ---- Disable Cursor ----
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  // ---- Update ----
  void Input::update() {
    previous_keys_ = current_keys_;
    previous_mouse_buttons_ = current_mouse_buttons_;
    previous_cursor_position_ = current_cursor_position_;

    scroll_delta_ = 0.0f;
  }

  // ---- Static Callbacks ----
  void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto& input = get_instance();
    input.current_keys_[key] = (action != GLFW_RELEASE);

    // ---- Notify Callbacks ----
    for (auto& callback : input.key_callbacks_) {
      callback(key, action);
    }
  }

  void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto& input = get_instance();
    input.current_mouse_buttons_[button] = (action != GLFW_RELEASE);
  }

  void Input::cursor_position_callback(GLFWwindow *window, double x, double y) {
    auto& input = get_instance();
    input.current_cursor_position_ = glm::vec2(x, y);

    // ---- Notify Callbacks ----
    for (auto& callback : input.mouse_callbacks_) {
      callback(x, y);
    }
  }

  void Input::scroll_callback(GLFWwindow *window, double x_offset,
                              double y_offset) {
    auto& input = get_instance();
    input.scroll_delta_ = static_cast<float>(y_offset);

    // ---- Notify Callbacks ----
    for (auto& callback : input.scroll_callbacks_) {
      callback(x_offset, y_offset);
    }
  }
} // STARBORN