/*
 * Created by Sarthak Rai on 10 Apr 2025.
*/

#include "Model.hpp"

namespace STARBORN {
  // ---- Draw ----
  void Model::draw(const Shader &shader) const {
    shader.set_mat4("model", transform_);
    for (const auto &mesh : meshes_) {
      // ---- Apply Material ----
      if (mesh->material_index >= 0 && mesh->material_index < materials_.size()) {
        materials_[mesh->material_index]->apply(shader);
      } else if (!textures_.empty()) {
        textures_[0]->bind();
      }

      mesh->draw();
    }
  }

  // ---- Model Loading ----
 std::shared_ptr<Model> ModelLoader::load_model(const std::string &path) {
    std::string err, warn;
    tinygltf::TinyGLTF loader;
    tinygltf::Model gltf_model;

    bool ret = false;
    if (path.ends_with(".glb"))
      ret = loader.LoadBinaryFromFile(&gltf_model, &err, &warn, path);
    else
      ret = loader.LoadASCIIFromFile(&gltf_model, &err, &warn, path);

    if (!warn.empty()) std::cerr << warn << std::endl;
    if (!err.empty()) std::cerr << err << std::endl;
    if (!ret) {
      std::cerr << "Failed to load glTF model: " << path << std::endl;
      return nullptr;
    }

    auto model = std::make_shared<Model>();

    // ---- Process Textures ----
    std::vector<std::shared_ptr<Texture>> texture_cache;
    for (const auto &image : gltf_model.images) {
      auto texture_ptr = std::make_shared<Texture>(
        image.image.data(),
        image.width,
        image.height,
        image.component
      );
      texture_cache.push_back(texture_ptr);
    }

    // ---- Process Materials ----
    for (const auto &material : gltf_model.materials) {
      auto mat = std::make_shared<Material>();

      // Process base material properties
      const auto &pbr = material.pbrMetallicRoughness;

      // Base color
      mat->base_color = glm::vec4(
        pbr.baseColorFactor[0],
        pbr.baseColorFactor[1],
        pbr.baseColorFactor[2],
        pbr.baseColorFactor[3]
      );

      // Metallic/roughness
      mat->metallic = static_cast<float>(pbr.metallicFactor);
      mat->roughness = static_cast<float>(pbr.roughnessFactor);

      // Emissive factor
      if (material.emissiveFactor.size() >= 3) {
        mat->emissive_factor = glm::vec3(
          material.emissiveFactor[0],
          material.emissiveFactor[1],
          material.emissiveFactor[2]
        );
      }

      // Alpha mode
      if (material.alphaMode == "BLEND") {
        mat->alpha_mode = Material::AlphaMode::BLEND;
      } else if (material.alphaMode == "MASK") {
        mat->alpha_mode = Material::AlphaMode::MASK;
        mat->alpha_cutoff = static_cast<float>(material.alphaCutoff);
      }

      mat->double_sided = material.doubleSided;

      // Process textures
      if (pbr.baseColorTexture.index >= 0) {
        const auto &texture = gltf_model.textures[pbr.baseColorTexture.index];
        if (texture.source >= 0 && texture.source < texture_cache.size()) {
          mat->base_color_texture = texture_cache[texture.source];
        }
      }

      if (pbr.metallicRoughnessTexture.index >= 0) {
        const auto &texture = gltf_model.textures[pbr.metallicRoughnessTexture.index];
        if (texture.source >= 0 && texture.source < texture_cache.size()) {
          mat->metallic_roughness_texture = texture_cache[texture.source];
        }
      }

      if (material.normalTexture.index >= 0) {
        const auto &texture = gltf_model.textures[material.normalTexture.index];
        if (texture.source >= 0 && texture.source < texture_cache.size()) {
          mat->normal_texture = texture_cache[texture.source];
        }
      }

      if (material.occlusionTexture.index >= 0) {
        const auto &texture = gltf_model.textures[material.occlusionTexture.index];
        if (texture.source >= 0 && texture.source < texture_cache.size()) {
          mat->occlusion_texture = texture_cache[texture.source];
        }
      }

      if (material.emissiveTexture.index >= 0) {
        const auto &texture = gltf_model.textures[material.emissiveTexture.index];
        if (texture.source >= 0 && texture.source < texture_cache.size()) {
          mat->emissive_texture = texture_cache[texture.source];
        }
      }

      model->materials_.push_back(mat);
    }

    // ---- Process Meshes ----
    for (const auto &mesh : gltf_model.meshes) {
      for (const auto &primitive : mesh.primitives) {
        if (auto processed_mesh = process_mesh(primitive, gltf_model))
          model->meshes_.push_back(processed_mesh);
      }
    }

    // ---- Compatibility ----
    if (!texture_cache.empty()) {
      model->textures_ = texture_cache;
    }

    return model;
  }

