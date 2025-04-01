/*
 * Created by Sarthak Rai on 01 Apr 2025.
 * Entry point
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.hpp"


int main() {
  // ---- Create Window ----
  STARBORN::Window window(800, 600, "Starman's Odyssey");

  // ---- Main Loop ----
  while (!glfwWindowShouldClose(window.get_window())) {
    glfwSwapBuffers(window.get_window());
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
