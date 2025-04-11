/*
 * Created by Sarthak Rai on 11 Apr 2025.
*/

#include "Material.hpp"

namespace STARBORN {
  void Material::apply(const Shader& shader) const {
    // Set material properties
    shader.set_vec3("uBaseColor", base_color);
    shader.set_float("uMetallic", metallic);
    shader.set_float("uRoughness", roughness);
    shader.set_vec3("uEmissive", emissive_factor);
    shader.set_float("uOcclusionStrength", 1.0f); // Adjust as needed

    // Set texture flags
    shader.set_bool("uUseAlbedoMap", base_color_texture != nullptr);
    shader.set_bool("uUseMetallicRoughnessMap", metallic_roughness_texture != nullptr);
    shader.set_bool("uUseNormalMap", normal_texture != nullptr);
    shader.set_bool("uUseOcclusionMap", occlusion_texture != nullptr);
    shader.set_bool("uUseEmissiveMap", emissive_texture != nullptr);

    // Bind textures with proper units
    if (base_color_texture) {
      glActiveTexture(GL_TEXTURE0);
      base_color_texture->bind();
      shader.set_int("uAlbedoMap", 0);
    }

    if (metallic_roughness_texture) {
      glActiveTexture(GL_TEXTURE1);
      metallic_roughness_texture->bind();
      shader.set_int("uMetallicRoughnessMap", 1);
    }

    if (normal_texture) {
      glActiveTexture(GL_TEXTURE2);
      normal_texture->bind();
      shader.set_int("uNormalMap", 2);
    }

    if (occlusion_texture) {
      glActiveTexture(GL_TEXTURE3);
      occlusion_texture->bind();
      shader.set_int("uOcclusionMap", 3);
    }

    if (emissive_texture) {
      glActiveTexture(GL_TEXTURE4);
      emissive_texture->bind();
      shader.set_int("uEmissiveMap", 4);
    }
  }
}