/*
 * Created by Sarthak Rai on 05 Apr 2025.
*/

#include "Camera.hpp"

namespace STARBORN {
  // ---- Constructor ----
   Camera::Camera(const glm::vec3 &position, const glm::vec3 &target, float fov,
               float aspect_ratio, float near_plane, float far_plane)
                 : position_(position), target_(target) {
     direction_ = normalize(position - target);
     up_ = glm::vec3(0.0f, 1.0f, 0.0f);
     right_ = normalize(cross(up_, direction_));
     camera_up_ = normalize(cross(direction_, right_));
     projection_ = glm::perspective(fov, aspect_ratio, near_plane, far_plane);
  }

  // ---- Getters ----
  glm::mat4 Camera::get_view_matrix() const {
     return glm::lookAt(position_, target_, up_);
  }

  glm::mat4 Camera::get_projection_matrix() const {
     return projection_;
  }

} // STARBORN