// Hello_Triangle.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include "hvr/loadShader/loadShader.hpp"

void key_callback(GLFWwindow* window, int key, int, int action, int)
{
  // When a user presses the escape key, we set the WindowShouldClose property
  // to true, closing the application
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return -1;
  }

  ////glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window =
      glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;

    //        printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  glViewport(0, 0, width, height);

  // Square vertex array
  float vertices[] = {
      // positions         // colors
      0.5f,
      0.5f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,  // bottom right
      -0.5f,
      0.5f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,  // bottom left
      0.0f,
      -0.5f,
      0.0f,
      0.0f,
      0.0f,
      1.0f  // top
  };
  // Vertex Indices array
  unsigned int indices[] = {
      // note that we start from 0!
      0,
      1,
      2,  // first triangle
  };

  unsigned int shaderProgram;
  //    GLuint shaderProgram;
  loadShader currentShader;
  shaderProgram =
      currentShader.loadShaders("vertex_shader.glsl", "fragment_shader.glsl");

  // EBO setup
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // ..:: Initialization code :: ..
  // 1. bind Vertex Array Object
  glBindVertexArray(VAO);
  // 2. copy our vertices array in a vertex buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 3. copy our index array in a element buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // 4. then set the vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  GLint nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
            << std::endl;

  float r = 0.1f;
  float g = 0.5f;
  float b = 0.3f;
  float a = 1.0f;
  // Program loop
  while (!glfwWindowShouldClose(window))
  {
    // Check and call events
    glfwPollEvents();
    glfwSetKeyCallback(window, key_callback);

    // Rendering commands here
    // setup background color and clear color buffer
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);

    // use linked shader program and draw a triangle
    glUseProgram(shaderProgram);

    // float timeValue         = float(glfwGetTime());
    // float greenValue        = (sin(timeValue) / 2.0f) + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(shaderProgram,
    // "ourColor");
    // glUseProgram(shaderProgram);
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // glDrawArrays(GL_TRIANGLES, 0, 3);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    // draw  in  wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // draw in fill mode (canceling wireframe mode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Swap the buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
