/*
 * Created by Sarthak Rai on 02 Apr 2025.
*/

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace STARBORN {

class Mesh {
private:
  // ---- Variables ----
  unsigned int vertex_buffer_object_{};
  unsigned int vertex_array_object_{};
  unsigned int element_buffer_object_{};

  std::vector<float> vertices_;
  std::vector<unsigned int> indices_;

  // ---- Private Methods ----
  void generate_buffers();
  static void link_vertex_attributes();
public:
  // ---- Constructor & Destructor ----
  Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
  ~Mesh();

  // ---- Methods ----
  void draw() const;
  [[nodiscard]] unsigned int get_vertex_buffer_object() const { return vertex_buffer_object_; }
  [[nodiscard]] unsigned int get_vertex_array_object() const { return vertex_array_object_; }
  [[nodiscard]] unsigned int get_element_buffer_object() const { return element_buffer_object_; }
};

} // STARBORN
