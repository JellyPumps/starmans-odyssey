/*
 * Created by Sarthak Rai on 11 Apr 2025.
*/

#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.hpp"
#include "Shader.hpp"

namespace STARBORN {
  class Material {
  public:
    // PBR properties
    glm::vec4 base_color{1.0f};
    float metallic{1.0f};
    float roughness{1.0f};

    // Textures
    std::shared_ptr<Texture> base_color_texture;
    std::shared_ptr<Texture> metallic_roughness_texture;
    std::shared_ptr<Texture> normal_texture;
    std::shared_ptr<Texture> occlusion_texture;
    std::shared_ptr<Texture> emissive_texture;

    glm::vec3 emissive_factor{0.0f};
    bool double_sided{false};

    // Alpha properties
    enum class AlphaMode {
      OPAQUE,
      MASK,
      BLEND
    };

    AlphaMode alpha_mode{AlphaMode::OPAQUE};
    float alpha_cutoff{0.5f};

    void apply(const Shader& shader) const;
  };
}