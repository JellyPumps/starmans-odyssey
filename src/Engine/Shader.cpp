/*
 * Created by Sarthak Rai on 02 Apr 2025.
*/

#include "Shader.hpp"

#include <bits/fs_fwd.h>

namespace STARBORN {

  // ---- Constructor & Destructor ----
  Shader::Shader(const char *vertex_path, const char *fragment_path) {
    // ---- Retrieve Vertex & Fragment Source Code ----
    std::string vertex_code = read_shader_file(vertex_path);
    std::string fragment_code = read_shader_file(fragment_path);

    // ---- Fallback Shaders ----
    if (vertex_code == SHADER_FAIL) {
      vertex_code = read_shader_file("../shaders/basic.vert");
      if (vertex_code == SHADER_FAIL) {
        std::cerr << "ERROR::SHADER::VERTEX::FALLBACK_FAILED" << std::endl;
      }
    }

    if (fragment_code == SHADER_FAIL) {
      fragment_code = read_shader_file("../shaders/basic.frag");
      if (fragment_code == SHADER_FAIL) {
        std::cerr << "ERROR::SHADER::FRAGMENT::FALLBACK_FAILED" << std::endl;
      }
    }

    const char* vshader = vertex_code.c_str();
    const char* fshader = fragment_code.c_str();

    // ---- Compile Shaders ----
    const unsigned int vertex = compile_shader(vshader, GL_VERTEX_SHADER);
    const unsigned int fragment = compile_shader(fshader, GL_FRAGMENT_SHADER);

    // ---- Shader Program ----
    create_shader_program(vertex, fragment);
  }

  Shader::~Shader() {
    // ---- Delete Shader Program ----
    glDeleteProgram(ID);
  }

  std::string Shader::read_shader_file(const char *file_path) const {
      std::string file_contents;
      std::ifstream shader_file;

      shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      try {
        shader_file.open(file_path);
        std::stringstream shader_stream;

        // ---- Read File's Buffer Contents ----
        shader_stream << shader_file.rdbuf();

        // ---- Close File Handles ----
        shader_file.close();

        // ---- Convert Stream Into String ----
        file_contents = shader_stream.str();
      } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ. USING FALLBACK" << std::endl;
        return SHADER_FAIL;
      }

      return file_contents;
    }

  unsigned int Shader::compile_shader(const char *shader_code,
                                      const GLenum shader_type) {
    int success;

    const unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code, nullptr);
    glCompileShader(shader);

    // ----- Compile Errors ----
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      char info_log[512];
      glGetShaderInfoLog(shader, 512, nullptr, info_log);
      throw std::runtime_error(info_log);
    }

    return shader;
  }

  void Shader::create_shader_program(const unsigned int vertex,
                                     const unsigned int fragment) {
    int success;

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      char info_log[512];
      glGetProgramInfoLog(ID, 512, nullptr, info_log);
      throw std::runtime_error(info_log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
} // STARBORN