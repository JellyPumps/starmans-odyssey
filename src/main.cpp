/*
 * Created by Sarthak Rai on 01 Apr 2025.
 * Entry point
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
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
  const STARBORN::Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

  // ---- Load Model ----
  std::shared_ptr<STARBORN::Model> model = STARBORN::ModelLoader::load_model(
    "assets/models/test_models/test_surface.glb");

  float last_frame = 0.0f;
  // ---- Main Loop ----
  while (!glfwWindowShouldClose(window.get_window())) {
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
    glClearColor(40.0f / 255.0f, 30.0f / 255.0f, 60.0f / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    // ---- Transformation ----
    shader.set_mat4("projection", camera->get_projection_matrix());
    shader.set_mat4("view", player.get_camera()->get_view_matrix());

    // ---- Lighting ----
    std::vector<glm::vec3> lightPositions = {
      glm::vec3(1.2f, 1.0f, 2.0f),
      glm::vec3(-1.2f, 1.0f, 2.0f)
    };
    std::vector<glm::vec3> lightColors = {
      glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(0.8f, 0.8f, 0.8f)
    };

    for (size_t i = 0; i < lightPositions.size(); ++i) {
      shader.set_vec3("uLightPositions[" + std::to_string(i) + "]", lightPositions[i]);
      shader.set_vec3("uLightColors[" + std::to_string(i) + "]", lightColors[i]);
    }
    shader.set_int("uLightCount", static_cast<int>(lightPositions.size()));

    // ---- Draw ----
    glm::mat4 model_matrix = glm::mat4(1.0f);
    shader.set_mat4("model", model_matrix);
    model->draw(shader);

    // ---- Event Polling ----
    glfwSwapBuffers(window.get_window());
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
