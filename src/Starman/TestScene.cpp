/*
 * Created by Sarthak Rai on 27 Apr 2025.
*/

#include "TestScene.hpp"
#include "Input.hpp"

namespace STARMAN {
  TestScene::TestScene(const STARBORN::Window &window)
    : shader_("assets/shaders/basic.vert", "assets/shaders/basic.frag"),
    test_model_("assets/models/test_models/tm_002.glb"),
    player_(glm::vec3(0.0f)),
    window_(window) {
    player_.set_aspect_ratio(16.0f / 9.0f);
  }

  void TestScene::init() {
    STARBORN::Input::get_instance().init(window_.get_window());
    glEnable(GL_DEPTH_TEST);
    STARBORN::ScreenQuad::init();
    frame_buffer_ = std::make_unique<STARBORN::FrameBuffer>(window_.get_width(), window_.get_height());
    post_processing_shader_ = std::make_unique<STARBORN::Shader>(
      "assets/shaders/post.vert", "assets/shaders/post.frag");
  }

  void TestScene::update(float delta_time) {
    // ---- Update Player ----
    player_.update(delta_time);
  }

  void TestScene::render() {
    const auto camera = player_.get_camera();

    // ---- Render to Frame Buffer ----
    frame_buffer_->bind();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_.use();
    shader_.set_vec3("viewPos", camera->get_position());
    shader_.set_vec3("lightPos", glm::vec3(2.0f, 2.0f, 2.0f));
    shader_.set_vec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader_.set_float("shininess", 32.0f);

    // ---- Set projection and view matrices ----
    shader_.set_mat4("projection", camera->get_projection_matrix());
    shader_.set_mat4("view", camera->get_view_matrix());

    // ---- Draw ----
    auto model = glm::mat4(1.0f);
    model = translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    model = scale(model, glm::vec3(2.0f));
    shader_.set_mat4("model", model);
    test_model_.draw(shader_);

    // ---- Render to Screen ----
    frame_buffer_->unbind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    post_processing_shader_->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame_buffer_->get_texture_id());
    post_processing_shader_->set_int("screenTexture", 0);

    // ---- Post Processing Effects ----
    post_processing_shader_->set_bool("applyQuantize", true);
    post_processing_shader_->set_float("quantizeIntensity", 0.8f);
    post_processing_shader_->set_int("colorLevels", 8);
    post_processing_shader_->set_bool("applyDither", true);
    post_processing_shader_->set_float("ditherIntensity", 1.0f);

    // ---- Draw Screen Quad ----
    STARBORN::ScreenQuad::draw();
  }

  void TestScene::cleanup() {
    STARBORN::ScreenQuad::cleanup();
  }

  void TestScene::on_enter() {
    // Empty
  }

  void TestScene::on_exit() {
    // Empty
  }
}