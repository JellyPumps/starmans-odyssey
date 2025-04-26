/*
 * Created by Sarthak Rai on 01 Apr 2025.
 * Entry point
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Input.hpp"
#include "Player.hpp"

int main() {
  // ---- Create Window ----
  STARBORN::Window window(800, 600, "Starman's Odyssey");

  // ---- Player ----
  auto player_position = glm::vec3(0.0f);
  STARMAN::Player player(player_position);
  std::shared_ptr<STARBORN::Camera> camera = player.get_camera();
  player.set_aspect_ratio(static_cast<float>(window.get_width())
    / static_cast<float>(window.get_height()));

  // ---- Input ----
  STARBORN::Input::get_instance().init(window.get_window());

  // ---- Shaders ----
  glEnable(GL_DEPTH_TEST);
  STARBORN::Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

  // ---- Load Model ----
  STARBORN::Model test_model("assets/models/test_models/tm_002.glb");

  // ---- Disable VSync ----
  glfwSwapInterval(0);

  float last_frame = 0.0f;
  // ---- Main Loop ----
  while (!glfwWindowShouldClose(window.get_window())) {
    float ambient_strength = 1.0f;
    // ---- Time ----
    auto current_frame = static_cast<float>(glfwGetTime());
    float delta_time = current_frame - last_frame;
    last_frame = current_frame;

    if (delta_time > 0.1f) delta_time = 0.1f;

    // ---- Input ----
    STARBORN::Input::get_instance().update();

    // ---- Update ----
    player.update(delta_time);

    // ---- Rendering ----
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    shader.set_vec3("viewPos", camera->get_position());
    shader.set_vec3("lightPos", glm::vec3(2.0f, 2.0f, 2.0f));
    shader.set_vec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    // ---- Transformation ----
    shader.set_mat4("projection", camera->get_projection_matrix());
    shader.set_mat4("view", player.get_camera()->get_view_matrix());

    // ---- Draw ----
    auto model = glm::mat4(1.0f);
    model = translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    shader.set_mat4("model", model);
    test_model.draw(shader);

    // ---- Event Polling ----
    glfwSwapBuffers(window.get_window());
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
