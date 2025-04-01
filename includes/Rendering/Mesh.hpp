/*
* Created by Sarthak Rai on 1/4/25.
*/

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

class Mesh {
public:
  // ---- Mesh Data ----
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
  ~Mesh();

  void draw() const;

private:
  // ---- Render Data ----
  unsigned int VAO, VBO, EBO;

  void setup_mesh();
};