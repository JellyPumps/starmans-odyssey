#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdexcept>

namespace STARBORN {
  // ---- Constructors & Destructor ----
  Texture::Texture(const char *filepath, const std::string& type) : type_(type) {
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    generate_texture(filepath);
    setup_texture();
  }

  Texture::Texture(const unsigned char* data, int width, int height, int channels,
               const std::string& type) : type_(type) {
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    const GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    setup_texture();
  }

  Texture::~Texture() {
    glDeleteTextures(1, &texture_id_);
  }

  void Texture::setup_texture() const {
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  void Texture::generate_texture(const char *filepath) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);

    if (data) {
      const GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      throw std::runtime_error("Failed to load texture.");
    }

    stbi_image_free(data);
  }

  void Texture::opengl_sampler(const Shader &shader) {
    shader.set_int("texture_id_", 0);
  }

  void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
  }
}