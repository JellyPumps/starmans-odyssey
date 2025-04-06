/*
 * Created by Sarthak Rai on 06 Apr 2025.
*/

#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>
#include <glm/glm.hpp>
#include <vector>

namespace STARBORN {

class Input {
private:
  // ---- Constructor & Destructor ----
  Input() = default;
  ~Input() = default;

  // ---- Callback Handlers ----
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
  static void cursor_position_callback(GLFWwindow* window, double x, double y);
  static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

  // ---- Internal State ----
  GLFWwindow* window_ = nullptr;

  // ---- Key States ----
  std::unordered_map<int, bool> current_keys_;
  std::unordered_map<int, bool> previous_keys_;

  // ---- Mouse States ----
  std::unordered_map<int, bool> current_mouse_buttons_;
  std::unordered_map<int, bool> previous_mouse_buttons_;
  glm::vec2 current_cursor_position_;
  glm::vec2 previous_cursor_position_;

  // ---- Scroll States ----
  float scroll_delta_ = 0.0f;

  // ---- Callbacks ----
  std::vector<std::function<void(int, int)>> key_callbacks_;
  std::vector<std::function<void(double, double)>> mouse_callbacks_;
  std::vector<std::function<void(double, double)>> scroll_callbacks_;
public:
  // ---- Singleton Instance ----
  static Input& get_instance() {
    static Input instance;
    return instance;
  }

  // ---- Initialization ----
  Input(const Input&) = delete;
  Input& operator=(const Input&) = delete;

  void init(GLFWwindow* window);

  // ---- Update ----
  void update();

  // ---- Query Key State ----
  bool is_key_pressed(const int key) {
    return current_keys_[key] && !previous_keys_[key];
  };
  bool is_key_released(const int key) {
    return !current_keys_[key] && previous_keys_[key];
  };
  bool is_key_down(const int key) {
    return current_keys_[key];
  };

  // --- Query Mouse State ----
  bool is_mouse_button_pressed(int button) {
    return current_mouse_buttons_[button] && !previous_mouse_buttons_[button];
  };
  bool is_mouse_button_released(int button) {
    return !current_mouse_buttons_[button] && previous_mouse_buttons_[button];
  };
  bool is_mouse_button_down(int button) {
    return current_mouse_buttons_[button];
  };

  // --- Query Cursor Position ----
  glm::vec2 get_cursor_position() const {
    return current_cursor_position_;
  };
  glm::vec2 get_cursor_delta() const {
    return current_cursor_position_ - previous_cursor_position_;
  };

  // --- Query Scroll ----
  float get_scroll_delta() const {
    return scroll_delta_;
  };

  // --- Register Callbacks ----
  void register_key_callback(const std::function<void(int, int)>& callback) {
    key_callbacks_.push_back(callback);
  };
  void register_mouse_callback(const std::function<void(double, double)>& callback) {
    mouse_callbacks_.push_back(callback);
  };
  void register_scroll_callback(const std::function<void(double, double)>& callback) {
    scroll_callbacks_.push_back(callback);
  };
};

} // STARBORN
