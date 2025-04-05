/*
 * Created by Sarthak Rai on 01 Apr 2025.
 * Entry point
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Window.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Camera.hpp"


int main() {
  // ---- Create Window ----
  STARBORN::Window window(800, 600, "Starman's Odyssey");

  // ---- Shaders ----
  glEnable(GL_DEPTH_TEST);
  const STARBORN::Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

  // ---- Mesh ----
  const std::vector vertices = {
    // positions          // colors           // texture coords
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f
  };

  std::vector cube_positions = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3 (2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  const STARBORN::Mesh mesh(vertices);

  // ---- Textures ----
  const STARBORN::Texture texture("assets/textures/test_image_2.jpg");

  shader.use();
  STARBORN::Texture::opengl_sampler(shader);

  glfwSwapInterval(0);

  // ---- Camera ----
  auto camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
  auto camera_target = glm::vec3(0.0f, 0.0f, 0.0f);

  STARBORN::Camera camera(camera_position, camera_target, 45.0f,
    static_cast<float>(window.get_width()) / static_cast<float>(window.get_height()),
    0.1f, 100.0f);

  shader.set_mat4("projection", camera.get_projection_matrix());

  // ---- Main Loop ----
  while (!glfwWindowShouldClose(window.get_window())) {
    // ---- Input ----

    // ---- Rendering ----
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture.bind();
    shader.use();

    // ---- Transformation ----
    shader.set_mat4("view", camera.get_view_matrix());

    // ---- Draw ----
    for (unsigned int i = 0; i < cube_positions.size(); i++) {
      auto model = glm::mat4(1.0f);
      model = translate(model, cube_positions[i]);
      float angle = 20.0f * static_cast<float>(i);
      model = rotate(model, angle, camera.get_direction());
      shader.set_mat4("model", model);

      mesh.draw();
    }

    // ---- Event Polling ----
    glfwSwapBuffers(window.get_window());
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
