/*
 * Created by Sarthak Rai on 02 Apr 2025.
*/

#pragma once

#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

namespace STARBORN {

class Shader {
private:
  std::string read_shader_file(const char *file_path) const;
  static unsigned int compile_shader(const char *shader_code, GLenum shader_type);
  void create_shader_program(unsigned int vertex, unsigned int fragment);
  static bool ends_with(const std::string& str, const std::string& suffix);
public:
  // ---- Variables ----
  unsigned int ID{};
  std::string SHADER_FAIL = "FAIL";

  // ---- Constructor & Destructor ----
  Shader(const char* vertex_path, const char* fragment_path);
  ~Shader();

  // --- Activate Shader ----
  void use() const { glUseProgram(ID); };

  // ---- Set Uniforms ----
  void set_bool(const std::string& name, const bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
  };
  void set_int(const std::string& name, const int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  };
  void set_float(const std::string& name, const float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  };

  void set_vec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }

  void set_vec2(const std::string &name, const float x, const float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }

  void set_vec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }

  void set_vec3(const std::string &name, const float x, const float y, const float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }

  void set_vec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }

  void set_vec4(const std::string &name, const float x, const float y, const float z, const float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }

  void set_mat2(const std::string &name, const glm::mat2 &value) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
  }

  void set_mat3(const std::string &name, const glm::mat3 &value) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
  }

  void set_mat4(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
  }
};

} // STARBORN
