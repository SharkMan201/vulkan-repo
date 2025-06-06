//
// Created by abdoe on 6/06/2025.
//
#include "learn_opengl/shaders/shader.h"
#include "third_party/glad/include/glad/glad.h"

#include <GLFW/glfw3.h>
#include <iostream>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  // Update view port width & height with resize
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS
#endif

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  float vertices[] = {
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.0f,
      0.0f, 1.0f,  0.0f, 0.0f, 0.5f, 0.0f, 0.0f,  0.0f,  1.0f,
  };
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  // 1. bind vertex array object
  glBindVertexArray(vao);

  // 2. copy our vertices array in a buffer for OpenGL to use
  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // tell openGL how to read the vertex buffer data
  // setup vertices (location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // setup colors (location = 1)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // setup element buffer object
  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // unbinding buffer & array objects
  glBindVertexArray(NULL); // needs to be unbound first to not record unbinding
                           // the ebos
  glBindBuffer(GL_ARRAY_BUFFER, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

  // create shaders
  Shader our_shader("_main/learn_opengl/shaders/shader.vert",
                    "_main/learn_opengl/shaders/shader.frag");

  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering commands
    // ------
    // clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // set openGL to use the program
    our_shader.use();

    // draw the triangle
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // de-allocate all resources
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  our_shader.destroy();

  glfwTerminate();
  return 0;
}