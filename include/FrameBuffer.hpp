/*
 * Created by Sarthak Rai on 26 Apr 2025.
*/

#pragma once

#include <glad/glad.h>
#include <stdexcept>

namespace STARBORN {
  class FrameBuffer {
  private:
    unsigned int FBO{}, texture_color_buffer{}, rbo{};
    int width_, height_;
  public:
    // ---- Constructor & Destructor ----
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    // ---- Bind & Unbind ----
    void bind() const;
    static void unbind();

    // ---- Resize ----
    void resize(int width, int height);

    // ---- Getters & Setters ----
    [[nodiscard]] unsigned int get_texture_id() const { return texture_color_buffer; }
    [[nodiscard]] int get_width() const { return width; }
    [[nodiscard]] int get_height() const { return height; }
  };
}