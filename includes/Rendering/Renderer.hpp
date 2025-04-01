/*
* Created by Sarthak Rai on 1/4/25.
*/
#pragma once

#include "glm/glm.hpp"
#include <GL/glew.h>
#include <string>
#include <memory>
#include <unordered_map>
#include "Camera.hpp"

class Shader;
class Mesh;

class Renderer {
public:
  explicit Renderer(const class Window& window);
  ~Renderer();

  // ---- Initialization ----
  void init();
  bool is_init() const { return initialized_; }

  // ---- Frame Control ----
  void begin_frame();
  void end_frame();
  void set_clear_color(const glm::vec4& color) const;

  // ---- Rendering ----
  void draw_mesh(const Mesh& mesh, const Shader& shader, const glm::mat4& model_matrix) const;
  void sumbit_camera(const Camera& camera);

  // ---- Resource Control ----
  void load_shader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path);
  Shader* get_shader(const std::string& name) const;

  // ---- State Control ----
  void set_viewport(int x, int y, int width, int height) const;
  void enable_depth_testing(bool enable) const;
  void enable_wireframe_mode(bool enable) const;

private:
  bool initialized_ = false;
  const class Window& window_;
  glm::mat4 projection_matrix_;
  glm::mat4 view_matrix_;

  std::unordered_map<std::string, std::unique_ptr<Shader>> shaders_;

  void setup_opengl_state();
  void check_gl_errors(const char* operation) const;
};
