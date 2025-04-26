/*
 * Created by Sarthak Rai on 26 Apr 2025.
*/

#include "FrameBuffer.hpp"

namespace STARBORN {
  // ---- Constructor & Destructor ----
   FrameBuffer::FrameBuffer(const int width, const int height) : width_(width), height_(height) {
     glGenFramebuffers(1, &FBO);
     glBindFramebuffer(GL_FRAMEBUFFER, FBO);

     // ---- Color Attachment Texture ----
     glGenTextures(1, &texture_color_buffer);
     glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_color_buffer, 0);

     // ---- Render Buffer Object ----
     glGenRenderbuffers(1, &rbo);
     glBindRenderbuffer(GL_RENDERBUFFER, rbo);
     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
     glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

     if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Framebuffer is not complete");

     glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

   FrameBuffer::~FrameBuffer() {
     glDeleteFramebuffers(1, &FBO);
   }

  void FrameBuffer::bind() const {
     glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  }

  void FrameBuffer::unbind() {
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void FrameBuffer::resize(const int width, const int height) {
     width_ = width;
     height_ = height;

     // ---- Recreate Texture ----
     glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

     // ---- Recreate RenderBuffer ----
     glBindRenderbuffer(GL_RENDERBUFFER, rbo);
     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

     // ---- Check Framebuffer ----
     glBindFramebuffer(GL_FRAMEBUFFER, FBO);
     if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("Framebuffer is not complete");
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}