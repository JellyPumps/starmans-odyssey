/*
 * Created by Sarthak Rai on 02 Apr 2025.
*/

#pragma once

#define MAX_BONE_INFLUENCE 4

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
  glm::vec3 tangent;
  glm::vec3 bitangent;
  int m_bone_ids[MAX_BONE_INFLUENCE];
  float m_weights[MAX_BONE_INFLUENCE];
};

namespace STARBORN {
  class Mesh {
  private:
    // ---- Variables ----
    unsigned int VAO, VBO, EBO;

    // ---- Private Methods ----
    void setup_mesh();
  public:
    // ---- Variables ----
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;

    // ---- Constructor & Destructor ----
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    ~Mesh();

    // ---- Methods ----
    void draw(const Shader &shader) const;
};

} // STARBORN
