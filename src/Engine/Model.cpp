/*
 * Created by Sarthak Rai on 10 Apr 2025.
*/

#include "Model.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace STARBORN {
  // ---- Load Model ----
  void Model::load_model(const std::string &path) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    fs::path file_path(path);
    directory_ = file_path.parent_path().string() + fs::path::preferred_separator;

    bool ret;

    if (path.substr(path.find_last_of('.') + 1 ) == "glb") ret = loader.LoadBinaryFromFile(&model_, &err, &warn, path);
    else ret = loader.LoadASCIIFromFile(&model_, &err, &warn, path);

    if (!warn.empty()) throw std::runtime_error(warn);
    if (!err.empty()) throw std::runtime_error(err);
    if (!ret) throw std::runtime_error("Failed to load glTF model: " + path);

    // ---- Process Scene ----
    int scene_idx = model_.defaultScene >= 0 ? model_.defaultScene : 0;
    const tinygltf::Scene &scene = model_.scenes[scene_idx];

    // ---- Process Nodes ----
    for (const int node_idx : scene.nodes) {
      const tinygltf::Node &node = model_.nodes[node_idx];
      process_node(node, model_);
    }
  }

  // ---- Process Node ----
  void Model::process_node(const tinygltf::Node & node, const tinygltf::Model &model) {
    if (node.mesh >= 0) {
      const tinygltf::Mesh &mesh = model.meshes[node.mesh];

      for (const auto &primitive : mesh.primitives) {
            meshes_.push_back(process_mesh(primitive, model));
      }
    }

    for (const int child_idx : node.children) {
      const tinygltf::Node &child = model.nodes[child_idx];
      process_node(child, model);
    }
  }

  // ---- Process Mesh ----
  Mesh Model::process_mesh(const tinygltf::Primitive &primitive, const tinygltf::Model &model) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // =============================================
    // 1. Process Vertex Data
    // =============================================

    // ---- Process Vertices ----
    const auto &pos_accessor = model.accessors[primitive.attributes.at("POSITION")];
    const auto &pos_view = model.bufferViews[pos_accessor.bufferView];
    const float *positions = reinterpret_cast<const float*>(&model.buffers[pos_view.buffer].data[pos_accessor.byteOffset + pos_view.byteOffset]);
    size_t vertex_count = pos_accessor.count;

    // ---- Process Normals ----
    const auto &norm_accessor = model.accessors[primitive.attributes.at("NORMAL")];
    const auto &norm_view = model.bufferViews[norm_accessor.bufferView];
    const float *normals = reinterpret_cast<const float*>(&model.buffers[norm_view.buffer].data[norm_accessor.byteOffset + norm_view.byteOffset]);

    // ---- Process Texture Coordinates ----
    const auto &tex_accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
    const auto &tex_view = model.bufferViews[tex_accessor.bufferView];
    const float *tex_coords = reinterpret_cast<const float*>(&model.buffers[tex_view.buffer].data[tex_accessor.byteOffset + tex_view.byteOffset]);

    // ---- Populate Vertices ----
    for (size_t i = 0; i < vertex_count; ++i) {
      Vertex vertex;

      // ---- Position ----
      vertex.position = glm::vec3(
        positions[3 * i + 0],
        positions[3 * i + 1],
        positions[3 * i + 2]
      );

      // ---- Normal ----
      vertex.normal = glm::vec3(
        normals[3 * i + 0],
        normals[3 * i + 1],
        normals[3 * i + 2]
      );

      // ---- Texture Coordinates ----
      vertex.tex_coords = tex_coords ? glm::vec2(
        tex_coords[2 * i + 0],
        tex_coords[2 * i + 1]
      ) : glm::vec2(0.0f, 0.0f);

      vertices.push_back(vertex);
    }

    // =============================================
    // 2. Process Index Data
    // =============================================

    if (primitive.indices >= 0) {
      auto &indices_accessor = model.accessors[primitive.indices];
      auto &indices_view = model.bufferViews[indices_accessor.bufferView];
      const void* indices_ptr = &model.buffers[indices_view.buffer].data[indices_accessor.byteOffset + indices_view.byteOffset];

      switch (indices_accessor.componentType) {
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
          const uint32_t *buf = static_cast<const uint32_t*>(indices_ptr);
          for (size_t i = 0; i < indices_accessor.count; i++) {
            indices.push_back(buf[i]);
          }
          break;
        }

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
          const uint16_t *buf = static_cast<const uint16_t*>(indices_ptr);
          for (size_t i = 0; i < indices_accessor.count; i++) {
            indices.push_back(buf[i]);
          }
          break;
        }

        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
          const uint8_t *buf = static_cast<const uint8_t*>(indices_ptr);
          for (size_t i = 0; i < indices_accessor.count; i++) {
            indices.push_back(buf[i]);
          }
          break;
        }

        default:
          throw std::runtime_error("Invalid indices type");
      }
    } else {
      for (size_t i = 0; i < vertex_count; i++) {
        indices.push_back(i);
      }
    }

    // =============================================
    // 3. Process Material Data
    // =============================================

    if (primitive.material >= 0) {
      const tinygltf::Material& material = model.materials[primitive.material];

      // ---- Base Color ----
      if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
        textures = load_material_textures(material, "albedo");
      }

      // ---- Metallic Roughness ----
      if (material.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
        auto metallicRoughnessTextures = load_material_textures(material, "metallicRoughness");
        textures.insert(textures.end(), metallicRoughnessTextures.begin(), metallicRoughnessTextures.end());
      }

      // ---- Normal Map ----
      if (material.normalTexture.index >= 0) {
        auto normalTextures = load_material_textures(material, "normal");
        textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());
      }

      // ---- Occlusion Map ----
      if (material.occlusionTexture.index >= 0) {
        auto occlusionTextures = load_material_textures(material, "occlusion");
        textures.insert(textures.end(), occlusionTextures.begin(), occlusionTextures.end());
      }

      // ---- Emissive Map ----
      if (material.emissiveTexture.index >= 0) {
        auto emissiveTextures = load_material_textures(material, "emissive");
        textures.insert(textures.end(), emissiveTextures.begin(), emissiveTextures.end());
      }
    }

    // =============================================
    // 4. Create and Return Mesh
    // =============================================

    return {vertices, indices, textures};
  }

  // ---- Load Material Textures ----
  std::vector<Texture> Model::load_material_textures(const tinygltf::Material &material, const std::string &type) {
    std::vector<Texture> textures;

    int tex_idx = -1;

    if (type == "baseColor") tex_idx = material.pbrMetallicRoughness.baseColorTexture.index;
    else if (type == "metallicRoughness") tex_idx = material.pbrMetallicRoughness.metallicRoughnessTexture.index;
    else if (type == "normal") tex_idx = material.normalTexture.index;
    else if (type == "occlusion") tex_idx = material.occlusionTexture.index;
    else if (type == "emissive") tex_idx = material.emissiveTexture.index;

    if (tex_idx >= 0) {
      auto &tex = model_.textures[tex_idx];
      auto &image = model_.images[tex.source];

      std::string gltf_type = "texture_diffuse";
      if (type == "normal") gltf_type = "texture_normal";
      else if (type == "metallicRoughness") gltf_type = "texture_metallicRoughness";
      else if (type == "occlusion") gltf_type = "texture_occlusion";
      else if (type == "emissive") gltf_type = "texture_emissive";

      textures.emplace_back(
        image.image,
        image.width,
        image.height,
        image.component,
        gltf_type,
        image.uri
      );
    }

    return textures;
  }

} // STARBORN