/*
 * Created by Sarthak Rai on 09 Apr 2025.
*/

#include "Player.hpp"

namespace STARMAN {
  // ---- Constructor ----
  Player::Player(const glm::vec3& position, float yaw, float pitch, float speed) :
  position_(position), yaw_(yaw), pitch_(pitch), speed_(speed) {
    // ---- Initialize Direction Vectors ----
    update_vectors();

    // ---- Create Camera ----
    camera_ = std::make_shared<STARBORN::Camera>(
      position_,
      position_ + direction_,
      45.0f, 16.0f / 9.0f,
      0.1f, 100.0f
    );
  }

  // ---- Update ----
  void Player::update(float delta_time) {
    handle_rotation();
    handle_movement(delta_time);
    sync_camera();
  }

  // ---- Helpers ----
  void Player::update_vectors() {
    // ---- Calculate Direction from Yaw and Pitch ----
    direction_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    direction_.y = sin(glm::radians(pitch_));
    direction_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    direction_ = glm::normalize(direction_);

    // ---- Calculate Right and Up ----
    right_ = normalize(cross(direction_, glm::vec3(0.0f, 1.0f, 0.0f)));
    up_ = normalize(cross(right_, direction_));
  }

  void Player::sync_camera() const {
    if (camera_) {
      camera_->set_position(position_);
      camera_->set_target(position_ + direction_);
      camera_->set_direction(direction_);
    }
  }

  // ---- Movement ----
  void Player::handle_movement(float delta_time) {
    auto& input = STARBORN::Input::get_instance();
    const float movement_speed = speed_ * delta_time;
    glm::vec3 movement_direction(0.0f);

    // ---- Handle Input ----
    if (input.is_key_down(GLFW_KEY_W))
      movement_direction += direction_;
    if (input.is_key_down(GLFW_KEY_S))
      movement_direction -= direction_;
    if (input.is_key_down(GLFW_KEY_A))
      movement_direction -= right_;
    if (input.is_key_down(GLFW_KEY_D))
      movement_direction += right_;

    // ---- Normalize ----
    if (length(movement_direction) > 0.0f)
      movement_direction = normalize(movement_direction);

    // ---- Apply ----
    position_ += movement_direction * movement_speed;
  }

  void Player::handle_rotation() {
    auto& input = STARBORN::Input::get_instance();
    static bool first_mouse = true;
    static float last_x = 400.0f;
    static float last_y = 300.0f;

    glm::vec2 cursor_position = input.get_cursor_position();

    // ---- First Frame ----
    if (first_mouse) {
      last_x = cursor_position.x;
      last_y = cursor_position.y;
      first_mouse = false;
    }

    // ---- Calculate Offsets ----
    float x_offset = cursor_position.x - last_x;
    float y_offset = last_y - cursor_position.y;

    last_x = cursor_position.x;
    last_y = cursor_position.y;

    // ---- Apply Sensitivity ----
    x_offset *= mouse_sensitivity_;
    y_offset *= mouse_sensitivity_;

    // ---- Update Yaw and Pitch ----
    yaw_ += x_offset;
    pitch_ += y_offset;

    // ---- Constrain Pitch ----
    if (pitch_ > 89.0f) pitch_ = 89.0f;
    if (pitch_ < -89.0f) pitch_ = -89.0f;

    // ---- Update Vectors ----
    update_vectors();
  }
} // STARMAN