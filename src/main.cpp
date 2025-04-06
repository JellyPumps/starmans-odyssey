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
#include "Input.hpp"

glm::vec3 screen_to_world(const glm::vec2& screen_position, const glm::mat4& projection, const glm::mat4& view,
                          const int width, const int height) {
  const glm::vec4 ndc(
      (2.0f * screen_position.x) / static_cast<float>(width) - 1.0f,
      1.0f - (2.0f * screen_position.y) / static_cast<float>(height), 0.1f,
      1.0f);

  const glm::mat4 inverse_viewp = glm::inverse(projection * view);
  const glm::vec4 world_position = inverse_viewp * ndc;

  return glm::vec3(world_position) / world_position.w;
}

int main() {
  // ---- Create Window ----
  STARBORN::Window window(800, 600, "Starman's Odyssey");

  // ---- Mouse Settings ----
  auto last_x = static_cast<float>(window.get_width()) / 2.0f;
  auto last_y = static_cast<float>(window.get_height()) / 2.0f;
  float yaw = -90.0f;
  float pitch = 0.0f;
  float sensitivity = 0.1f;
  bool first = true;

  bool cursor_locked = true;

  // ---- Input ----
  STARBORN::Input::get_instance().init(window.get_window());

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

  // ---- Camera ----
  auto camera_position = glm::vec3(0.0f, 0.0f, 3.0f);
  auto camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
  float near_plane = 0.1f;
  float far_plane = 100.0f;

  STARBORN::Camera camera(camera_position, camera_target, 45.0f,
    static_cast<float>(window.get_width()) / static_cast<float>(window.get_height()),
    near_plane, far_plane);

  shader.set_mat4("projection", camera.get_projection_matrix());

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

    // ---- Camera Controls ----
    camera_position = camera.get_position();
    glm::vec3 camera_direction = camera.get_direction();
    glm::vec3 camera_right = camera.get_right();
    glm::vec3 camera_up = camera.get_camera_up();

    glm::vec2 cursor_position = STARBORN::Input::get_instance().get_cursor_position();

    // ---- Mouse Movement ----
    if (first) {
      last_x = cursor_position.x;
      last_y = cursor_position.y;
      first = false;
    }

    float x_offset = cursor_position.x - last_x;
    float y_offset = last_y - cursor_position.y; // Reversed: y ranges bottom to top
    last_x = cursor_position.x;
    last_y = cursor_position.y;

    // ---- Apply Sensitivity ----
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    // ---- Update Yaw and Pitch ----
    if (cursor_locked) {
      yaw += x_offset;
      pitch += y_offset;

      // ---- Constrain Pitch ----
      if (pitch > 89.0f) pitch = 89.0f;
      if (pitch < -89.0f) pitch = -89.0f;

      // ---- Calculate Direction ----
      glm::vec3 direction;
      direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      direction.y = sin(glm::radians(pitch));
      direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      camera_direction = glm::normalize(direction);

      // ---- Update Camera ----
      camera_target = camera_position + camera_direction;

      camera_right = glm::normalize(cross(direction, camera.get_up()));
      camera_up = glm::normalize(cross(camera_right, direction));
    }

    // ---- Movement ----
    const float movement_speed = 2.5f * delta_time;
    glm::vec3 movement_direction(0.0f);

    if (STARBORN::Input::get_instance().is_key_down(GLFW_KEY_W))
      movement_direction += camera_direction;
    if (STARBORN::Input::get_instance().is_key_down(GLFW_KEY_S))
      movement_direction -= camera_direction;
    if (STARBORN::Input::get_instance().is_key_down(GLFW_KEY_A))
      movement_direction -= camera_right;
    if (STARBORN::Input::get_instance().is_key_down(GLFW_KEY_D))
      movement_direction += camera_right;
    if (STARBORN::Input::get_instance().is_key_down(GLFW_KEY_SPACE))
      movement_direction += camera.get_up();
    if (STARBORN::Input::get_instance().is_key_down(GLFW_KEY_LEFT_SHIFT))
      movement_direction -= camera.get_up();

    // ---- Normalize ----
    if (length(movement_direction) > 0.0f)
      movement_direction = normalize(movement_direction);

    camera_position += movement_direction * movement_speed;

    // ---- Update Camera ----
    camera.set_position(camera_position);
    camera.set_target(camera_target);

    // ---- Toggle cursor lock ----
    if (STARBORN::Input::get_instance().is_key_down(GLFW_KEY_ESCAPE)) {
      cursor_locked = !cursor_locked;
      glfwSetInputMode(window.get_window(),
                      GLFW_CURSOR,
                      cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

      if (cursor_locked) first = true;
    }

    // ---- Rendering ----
    glClearColor(0.0f, 181.0f / 255.0f, 226.0f / 255.0f, 1.0f);
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
