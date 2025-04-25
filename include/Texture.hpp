/*
 * Created by Sarthak Rai on 04 Apr 2025.
*/

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <vector>

namespace STARBORN {
  class Texture {
  private:
    // ---- Variables ----
    unsigned int texture_id_{};
    int width_{}, height_{}, nr_channels_{};
    std::vector<unsigned char> data_;
    std::string type_;
    std::string path_;

    // ---- Setup ----
    void setup_texture() const;
    void generate_texture(const char *filepath);
    void load_from_memory();
  public:
    explicit Texture(const char *filepath, const std::string& type = "texture_diffuse");
    Texture(const std::vector<unsigned char>& data, int width, int height, int channels,
            const std::string& type = "texture_diffuse", const std::string& path = "");
    ~Texture();

    // ---- Getters ----
    [[nodiscard]] const std::string& get_type() const { return type_; }
    [[nodiscard]] const std::string& get_path() const { return path_; }

    // ---- Methods ----
    static void opengl_sampler(const Shader &shader);
    void bind() const;
  };
}