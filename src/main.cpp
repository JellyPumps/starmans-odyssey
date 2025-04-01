/*
 * Created by Sarthak Rai on 01 Apr 2025.
 * Entry point
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.hpp"


int main() {
  // ---- Create Window ----
  STARBORN::Window window(800, 600, "Starman's Odyssey");

  // ---- Shaders ----
  const char *vertex_shader_source = "#version 330 core \n"
                                     "layout (location = 0) in vec3 aPos; \n"
                                     "void main() {\n"
                                     "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
                                     "}\0";

  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
  glCompileShader(vertex_shader);

  const char *fragment_shader_source = "#version 330 core \n"
                                       "out vec4 FragColor; \n"
                                       "void main() {\n"
                                       "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
                                       "}\0";

  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
  glCompileShader(fragment_shader);

  // ---- Shader Program ----
  unsigned int shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glUseProgram(shader_program);

  // ---- Delete Shaders ----
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // ---- Triangle ----
  constexpr float vertices[] = {
    -0.5f, -0.5f, 0.0f, // Bottom Left
     0.5f, -0.5f, 0.0f, // Bottom Right
     0.0f,  0.5f, 0.0f  // Top
  };

  unsigned int indices[] = {
    0, 1, 3
  };

  unsigned int vertex_buffer_object;
  unsigned int vertex_array_object;
  unsigned int element_buffer_object;

  glGenVertexArrays(1, &vertex_array_object);
  glGenBuffers(1, &vertex_buffer_object);
  glGenBuffers(1, &element_buffer_object);

  glBindVertexArray(vertex_array_object);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // ---- Linking Vertex Attributes ----
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // ---- Main Loop ----
  while (!glfwWindowShouldClose(window.get_window())) {
    // ---- Input ----

    // ---- Rendering ----
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vertex_array_object);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // ---- Event Polling ----
    glfwSwapBuffers(window.get_window());
    glfwPollEvents();
  }

  // ---- Cleanup ----
  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return 0;
}
