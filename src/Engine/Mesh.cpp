/*
 * Created by Sarthak Rai on 02 Apr 2025.
*/

#include "Mesh.hpp"
#include <utility>

namespace STARBORN {
  // ---- Constructor & Destructor ----
   Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
     this->vertices_ = std::move(vertices);
     this->indices_ = std::move(indices);
     this->textures_ = std::move(textures);

     setup_mesh();
   }

  // ---- Setup Mesh ----
  void Mesh::setup_mesh() {
     // ---- Generate Buffers ----
     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &VBO);
     glGenBuffers(1, &EBO);

     // ---- Bind Buffers ----
     glBindVertexArray(VAO);
     glBindBuffer(GL_ARRAY_BUFFER, VBO);

     // ---- Set Buffer Data ----
     glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

     // ---- Vertex Positions ----
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

     // ---- Vertex Normals ----
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

     // ---- Vertex Texture Coordinates ----
     glEnableVertexAttribArray(2);
     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, TexCoords)));
  }

  // ---- Draw ----
  void Mesh::draw(Shader &shader) {
     unsigned int diffuseNr = 0;
     unsigned int specularNr = 0;

     for (unsigned int i = 0; i < textures_.size(); i++) {
       glActiveTexture(GL_TEXTURE0 + i);

       std::string number;
       std::string name = textures_[i].get_type();

       if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
       else if (name == "texture_specular") number = std::to_string(specularNr++);

       shader.set_int(("material." + name + number).c_str(), i);
       textures_[i].bind();
     }

     glActiveTexture(GL_TEXTURE0);

     // ---- Draw Mesh ----
     glBindVertexArray(VAO);
     glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_.size()), GL_UNSIGNED_INT, nullptr);
     glBindVertexArray(0);
  }
} // STARBORN