/*
 * Created by Sarthak Rai on 26 Apr 2025.
*/

#include "ScreenQuad.hpp"

namespace STARBORN {
  namespace ScreenQuad {
    unsigned int VAO = 0;

    void init() {
      if (VAO != 0) return;

      unsigned int VBO;
      float quad_vertices[] = {
        // Positions      // Texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      };

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

      glBindVertexArray(0);
      glDeleteBuffers(1, &VBO);
    }

    void draw() {
      if (VAO == 0) init();
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      glBindVertexArray(0);
    }

    void cleanup() {
      if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
      }
    }
  }
}