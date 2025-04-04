/*
 * Created by Sarthak Rai on 02 Apr 2025.
*/

#include "Mesh.hpp"

namespace STARBORN {
  // ---- Constructor & Destructor ----
  Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
  : vertices_(vertices), indices_(indices) {
    // ---- Generate Buffers ----
    generate_buffers();

    // ---- Bind Buffers ----
    glBindVertexArray(vertex_array_object_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array_object_);
    glBufferData(GL_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(vertices_.size() * sizeof(float)),
      vertices_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(indices_.size() * sizeof(unsigned int)),
      indices_.data(), GL_STATIC_DRAW);

    // ---- Link Vertex Attributes ----
    link_vertex_attributes();
  }

  Mesh::~Mesh() {
    glDeleteBuffers(1, &vertex_buffer_object_);
    glDeleteBuffers(1, &element_buffer_object_);
    glDeleteVertexArrays(1, &vertex_array_object_);
  }

  // ---- Private Methods ----
  void Mesh::generate_buffers() {
    glGenVertexArrays(1, &vertex_array_object_);
    glGenBuffers(1, &vertex_buffer_object_);
    glGenBuffers(1, &element_buffer_object_);
  }

  void Mesh::link_vertex_attributes() {
    // ---- Position Attribute ----
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    // ---- Color Attribute ----
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ---- Texture Coordinate Attribute ----
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  // ---- Draw ----
  void Mesh::draw() const {
    glBindVertexArray(vertex_array_object_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  }

} // STARBORN