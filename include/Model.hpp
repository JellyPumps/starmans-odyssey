/*
 * Created by Sarthak Rai on 10 Apr 2025.
*/

#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>
#include <vector>
#include <tiny_gltf.h>

namespace STARBORN {

  class Model {
  private:
    // ---- Model Data ----
    std::vector<Mesh> meshes_;
    std::string directory_;
    tinygltf::Model model_;

    // ---- Private Methods ----
    void load_model(const std::string& path);
    void process_node(const tinygltf::Node& node, const tinygltf::Model& model);
    Mesh process_mesh(const tinygltf::Primitive& primitive, const tinygltf::Model& model);
    std::vector<Texture> load_material_textures(const tinygltf::Material& material,
      const std::string &type);
  public:
    Model(const std::string &path) {
      load_model(path);
    }

    void draw(Shader shader) {
      for (auto &mesh : meshes_) {
        mesh.draw(shader);
      }
    }
  };
} // STARBORN
