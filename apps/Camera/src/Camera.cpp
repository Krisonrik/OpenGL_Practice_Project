HVR_WINDOWS_DISABLE_ALL_WARNING

//#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include "hvr/cameraEuler/cameraEuler.hpp"
#include "hvr/loadAssetDir/loadAssetDir.hpp"
#include "hvr/loadShader/loadShader.hpp"

#include "tinyxml2.h"

HVR_WINDOWS_ENABLE_ALL_WARNING
// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

cameraEuler curCam(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of last frame

float lastX = 400, lastY = 300;
float pitch = 0.0f, yaw = 0.0f;
bool firstMouse = true;
float fov       = 45.0f;

void key_callback(GLFWwindow* window, int key, int, int action, int)
{
  // When a user presses the escape key, we set the WindowShouldClose property
  // to true, closing the application
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  float cameraSpeed = 2.5f * deltaTime;
  // float cameraSpeed = 0.05f; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    curCam.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    curCam.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    curCam.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    curCam.ProcessKeyboard(RIGHT, deltaTime);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)  // this bool variable is initially set to true
  {
    lastX      = xpos;
    lastY      = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos;  // reversed since y-coordinates range from bjottom to top
  lastX = xpos;
  lastY = ypos;

  curCam.ProcessMouseMovement(xoffset, yoffset);

  /*float sensitivity = 0.05f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;
  pitch += yoffset;
  yaw += xoffset;

  if (pitch > 89.0f)
      pitch = 89.0f;
  if (pitch < -89.0f)
      pitch = -89.0f;*/
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  // if (fov >= 1.0f && fov <= 45.0f)
  //    fov -= yoffset;
  // if (fov <= 1.0f)
  //    fov = 1.0f;
  // if (fov >= 45.0f)
  //    fov = 45.0f;

  curCam.ProcessMouseScroll(yoffset);
}

void loadImg(const char* imgDirChar,
             unsigned int texture,
             unsigned int textureUnit,
             bool hasAlpha)
{
  cv::Mat image;
  if (hasAlpha)
  {
    image = cv::imread(imgDirChar, 1);
    cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
  }
  else
  {
    image = cv::imread(imgDirChar, -1);
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
  }

  cv::flip(image, image, 0);

  glGenTextures(1, &texture);

  glActiveTexture(
      GL_TEXTURE0 +
      textureUnit);  // activate the texture unit first before binding texture
  glBindTexture(GL_TEXTURE_2D, texture);

  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (!image.empty())
  {
    if (hasAlpha)
    {
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGB,
                   image.cols,
                   image.rows,
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   image.ptr());
      glGenerateMipmap(GL_TEXTURE_2D);
      // return;
    }
    else
    {
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGB,
                   image.cols,
                   image.rows,
                   0,
                   GL_RGB,
                   GL_UNSIGNED_BYTE,
                   image.ptr());
      glGenerateMipmap(GL_TEXTURE_2D);
      // return;
    }
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
    // return;
  }

  // stbi_image_free(data);
  // return;
}

int main()
{
  // Initialize GLFW
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

  const unsigned int screenWidth = 800, screenHeight = 600;

  GLFWwindow* window = glfwCreateWindow(
      screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
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

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Triangle vertex array
  // float vertices[] = {
  //    // positions          // colors           // texture coords
  //    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
  //    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
  //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
  //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
  //};
  glEnable(GL_DEPTH_TEST);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(2.0f, 5.0f, -15.0f),
                               glm::vec3(-1.5f, -2.2f, -2.5f),
                               glm::vec3(-3.8f, -2.0f, -12.3f),
                               glm::vec3(2.4f, -0.4f, -3.5f),
                               glm::vec3(-1.7f, 3.0f, -7.5f),
                               glm::vec3(1.3f, -2.0f, -2.5f),
                               glm::vec3(1.5f, 2.0f, -2.5f),
                               glm::vec3(1.5f, 0.2f, -1.5f),
                               glm::vec3(-1.3f, 1.0f, -1.5f)};

  // compile shader
  unsigned int shaderProgram;
  //    GLuint shaderProgram;
  loadShader currentShader;
  shaderProgram = currentShader.loadShaders(
      "vertex_shader_coordinate-systems.glsl", "fragment_shader_texture.glsl");

  // EBO setup
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  // ..:: Initialization code :: ..
  // 1. bind Vertex Array Object
  glBindVertexArray(VAO);
  // 2. copy our vertices array in a vertex buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 3. copy our index array in a element buffer for OpenGL to use
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(
  //    GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // 4. then set the vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(
      1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
  // (void*)(6 * sizeof(float)));  glEnableVertexAttribArray(2);

  loadAssetDir dirLoader;
  std::string assetDir;
  dirLoader.loadXML("input.xml", assetDir);

  std::string dirPadding = "\\textures\\";
  std::string img        = "container.jpg";
  std::string imgDir     = assetDir + dirPadding + img;
  // std::cout << imgDir << std::endl;

  const char* imgDirChar = imgDir.data();
  unsigned int texture1 = 0, texture2 = 0;
  // bool hasAlpha = false;
  loadImg(imgDirChar, texture1, 0, false);

  img        = "awesomeface.png";
  imgDir     = assetDir + dirPadding + img;
  imgDirChar = imgDir.data();
  loadImg(imgDirChar, texture2, 1, true);

  // use linked shader program and draw a triangle
  glUseProgram(shaderProgram);
  glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"),
              0);  // set it manually
  glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1);

  float timeDelate = 0.0f;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Rendering commands here
    // setup background color and clear color buffer
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime          = currentFrame - lastFrame;
    lastFrame          = currentFrame;
    timeDelate += .1f;

    // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    // float radius = 20.0f;
    // float camX = sin(glfwGetTime()) * radius;
    // float camZ = cos(glfwGetTime()) * radius;
    // float camY = cos(glfwGetTime()) * radius;
    // glm::mat4 view;
    // glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

    // direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    // direction.y = sin(glm::radians(pitch));
    // direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    // view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    view = curCam.GetViewMatrix();

    // if (timeDelate > 10.0f) {
    //    std::cout << glm::to_string(view) << std::endl;
    //    //std::cout << glm::to_string(curCam.Position) << std::endl;
    //    //std::cout << glm::to_string(curCam.Front) << std::endl;
    //    //std::cout << glm::to_string(curCam.Up) << std::endl;

    //    timeDelate = 0.0f;
    //}

    // model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f,
    // 0.0f));  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  view
    // = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    projection = glm::perspective(glm::radians(cameraEuler::camDefault::ZOOM),
                                  float(screenWidth / screenHeight),
                                  0.1f,
                                  100.0f);

    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
      glm::mat4 model;
      model = glm::translate(model, cubePositions[i]);
      // float angle = 20.0f * i;
      if (i < 3)
      {
        model = glm::rotate(
            model, glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
      }
      else
      {
        model = glm::rotate(model,
                            (float)glfwGetTime() * glm::radians(50.0f),
                            glm::vec3(1.0f, 0.3f, 0.5f));
      }
      int modelLoc = glGetUniformLocation(shaderProgram, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      // ourShader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // draw  in  wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // draw in fill mode (canceling wireframe mode)
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Swap the buffers
    glfwSwapBuffers(window);
    // clear zbuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  glfwTerminate();
  return 0;
}
