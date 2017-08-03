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
#include "hvr/loadImg/loadImg.hpp"
#include "hvr/loadShader/loadShader.hpp"

#include "tinyxml2.h"

HVR_WINDOWS_ENABLE_ALL_WARNING

cameraEuler curCam(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of last frame

float lastX = 400, lastY = 300;
float pitch = 0.0f, yaw = 0.0f;
bool firstMouse = true;
float fov       = 45.0f;

void key_callback(GLFWwindow* window)
{
  // When a user presses the escape key, we set the WindowShouldClose property
  // to true, closing the application
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    curCam.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    curCam.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    curCam.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    curCam.ProcessKeyboard(RIGHT, deltaTime);
}
void mouse_callback(GLFWwindow*, double xpos, double ypos)
{
  if (firstMouse)  // this bool variable is initially set to true
  {
    lastX      = float(xpos);
    lastY      = float(ypos);
    firstMouse = false;
  }

  float xoffset = float(xpos) - lastX;
  float yoffset =
      lastY -
      float(ypos);  // reversed since y-coordinates range from bjottom to top
  lastX = float(xpos);
  lastY = float(ypos);

  curCam.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow*, double, double yoffset)
{
  curCam.ProcessMouseScroll(float(yoffset));
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

  glEnable(GL_DEPTH_TEST);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
      0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
      0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
      -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
      0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
      -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
      0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
      -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
      1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
      -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

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
  unsigned int lightingProgram;
  unsigned int lampProgram;

  loadShader lightingShader;
  lightingProgram =
      lightingShader.loadShaders("vertex_shader_lighting_multiple.glsl",
                                 "fragment_shader_lighting_multiple.glsl");

  loadShader lampShader;
  lampProgram = lampShader.loadShaders("vertex_shader_lamp.glsl",
                                       "fragment_shader_lamp.glsl");

  // EBO setup
  unsigned int VBO, lightVBO, VAO, lightVAO;  /// , EBO;
  glGenVertexArrays(1, &VAO);
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &lightVBO);
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
      2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // we only need to bind to the VBO, the container's VBO's data already
  // contains the correct data.
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // set the vertex attributes (only position data for our lamp)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  loadAssetDir dirLoader;
  loadImg imgLoader;
  std::string assetDir;
  dirLoader.loadXML("input.xml", assetDir);

  std::string dirPadding = "\\textures\\";
  std::string img        = "container2.png";
  std::string imgDir     = assetDir + dirPadding + img;
  // std::cout << imgDir << std::endl;

  const char* imgDirChar = imgDir.data();
  unsigned int texture1 = 0, texture2 = 0, texture3 = 0;
  // bool hasAlpha = false;
  imgLoader.loadImgs(imgDirChar, texture1, false);
  // std::cout << imgDirChar << std::endl;

  img        = "container2_specular.png";
  imgDir     = assetDir + dirPadding + img;
  imgDirChar = imgDir.data();
  imgLoader.loadImgs(imgDirChar, texture2, false);

  img        = "matrix.jpg";
  imgDir     = assetDir + dirPadding + img;
  imgDirChar = imgDir.data();
  imgLoader.loadImgs(imgDirChar, texture3, false);

  float timeDelate = 0.0f;

  float r = 0.1f;
  float g = 0.5f;
  float b = 0.3f;
  float a = 1.0f;

  // glm::vec3 lightAmbient  = glm::vec3(0.2f, 0.2f, 0.2f);
  // glm::vec3 lightDiffuse  = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 lightPos(.2f, 1.0f, .3f);
  glm::vec3 lightColor(0.5f, 0.5f, 1.0f);
  float lightCutoff      = glm::cos(glm::radians(12.5f));
  float lightOuterCutoff = glm::cos(glm::radians(15.0f));

  glm::vec3 ambient  = glm::vec3(1.0f, 0.5f, 0.31f);
  glm::vec3 diffuse  = glm::vec3(1.0f, 0.5f, 0.31f);
  glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
  // float lightMovementRadius = 0.1f;
  float glowIntensity = 0.0f;
  // float shininess = 5.0f;
  glm::vec3 pointLightPositions[] = {glm::vec3(0.7f, 0.2f, 2.0f),
                                     glm::vec3(2.3f, -3.3f, -4.0f),
                                     glm::vec3(-4.0f, 2.0f, -12.0f),
                                     glm::vec3(0.0f, 0.0f, -3.0f)};

  // Program loop
  while (!glfwWindowShouldClose(window))
  {
    // Check and call events
    glfwPollEvents();
    // glfwSetKeyCallback(window, key_callback);
    key_callback(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Rendering commands here
    // setup background color and clear color buffer
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);

    float currentFrame = float(glfwGetTime());
    deltaTime          = currentFrame - lastFrame;
    lastFrame          = currentFrame;
    timeDelate += .1f;

    // lightColor.x = (float)sin(glfwGetTime() * 2.0f);
    // lightColor.y = (float)sin(glfwGetTime() * 0.7f);
    // lightColor.z = (float)sin(glfwGetTime() * 1.3f);

    glm::vec3 lightDiffuse =
        lightColor * glm::vec3(0.5f);  // decrease the influence
    glm::vec3 lightAmbient = lightDiffuse * glm::vec3(0.2f);  // low influence

    // glowIntensity = glm::abs((float)glm::sin(glfwGetTime()));
    // lightPos      = lightPos +
    //           glm::vec3((float)glm::cos(glfwGetTime()) * lightMovementRadius,
    //                     (float)glm::cos(glfwGetTime()) * lightMovementRadius,
    //                     (float)glm::sin(glfwGetTime()) *
    //                     lightMovementRadius);
    // std::cout << glm::to_string(lightPos) << std::endl;
    // glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    view = curCam.GetViewMatrix();

    glUseProgram(lightingProgram);

    // directional light setting
    glUniform3f(glGetUniformLocation(lightingProgram, "dirLight.ambient"),
                lightAmbient.r,
                lightAmbient.g,
                lightAmbient.b);
    glUniform3f(glGetUniformLocation(lightingProgram, "dirLight.diffuse"),
                lightDiffuse.r,
                lightDiffuse.g,
                lightDiffuse.b);
    glUniform3f(glGetUniformLocation(lightingProgram, "dirLight.specular"),
                lightSpecular.r,
                lightSpecular.g,
                lightSpecular.b);
    glUniform3f(glGetUniformLocation(lightingProgram, "diright.position"),
                -lightPos.x,
                -lightPos.y,
                -lightPos.z);

    // point lights setting
    for (int i = 0; i < 4; i++)
    {
      std::string counter    = std::to_string(i);
      std::string lightType  = std::string("pointLights[");
      std::string uniformVal = lightType.append(counter);
      std::string tempHolder = uniformVal;
      glUniform3f(glGetUniformLocation(lightingProgram,
                                       uniformVal.append("].ambient").c_str()),
                  lightAmbient.r,
                  lightAmbient.g,
                  lightAmbient.b);
      uniformVal = tempHolder;
      glUniform3f(glGetUniformLocation(lightingProgram,
                                       uniformVal.append("].diffuse").c_str()),
                  lightDiffuse.r,
                  lightDiffuse.g,
                  lightDiffuse.b);
      uniformVal = tempHolder;
      glUniform3f(glGetUniformLocation(lightingProgram,
                                       uniformVal.append("].specular").c_str()),
                  lightSpecular.r,
                  lightSpecular.g,
                  lightSpecular.b);
      uniformVal = tempHolder;
      glUniform3f(glGetUniformLocation(lightingProgram,
                                       uniformVal.append("].position").c_str()),
                  pointLightPositions[i].x,
                  pointLightPositions[i].y,
                  pointLightPositions[i].z);
      uniformVal = tempHolder;
      glUniform1f(glGetUniformLocation(lightingProgram,
                                       uniformVal.append("].constant").c_str()),
                  1.0f);
      uniformVal = tempHolder;
      glUniform1f(glGetUniformLocation(lightingProgram,
                                       uniformVal.append("].linear").c_str()),
                  0.09f);
      uniformVal = tempHolder;
      glUniform1f(
          glGetUniformLocation(lightingProgram,
                               uniformVal.append("].quadratic").c_str()),
          0.032f);
    }

    // spot light setting
    glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.ambient"),
                lightAmbient.r,
                lightAmbient.g,
                lightAmbient.b);
    glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.diffuse"),
                lightDiffuse.r,
                lightDiffuse.g,
                lightDiffuse.b);
    glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.specular"),
                lightSpecular.r,
                lightSpecular.g,
                lightSpecular.b);
    glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.position"),
                curCam.getPosition().x,
                curCam.getPosition().y,
                curCam.getPosition().z);
    glUniform3f(glGetUniformLocation(lightingProgram, "spotLight.direction"),
                curCam.getFront().x,
                curCam.getFront().y,
                curCam.getFront().z);

    glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.constant"),
                1.0f);
    glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.linear"),
                0.09f);
    glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.quadratic"),
                0.032f);
    glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.cutOff"),
                lightCutoff);
    glUniform1f(glGetUniformLocation(lightingProgram, "spotLight.outerCutOff"),
                lightOuterCutoff);

    glUniform3f(glGetUniformLocation(lightingProgram, "viewPos"),
                curCam.getPosition().x,
                curCam.getPosition().y,
                curCam.getPosition().z);

    glUniform3f(glGetUniformLocation(lightingProgram, "material.specular"),
                specular.r,
                specular.g,
                specular.b);
    glUniform1f(glGetUniformLocation(lightingProgram, "material.glowIntensity"),
                glowIntensity);
    glUniform1i(glGetUniformLocation(lightingProgram, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingProgram, "material.specular"), 1);
    glUniform1i(glGetUniformLocation(lightingProgram, "material.shininess"), 1);
    glUniform1i(glGetUniformLocation(lightingProgram, "material.emit"), 2);

    projection = glm::perspective(glm::radians(cameraEuler::camDefault::ZOOM),
                                  float(screenWidth / screenHeight),
                                  0.1f,
                                  100.0f);

    int viewLoc = glGetUniformLocation(lightingProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    int projectionLoc = glGetUniformLocation(lightingProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
      int modelLoc = glGetUniformLocation(lightingProgram, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      // ourShader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glUseProgram(lampProgram);

    // view/projection transformations

    viewLoc = glGetUniformLocation(lampProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    projectionLoc = glGetUniformLocation(lampProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // world transformation
    glm::mat4 model;
    model = glm::translate(model, lightPos);
    model =
        glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));

    model        = glm::scale(model, glm::vec3(.2f));
    int modelLoc = glGetUniformLocation(lampProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // render the cube

    // glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

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
