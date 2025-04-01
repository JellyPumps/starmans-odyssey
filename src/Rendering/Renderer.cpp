/*
* Created by Sarthak Rai on 1/4/25.
*/

#include "Rendering/Renderer.hpp"

#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Window.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <GL/glew.h>  // Include GLEW for APIENTRY

Renderer::Renderer(const Window& window) : window_(window) {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

Renderer::~Renderer() {
    shaders_.clear();
}

void Renderer::init() {
    if (initialized_) return;

    // Create OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window_.get_context());
    if (!context) {
        throw std::runtime_error("Failed to create OpenGL context: " + std::string(SDL_GetError()));
    }

    if (!glGetString(GL_VERSION)) {
        throw std::runtime_error("OpenGL not properly initialized");
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW: " + std::string((const char*)glewGetErrorString(err)));
    }

    // Check OpenGL version
    std::cerr << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    setup_opengl_state();
    initialized_ = true;
}

void Renderer::setup_opengl_state() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::begin_frame() {
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Immediate test quad rendering
    const char* vs = "#version 330 core\n"
        "void main() {\n"
        "vec2 pos = vec2(gl_VertexID&1, (gl_VertexID>>1)&1)*2.0-1.0;\n"
        "gl_Position = vec4(pos, 0.0, 1.0);\n"
        "}";

    const char* fs = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() { FragColor = vec4(1.0, 0.0, 0.0, 1.0); }";

    GLuint prog;
    glGenProgramPipelines(1, &prog);
    glUseProgram(prog);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
void Renderer::end_frame() const {
    window_.swap_buffers();
    check_gl_errors("End of frame");
}

void Renderer::set_clear_color(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::submit_camera(const Camera& camera) {
    view_matrix_ = camera.get_view_matrix();
    projection_matrix_ = camera.get_projection_matrix();
}

void Renderer::draw_mesh(const Mesh& mesh, const Shader& shader, const glm::mat4& model_matrix) const {
    if(!shader.is_valid()) {
        std::cerr << "Shader not valid for drawing!" << std::endl;
        return;
    }

    shader.use();

    // Verify matrices
    if(glm::determinant(view_matrix_) == 0 ||
       glm::determinant(projection_matrix_) == 0) {
        std::cerr << "Invalid camera matrices!" << std::endl;
       }

    mesh.draw();
}

// ---- Shader Management ----
void Renderer::load_shader(const std::string& name, const char* vertex_path, const char* fragment_path) {
    auto shader = std::make_unique<Shader>(vertex_path, fragment_path);
    shader->use();
    GLint status;
    glGetProgramiv(shader->get_id(), GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        char log[512];
        glGetProgramInfoLog(shader->get_id(), 512, NULL, log);
        std::cerr << "SHADER FAILED:\n" << log << std::endl;
    }

    // Simple test draw
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_POINTS, 0, 1); // Minimal draw call
    window_.swap_buffers();

    shaders_[name] = std::move(shader);
}

Shader* Renderer::get_shader(const std::string& name) const {
    auto it = shaders_.find(name);
    return it != shaders_.end() ? it->second.get() : nullptr;
}

// ---- State Management ----
void Renderer::set_viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::enable_depth_testing(bool enable) {
    enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Renderer::enable_wireframe_mode(bool enable) {
    glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
}

void Renderer::check_gl_errors(const char* operation) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error during " << operation << ": 0x"
                  << std::hex << err << std::dec << std::endl;
    }
}
