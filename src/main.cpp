/*
 * Created by Sarthak Rai on 01 Apr 2025.
 * Entry point
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  // ---- Initialize GLFW ----
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // ---- Create A Window ----
  GLFWwindow *window = glfwCreateWindow(800, 600, "Starman's Odyssey", NULL, NULL);

  if (window == NULL) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(window);

  // ---- Initialize GLAD ----
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    glfwDestroyWindow(window);
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLAD");
  }

  // ---- Set Viewport ----
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // ---- Main Loop ----
  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
