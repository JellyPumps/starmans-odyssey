/*
 * Created by Sarthak Rai on 27 Apr 2025.
*/

#pragma once

#include "Scene.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Player.hpp"
#include "ScreenQuad.hpp"
#include "FrameBuffer.hpp"
#include <memory>

namespace STARMAN {
  class TestScene : public STARBORN::Scene {
  private:
    STARBORN::Shader shader_;
    std::unique_ptr<STARBORN::Shader> post_processing_shader_;
    STARBORN::Model test_model_;
    Player player_;
    std::unique_ptr<STARBORN::FrameBuffer> frame_buffer_;
    STARBORN::Window window_;

  public:
    // ---- Constructor & Destructor ----
    explicit TestScene(const STARBORN::Window &window);
    ~TestScene() override = default;

    // ---- Scene Methods ----
    void init() override;
    void update(float delta_time) override;
    void render() override;
    void cleanup() override;
    void on_enter() override;
    void on_exit() override;
  };
}