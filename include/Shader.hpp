/*
 * Created by Sarthak Rai on 02 Apr 2025.
*/

#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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
};

} // STARBORN
