/*
* Created by Sarthak Rai on 1/4/25.
*/

#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

class Mesh {
public:
  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
  ~Mesh();

  void draw() const;

private:
  unsigned int VAO, VBO, EBO;
  size_t index_count;
};