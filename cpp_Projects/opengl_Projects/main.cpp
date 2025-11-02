#include <GL/glew.h>
#include <GLFW/glfw3.h>
// compile with: cpp compiler
#include <iostream>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int main() {
  if (!glfwInit()) {
    // std::cerr << "Failed to initialize GLFW\n";
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  GLFWwindow *window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL);
  if (!window) {
    // std::cerr << "Failed to create GLFW window\n";
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW (must be done after context creation)
  if (glewInit() != GLEW_OK) {
    // if (glewInit() != 0) {
    // std::cerr << "Failed to initialize GLEW\n";
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
