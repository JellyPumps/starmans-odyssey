//
// Created by sarthak on 4/1/25.
//

#include "Rendering/Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "GL/glew.h"

void Shader::GLAPIENTRY opengl_debug_callback(
    GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* message, const void* userParam)
{
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** ERROR **" : "")
              << " type: 0x" << std::hex << type
              << ", severity: 0x" << std::hex << severity
              << ", message: " << message << std::endl;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = read_file(vertexPath);
    std::string fragmentCode = read_file(fragmentPath);
    compile(vertexCode.c_str(), fragmentCode.c_str());
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::compile(const char* vertexSource, const char* fragmentSource) {
    // Vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    // Fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    // Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    check_compile_errors(ID, "PROGRAM");

    // Delete the shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const {
    glUseProgram(ID);
}

bool Shader::is_valid() const {
    GLint status;
    glValidateProgram(ID);
    glGetProgramiv(ID, GL_VALIDATE_STATUS, &status);
    return status == GL_TRUE;
}

bool Shader::uniform_exists(const std::string& name) const {
    return glGetUniformLocation(ID, name.c_str()) != -1;
}

int Shader::get_uniform_location(const std::string &name) const {
    if (uniform_location_cache.find(name) != uniform_location_cache.end())
        return uniform_location_cache[name];

    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1)
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    uniform_location_cache[name] = location;
    return location;
}

// Uniform setters
void Shader::set_bool(const std::string &name, bool value) const {
    glUniform1i(get_uniform_location(name), (int)value);
}

void Shader::set_int(const std::string &name, int value) const {
    glUniform1i(get_uniform_location(name), value);
}

void Shader::set_float(const std::string &name, float value) const {
    glUniform1f(get_uniform_location(name), value);
}

void Shader::set_vec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(get_uniform_location(name), 1, &value[0]);
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(get_uniform_location(name), 1, &value[0]);
}

void Shader::set_vec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(get_uniform_location(name), 1, &value[0]);
}

void Shader::set_mat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &mat[0][0]);
}

std::string Shader::read_file(const char* path) {
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        code = stream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path << std::endl;
    }

    return code;
}

void Shader::check_compile_errors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
}
