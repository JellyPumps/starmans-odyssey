/*
* Created by Sarthak Rai on 3/31/25.
*/

#include "Rendering/Camera.hpp"
#include <cmath>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

namespace Renderer {
  Camera::Camera(const float fov_deg, const float aspect_ration,
               const float near_plane, const float far_plane)
  : fov_(fov_deg),
    aspect_ratio_(aspect_ration),
    near_plane_(near_plane),
    far_plane_(far_plane) {

    update_view_matrices();
    update_projection_matrices();
  }

  // Matrix updates
  void Camera::update_projection_matrices() {
    if (projection_type_ == ProjectionType::Perspective) {
      // Perspective projection (Using FOV)
      float fov_rad = glm::radians(fov_);
      float cot_fov = 1.0f / std::tan(fov_rad / 2.0f);

      projection_ = glm::mat4(0.0f);
      projection_[0][0] = cot_fov / aspect_ratio_;
      projection_[1][1] = cot_fov;
      projection_[2][2] = (far_plane_ + near_plane_) / (near_plane_ - far_plane_);
      projection_[2][3] = (2.0f * far_plane_ * near_plane_) / (near_plane_ - far_plane_);
      projection_[3][2] = -1.0f;
    } else {
      // Ortho
      // Not implemented yet
    }
    view_projection_ = projection_ * view_;
    dirty_ = false;
  }

  void Camera::update_view_matrices() {
    update_basis_vectors();

    view_ = lookAt(position_, position_ + forward_, up_);
    view_projection_ = projection_ * view_;
    dirty_ = false;
  }

  // Basis vectors
  void Camera::update_basis_vectors() {
    // Convert euler angles to rotation matrix
    glm::mat4 rotation_mat = toMat4(orientation_);
    forward_ = glm::vec3(rotation_mat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    up_ = glm::vec3(rotation_mat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    right_ = glm::vec3(rotation_mat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
  }

  // Setters
  void Camera::set_position(const glm::vec3 &position) {
    position_ = position;
    dirty_ = true;
  }

  void Camera::set_rotation(const glm::vec3 &euler_degrees) {
    orientation_ = glm::quat(radians(euler_degrees));
    dirty_ = true;
  }

  void Camera::look_at(const glm::vec3 &target, glm::vec3 up) {
    forward_ = normalize(target - position_);
    orientation_ = quatLookAt(forward_, up);
    dirty_ = true;
  }

  void Camera::set_perspective(float fov_deg, float aspect_ratio,
                               float near_plane, float far_plane) {
    fov_ = fov_deg;
    aspect_ratio_ = aspect_ratio;
    near_plane_ = near_plane;
    far_plane_ = far_plane;
    projection_type_ = ProjectionType::Perspective;
    dirty_ = true;
  }

  // Set orthographic --- Not implemented yet

  // Movement
  void Camera::translate(const glm::vec3 &translation) {
    position_ += translation;
    dirty_ = true;
  }

  void Camera::rotate(float pitch_deg, float yaw_deg, float roll_deg) {
    const glm::quat pitch = angleAxis(glm::radians(pitch_deg), right_);
    const glm::quat yaw = angleAxis(glm::radians(yaw_deg), up_);
    const glm::quat roll = angleAxis(glm::radians(roll_deg), forward_);

    orientation_ = yaw * pitch * roll * orientation_; // Order matters!!!
    dirty_ = true;
  }

  // Input
  void Camera::update_from_input(float delta_time, const Input &input) {
    // Rotation
    if (input.is_mouse_button_held(SDL_BUTTON_RIGHT)) {
      const glm::vec2 mouse_delta = input.get_mouse_delta();
      rotate(mouse_delta.y * mouse_sensitivity_, mouse_delta.x * mouse_sensitivity_, 0.0f);
    }
  }

} // Renderer