/*
 * Created by Sarthak Rai on 05 Apr 2025.
*/

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace STARBORN {
  class Camera {
  private:
    // ---- Variables ----
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 direction_{};
    glm::vec3 up_{};
    glm::vec3 right_{};
    glm::vec3 camera_up_{};
    glm::mat4 projection_{};
  public:
    // ---- Constructor ----
    Camera(const glm::vec3& position, const glm::vec3& target, float fov, float aspect_ratio, float near_plane, float far_plane);

    // ---- Getters ----
    [[nodiscard]] glm::mat4 get_view_matrix() const;
    [[nodiscard]] glm::mat4 get_projection_matrix() const;
    [[nodiscard]] glm::vec3 get_position() const { return position_; };
    [[nodiscard]] glm::vec3 get_direction() const { return direction_; };
    [[nodiscard]] glm::vec3 get_up() const { return up_; };
    [[nodiscard]] glm::vec3 get_right() const { return right_; };
    [[nodiscard]] glm::vec3 get_camera_up() const { return camera_up_; };
    [[nodiscard]] glm::vec3 get_target() const { return target_; };

    // ---- Setters ----
    void set_position(const glm::vec3& position) { position_ = position; };
    void set_direction(const glm::vec3& direction) { direction_ = direction; };
    void set_target(const glm::vec3& target) { target_ = target; };
  };
} // STARBORN
