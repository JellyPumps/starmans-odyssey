/*
 * Created by Sarthak Rai on 09 Apr 2025.
*/

#pragma once

#include "Camera.hpp"
#include "Input.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace STARMAN {

class Player {
private:
  // ---- Player Properties ----
  glm::vec3 position_;
  glm::vec3 direction_;
  glm::vec3 up_;
  glm::vec3 right_;
  float yaw_;
  float pitch_;
  float speed_;
  float mouse_sensitivity_ = 0.1f;

  // ---- Private Methods ----
  void handle_rotation();
  void handle_movement(float delta_time);
  void update_vectors();
  void sync_camera() const;

  // ---- Camera ----
  std::shared_ptr<STARBORN::Camera> camera_;
public:
  // ---- Constructor ----
  explicit Player(const glm::vec3 &position, float yaw = -90.0f,
                  float pitch = 0.0f, float speed = 2.5f);

  // ---- Update ----
  void update(float delta_time);

  // ---- Getters ----
  [[nodiscard]] std::shared_ptr<STARBORN::Camera> get_camera() const { return camera_; };
  [[nodiscard]] glm::vec3 get_position() const { return position_; };
  [[nodiscard]] glm::vec3 get_direction() const { return direction_; };
  [[nodiscard]] glm::vec3 get_up() const { return up_; };
  [[nodiscard]] glm::vec3 get_right() const { return right_; };
  [[nodiscard]] float get_yaw() const { return yaw_; };
  [[nodiscard]] float get_pitch() const { return pitch_; };
  [[nodiscard]] float get_speed() const { return speed_; };

  // ---- Setters ----
  void set_position(const glm::vec3& position) {
    position_ = position;
    sync_camera();
  };
  void set_sensitivity(float sensitivity) { mouse_sensitivity_ = sensitivity; };
  void set_speed(float speed) { speed_ = speed; };
  void set_aspect_ratio(float aspect_ratio) {
    if (camera_) {
      camera_ = std::make_shared<STARBORN::Camera>(
        position_,
        position_ + direction_,
        45.0f, aspect_ratio,
        0.1f, 100.0f
      );
    }
  }
};

} // STARMAN
