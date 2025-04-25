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
    unsigned int VBO, EBO;

    // ---- Private Methods ----
    void setup_mesh() {
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

      // ---- Positions ----
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

      // ---- Normals ----
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

      // --- Texture Coordinates ----
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

      // ---- Tangent ----
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

      // ---- Bitangent ----
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

      // ---- Bone IDs ----
      glEnableVertexAttribArray(5);
      glVertexAttribIPointer(5, MAX_BONE_INFLUENCE, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_bone_ids));

      // ---- Bone Weights ----
      glEnableVertexAttribArray(6);
      glVertexAttribPointer(6, MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_weights));

      glBindVertexArray(0);
    }
  public:
    // ---- Variables ----
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;
    unsigned int VAO;

    // ---- Constructor & Destructor ----
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
      this->vertices_ = vertices;
      this->indices_ = indices;
      this->textures_ = textures;

      setup_mesh();
    }

    // ---- Methods ----
    void draw(Shader &shader) {
      unsigned int diffuse_nr = 1;
      unsigned int specular_nr = 1;
      unsigned int normal_nr = 1;
      unsigned int height_nr = 1;

      for (unsigned int i = 0; i < textures_.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = textures_[i].get_type();

        if (name == "texture_diffuse") {
          number = std::to_string(diffuse_nr++);
        } else if (name == "texture_specular") {
          number = std::to_string(specular_nr++);
        } else if (name == "texture_normal") {
          number = std::to_string(normal_nr++);
        } else if (name == "texture_height") {
          number = std::to_string(height_nr++);
        }

        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        textures_[i].bind();
      }
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_.size()), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      glActiveTexture(GL_TEXTURE0);
    }
};

} // STARBORN
