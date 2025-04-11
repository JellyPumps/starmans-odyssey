/*
 * Created by Sarthak Rai on 04 Apr 2025.
*/

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>

namespace STARBORN {
  class Texture {
  private:
    // ---- Variables ----
    unsigned int texture_id_{};
    int width_{}, height_{}, nr_channels_{};
    unsigned char *data_{};

    // ---- Setup ----
    void setup_texture() const;
    static void generate_texture(const char *filepath);
  public:
    explicit Texture(const char *filepath);
    Texture(const unsigned char* data, int width, int height, int channels);
    ~Texture();

    // ---- Methods ----
    static void opengl_sampler(const Shader &shader);
    void bind() const;
  };
}