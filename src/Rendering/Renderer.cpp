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

Renderer::Renderer(const Window& window) : window_(window) {}

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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    set_clear_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
}

void Renderer::begin_frame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    if (!initialized_) return;

    shader.use();

    // Set matrices
    shader.set_mat4("uModel", model_matrix);
    shader.set_mat4("uView", view_matrix_);
    shader.set_mat4("uProjection", projection_matrix_);

    // Draw mesh
    mesh.draw();
    check_gl_errors("Mesh drawing");
}

// ---- Shader Management ----
void Renderer::load_shader(const std::string& name, const char* vertex_path, const char* fragment_path) {
    shaders_[name] = std::make_unique<Shader>(vertex_path, fragment_path);
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

