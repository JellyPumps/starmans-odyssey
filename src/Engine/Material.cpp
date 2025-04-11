/*
 * Created by Sarthak Rai on 11 Apr 2025.
*/

#include "Material.hpp"

namespace STARBORN {
  void Material::apply(const Shader& shader) const {
    // ---- Set Material Properties ----
    shader.set_vec4("material.baseColor", base_color);
    shader.set_float("material.metallic", metallic);
    shader.set_float("material.roughness", roughness);
    shader.set_vec3("material.emissiveFactor", emissive_factor);
    shader.set_float("material.alphaCutoff", alpha_cutoff);
    shader.set_int("material.alphaModeOpaque", alpha_mode == AlphaMode::OPAQUE ? 1 : 0);
    shader.set_int("material.alphaModeBlend", alpha_mode == AlphaMode::BLEND ? 1 : 0);
    shader.set_int("material.alphaModeMask", alpha_mode == AlphaMode::MASK ? 1 : 0);

    // ---- Set Texture Flags ----
    shader.set_int("material.hasBaseColorTexture", base_color_texture ? 1 : 0);
    shader.set_int("material.hasMetallicRoughnessTexture", metallic_roughness_texture ? 1 : 0);
    shader.set_int("material.hasNormalTexture", normal_texture ? 1 : 0);
    shader.set_int("material.hasOcclusionTexture", occlusion_texture ? 1 : 0);
    shader.set_int("material.hasEmissiveTexture", emissive_texture ? 1 : 0);

    // ---- Bind Textures -----
    if (base_color_texture) {
      glActiveTexture(GL_TEXTURE0);
      base_color_texture->bind();
      shader.set_int("material.baseColorTexture", 0);
    }

    if (metallic_roughness_texture) {
      glActiveTexture(GL_TEXTURE1);
      metallic_roughness_texture->bind();
      shader.set_int("material.metallicRoughnessTexture", 1);
    }

    if (normal_texture) {
      glActiveTexture(GL_TEXTURE2);
      normal_texture->bind();
      shader.set_int("material.normalTexture", 2);
    }

    if (occlusion_texture) {
      glActiveTexture(GL_TEXTURE3);
      occlusion_texture->bind();
      shader.set_int("material.occlusionTexture", 3);
    }

    if (emissive_texture) {
      glActiveTexture(GL_TEXTURE4);
      emissive_texture->bind();
      shader.set_int("material.emissiveTexture", 4);
    }
  }
}