  std::shared_ptr<Mesh> ModelLoader::process_mesh(const tinygltf::Primitive &primitive,
                                                const tinygltf::Model &gltf_model) {
    if (primitive.indices < 0 || primitive.attributes.find("POSITION") == primitive.attributes.end()) {
      return nullptr;
    }

    // ---- Get Accessors ----
    const auto &position_accessor = gltf_model.accessors[primitive.attributes.at("POSITION")];
    const auto &position_view = gltf_model.bufferViews[position_accessor.bufferView];
    const auto &position_buffer = gltf_model.buffers[position_view.buffer];

    // ---- Extract Vertex Data ----
    std::vector<float> vertices;

    // ---- Process Positions ----
    const auto *positions = reinterpret_cast<const float *>(
        &position_buffer.data[position_view.byteOffset + position_accessor.byteOffset]);

    const size_t vertex_count = position_accessor.count;

    // ---- Check For Normals ----
    const bool has_normals = primitive.attributes.find("NORMAL") != primitive.attributes.end();
    const float *normals = nullptr;

    if (has_normals) {
      const auto &normal_accessor = gltf_model.accessors[primitive.attributes.at("NORMAL")];
      const auto &normal_view = gltf_model.bufferViews[normal_accessor.bufferView];
      const auto &normal_buffer = gltf_model.buffers[normal_view.buffer];

      normals = reinterpret_cast<const float*>(
        &normal_buffer.data[normal_view.byteOffset + normal_accessor.byteOffset]
      );
    }

    // ---- Check For Vertex Colors ----
    const bool has_colors = primitive.attributes.find("COLOR_0") != primitive.attributes.end();
    const float *colors = nullptr;

    if (has_colors) {
      const auto &color_accessor = gltf_model.accessors[primitive.attributes.at("COLOR_0")];
      const auto &color_view = gltf_model.bufferViews[color_accessor.bufferView];
      const auto &color_buffer = gltf_model.buffers[color_view.buffer];

      colors = reinterpret_cast<const float*>(
        &color_buffer.data[color_view.byteOffset + color_accessor.byteOffset]
      );
    }

    // ---- Check For Texture Coordinates ----
    const bool has_tex_coords = primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end();
    const float *tex_coords = nullptr;

    if (has_tex_coords) {
      const auto &tex_coord_accessor = gltf_model.accessors[primitive.attributes.at("TEXCOORD_0")];
      const auto &tex_coord_view = gltf_model.bufferViews[tex_coord_accessor.bufferView];
      const auto &tex_coord_buffer = gltf_model.buffers[tex_coord_view.buffer];

      tex_coords = reinterpret_cast<const float*>(
        &tex_coord_buffer.data[tex_coord_view.byteOffset + tex_coord_accessor.byteOffset]
      );
    }

    // ---- Create Vertex Array ----
    for (size_t i = 0; i < vertex_count; i++) {
      // ---- Position ----
      vertices.push_back(positions[i * 3 + 0]);
      vertices.push_back(positions[i * 3 + 1]);
      vertices.push_back(positions[i * 3 + 2]);

      // ---- Color ----
      if (has_colors) {
        vertices.push_back(colors[i * 4 + 0]);
        vertices.push_back(colors[i * 4 + 1]);
        vertices.push_back(colors[i * 4 + 2]);
      } else {
        vertices.push_back(1.0f);
        vertices.push_back(1.0f);
        vertices.push_back(1.0f);
      }

      // ---- Normal ----
      if (has_normals) {
        vertices.push_back(normals[i * 3 + 0]);
        vertices.push_back(normals[i * 3 + 1]);
        vertices.push_back(normals[i * 3 + 2]);
      } else {
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);
      }

      // ---- Texture Coordinates ----
      if (has_tex_coords) {
        vertices.push_back(tex_coords[i * 2 + 0]);
        vertices.push_back(tex_coords[i * 2 + 1]);
      } else {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }
    }

    // Process indices if present
    std::vector<unsigned int> indices;
    if (primitive.indices >= 0) {
      const auto &index_accessor = gltf_model.accessors[primitive.indices];
      const auto &index_view = gltf_model.bufferViews[index_accessor.bufferView];
      const auto &index_buffer = gltf_model.buffers[index_view.buffer];

      const void* index_data = &index_buffer.data[index_view.byteOffset + index_accessor.byteOffset];
      const size_t index_count = index_accessor.count;

      indices.resize(index_count);

      // Handle different index component types
      if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
        const auto* shortIndices = static_cast<const uint16_t*>(index_data);
        for (size_t i = 0; i < index_count; i++) {
          indices[i] = static_cast<unsigned int>(shortIndices[i]);
        }
      } else if (index_accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
        memcpy(indices.data(), index_data, index_count * sizeof(unsigned int));
      }
    }

    auto mesh = std::make_shared<Mesh>(vertices, indices);
    mesh->material_index = primitive.material;
    return mesh;
  }
} // STARBORN