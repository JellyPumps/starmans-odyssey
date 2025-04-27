/*
 * Created by Sarthak Rai on 01 Apr 2025.
 * Entry point
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.hpp"
#include "Input.hpp"
#include "TestScene.hpp"
#include "SceneManager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
  // ---- Create Window ----
  STARBORN::Window window(800, 600, "Starman's Odyssey");

  // ---- Input ----
  STARBORN::Input::get_instance().init(window.get_window());

  // ---- Setup Scene Manager ----
  auto &scene_manager = STARBORN::SceneManager::get_instance();
  scene_manager.add_scene("test_scene", std::make_unique<STARMAN::TestScene>(window));
  scene_manager.set_active_scene("test_scene");

  // ---- Disable VSync ----
  glfwSwapInterval(0);

  float last_frame = 0.0f;
  // ---- Main Loop ----
  while (!glfwWindowShouldClose(window.get_window())) {
    float ambient_strength = 1.0f;
    // ---- Time ----
    const auto current_frame = static_cast<float>(glfwGetTime());
    float delta_time = current_frame - last_frame;
    last_frame = current_frame;

    if (delta_time > 0.1f) delta_time = 0.1f;

    // ---- Update Input ----
    STARBORN::Input::get_instance().update();

    // ---- Update Scene ----
    scene_manager.update(delta_time);

    // ---- Render Scene ----
    scene_manager.render();

    // ---- Event Polling ----
    glfwSwapBuffers(window.get_window());
    glfwPollEvents();
  }

  scene_manager.cleanup();
  glfwTerminate();
  return 0;
}
