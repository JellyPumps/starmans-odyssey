/*
 * Created by Sarthak Rai on 02 Apr 2025.
*/

#pragma once

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
  glm::vec3 Normal;
  glm::vec2 TexCoords;
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
    void draw(Shader &shader);
};

} // STARBORN
