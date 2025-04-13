/*
 * Created by Sarthak Rai on 13 Apr 2025.
*/

#pragma once

#include <glm/glm.hpp>
#include "Shader.hpp"

namespace STARBORN {
  enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
  };

  class Light {
  protected:
    glm::vec3 colour_;
    float intensity_;
    bool casts_shadow_;
    LightType type_;
  public:
    // ---- Constructor & Destructor ----
    Light(LightType type, const glm::vec3 &colour = glm::vec3(1.0f),
      float intensity = 1.0f, bool casts_shadow = false);
    virtual ~Light() = default;

    // ---- Getters ----
    [[nodiscard]] glm::vec3 get_colour() const { return colour_; }
    [[nodiscard]] float get_intensity() const { return intensity_; }
    [[nodiscard]] bool does_casts_shadow() const { return casts_shadow_; }
    [[nodiscard]] LightType get_type() const { return type_; }

    // ---- Setters ----
    void set_colour(const glm::vec3 &colour) { colour_ = colour; }
    void set_intensity(const float intensity) { intensity_ = intensity; }
    void set_casts_shadow(const bool casts_shadow) { casts_shadow_ = casts_shadow; }
    void set_type(const LightType type) { type_ = type; }

    // ---- Virtuals ----
    virtual void prepare_shadow_map() = 0;
    virtual void bind(const Shader &shader, size_t light_index) const = 0;
  };
}