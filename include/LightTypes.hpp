/*
 * Created by Sarthak Rai on 13 Apr 2025.
*/

#pragma once

#include "Light.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace STARBORN {
  class DirectionalLight final : public Light {
  private:
    glm::vec3 direction_;
    glm::mat4 light_space_matrix_{};

  public:
    explicit DirectionalLight(const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f),
                    const glm::vec3& colour = glm::vec3(1.0f),
                    float intensity = 1.0f,
                    bool casts_shadow = false)
        : Light(LightType::DIRECTIONAL, colour, intensity, casts_shadow),
          direction_(glm::normalize(direction)) {}

    // ---- Getters ----
    [[nodiscard]] glm::vec3 get_direction() const { return direction_; }
    void set_direction(const glm::vec3& direction) { direction_ = glm::normalize(direction); }
    [[nodiscard]] glm::mat4 get_light_space_matrix() const { return light_space_matrix_; }

    void prepare_shadow_map() override;
    void bind(const Shader& shader, size_t light_index) const override;
  };

  class PointLight final : public Light {
  private:
    glm::vec3 position_;
    float radius_;  // Light influence radius

    // ---- Attenuation factors ----
    float constant_;
    float linear_;
    float quadratic_;

  public:
    explicit PointLight(const glm::vec3& position = glm::vec3(0.0f),
              const glm::vec3& colour = glm::vec3(1.0f),
              float intensity = 1.0f,
              bool casts_shadow = false,
              float radius = 50.0f)
        : Light(LightType::POINT, colour, intensity, casts_shadow),
          position_(position), radius_(radius),
          constant_(1.0f), linear_(0.09f), quadratic_(0.032f) {}

    // ---- Getters ----
    [[nodiscard]] glm::vec3 get_position() const { return position_; }
    [[nodiscard]] float get_radius() const { return radius_; }
    [[nodiscard]] float get_constant() const { return constant_; }
    [[nodiscard]] float get_linear() const { return linear_; }
    [[nodiscard]] float get_quadratic() const { return quadratic_; }

    // ---- Setters ----
    void set_position(const glm::vec3& position) { position_ = position; }
    void set_radius(float radius) { radius_ = radius; }
    void set_attenuation(float constant, float linear, float quadratic) {
      constant_ = constant;
      linear_ = linear;
      quadratic_ = quadratic;
    }

    void prepare_shadow_map() override;
    void bind(const Shader& shader, size_t light_index) const override;
  };

  class SpotLight final : public Light {
  private:
    glm::vec3 position_;
    glm::vec3 direction_;

    float inner_cutoff_;  // Cosine of inner cutoff angle
    float outer_cutoff_;  // Cosine of outer cutoff angle

    // ---- Attenuation factors ----
    float constant_;
    float linear_;
    float quadratic_;

  public:
    explicit SpotLight(const glm::vec3& position = glm::vec3(0.0f),
             const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f),
             const glm::vec3& colour = glm::vec3(1.0f),
             float intensity = 1.0f,
             bool casts_shadow = false,
             float inner_cutoff_degrees = 12.5f,
             float outer_cutoff_degrees = 17.5f)
        : Light(LightType::SPOT, colour, intensity, casts_shadow),
          position_(position), direction_(glm::normalize(direction)),
          inner_cutoff_(glm::cos(glm::radians(inner_cutoff_degrees))),
          outer_cutoff_(glm::cos(glm::radians(outer_cutoff_degrees))),
          constant_(1.0f), linear_(0.09f), quadratic_(0.032f) {}

    // ---- Getters ----
    [[nodiscard]] glm::vec3 get_position() const { return position_; }
    [[nodiscard]] glm::vec3 get_direction() const { return direction_; }
    [[nodiscard]] float get_inner_cutoff() const { return inner_cutoff_; }
    [[nodiscard]] float get_outer_cutoff() const { return outer_cutoff_; }
    [[nodiscard]] float get_constant() const { return constant_; }
    [[nodiscard]] float get_linear() const { return linear_; }
    [[nodiscard]] float get_quadratic() const { return quadratic_; }

    // ---- Setters ----
    void set_position(const glm::vec3& position) { position_ = position; }
    void set_direction(const glm::vec3& direction) { direction_ = glm::normalize(direction); }
    void set_cutoff_angles(float inner_degrees, float outer_degrees) {
      inner_cutoff_ = glm::cos(glm::radians(inner_degrees));
      outer_cutoff_ = glm::cos(glm::radians(outer_degrees));
    }
    void set_attenuation(float constant, float linear, float quadratic) {
      constant_ = constant;
      linear_ = linear;
      quadratic_ = quadratic;
    }

    void prepare_shadow_map() override;
    void bind(const Shader& shader, size_t light_index) const override;
  };
}