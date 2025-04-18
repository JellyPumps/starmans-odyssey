/*
 * Created by Sarthak Rai on 02 Apr 2025.
*/

#include "Mesh.hpp"

namespace STARBORN {
  // ---- Constructor & Destructor ----
  Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
  : vertices_(vertices), indices_(indices), use_indices_(!indices.empty()) {
    // ---- Generate Buffers ----
    generate_buffers();

    // ---- Bind Buffers ----
    glBindVertexArray(vertex_array_object_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_array_object_);
    glBufferData(GL_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(vertices_.size() * sizeof(float)),
      vertices_.data(), GL_STATIC_DRAW);

    if (use_indices_) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices_.size() * sizeof(unsigned int)),
        indices_.data(), GL_STATIC_DRAW);
    }

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
    // POSITION (3), COLOR (3), NORMAL (3), TEXCOORD (2)
    const GLsizei stride = 11 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, static_cast<void *>(0));
    glEnableVertexAttribArray(0);  // Position

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  // Color

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);  // Normal

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(9 * sizeof(float)));
    glEnableVertexAttribArray(3);  // TexCoord

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  // ---- Draw ----
  void Mesh::draw() const {
    glBindVertexArray(vertex_array_object_);

    if (use_indices_) {
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, nullptr);
    } else {
      glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_.size() / 8));
    }
  }

} // STARBORN