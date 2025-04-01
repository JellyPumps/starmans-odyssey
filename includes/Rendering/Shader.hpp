/*
* Created by Sarthak Rai on 1/4/25.
*/

#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <unordered_map>

class Shader {
public:
  static void GLAPIENTRY opengl_debug_callback(
        GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length,
        const GLchar* message, const void* userParam);

  Shader() = default;
  Shader(const char* vertexPath, const char* fragmentPath);
  ~Shader();

  void compile(const char* vertexSource, const char* fragmentSource);
  void use() const;

  // Uniform setters
  void set_bool(const std::string &name, bool value) const;
  void set_int(const std::string &name, int value) const;
  void set_float(const std::string &name, float value) const;
  void set_vec2(const std::string &name, const glm::vec2 &value) const;
  void set_vec3(const std::string &name, const glm::vec3 &value) const;
  void set_vec4(const std::string &name, const glm::vec4 &value) const;
  void set_mat2(const std::string &name, const glm::mat2 &mat) const;
  void set_mat3(const std::string &name, const glm::mat3 &mat) const;
  void set_mat4(const std::string &name, const glm::mat4 &mat) const;

  template<typename T>
  void set_uniform(const std::string &name, const T& value) const {
      if constexpr (std::is_same_v<T, bool>) {
        set_bool(name, value);
      } else if constexpr (std::is_same_v<T, int>) {
        set_int(name, value);
      } else if constexpr (std::is_same_v<T, float>) {
        set_float(name, value);
      } else if constexpr (std::is_same_v<T, glm::vec2>) {
        set_vec2(name, value);
      } else if constexpr (std::is_same_v<T, glm::vec3>) {
        set_vec3(name, value);
      } else if constexpr (std::is_same_v<T, glm::vec4>) {
        set_vec4(name, value);
      } else if constexpr (std::is_same_v<T, glm::mat2>) {
        set_mat2(name, value);
      } else if constexpr (std::is_same_v<T, glm::mat3>) {
        set_mat3(name, value);
      } else if constexpr (std::is_same_v<T, glm::mat4>) {
        set_mat4(name, value);
      }
    }

  bool is_valid() const;
  bool uniform_exists(const std::string &name) const;

  unsigned int get_id() const { return ID; }

  int get_uniform_location(const std::string &name) const;

private:
  unsigned int ID{};
  mutable std::unordered_map<std::string, int> uniform_location_cache;

  static std::string read_file(const char* path) ;
  static void check_compile_errors(unsigned int shader, const std::string& type) ;
};
