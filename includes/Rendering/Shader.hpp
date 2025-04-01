/*
* Created by Sarthak Rai on 1/4/25.
*/

#pragma once
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
  Shader(const char* vertex_path, const char* fragment_path);
  ~Shader();

  void use() const;

  // ---- Uniform Setters ----
  void set_uniform(const std::string& name, bool value) const;
  void set_uniform(const std::string& name, int value) const;
  void set_uniform(const std::string& name, float value) const;
  void set_uniform(const std::string& name, const glm::vec2& value) const;
  void set_uniform(const std::string& name, const glm::vec3& value) const;
  void set_uniform(const std::string& name, const glm::vec4& value) const;
  void set_uniform(const std::string& name, const glm::mat4& value) const;

private:
  unsigned int ID;
  std::string load_shader_file(const char* path) const;
  void check_compile_errors(unsigned int shader, const std::string& type) const;
};
