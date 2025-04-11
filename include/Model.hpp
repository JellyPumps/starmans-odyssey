/*
 * Created by Sarthak Rai on 10 Apr 2025.
*/

#pragma once

#include <tiny_gltf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include "Mesh.hpp"
#include "Texture.hpp"
#include <string>

namespace STARBORN {

class Model {
public:
  std::vector<std::shared_ptr<Mesh>> meshes_;
  std::vector<std::shared_ptr<Texture>> textures_;
  glm::mat4 transform_{1.0f};

  // ---- Draw ----
  void draw(const Shader &shader) const;
};

class ModelLoader {
private:
  static std::shared_ptr<Mesh> process_mesh(const tinygltf::Primitive &primitive,
    const tinygltf::Model &gltf_model);
public:
  static std::shared_ptr<Model> load_model(const std::string &path);
};
} // STARBORN
