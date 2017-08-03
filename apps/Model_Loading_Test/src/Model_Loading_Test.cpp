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

#include "tinyxml2.h"
HVR_WINDOWS_ENABLE_ALL_WARNING

#include "hvr/cameraEuler/cameraEuler.hpp"
#include "hvr/loadAssetDir/loadAssetDir.hpp"
#include "hvr/loadImg/loadImg.hpp"
#include "hvr/loadShader/loadShader.hpp"
#include "hvr\Model\Model.hpp"
//#include "hvr\loadMesh\loadMesh.hpp"
#include "hvr\loadShader\loadShaderNew.hpp"

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

  // compile shader

  loadShaderNew lightingProgram("vertex_shader_lighting_multiple.glsl",
                                "fragment_shader_lighting_multiple_model.glsl");
  // std::cout << lightingProgram.getID() << std::endl;

  loadShaderNew lampProgram("vertex_shader_lamp.glsl",
                            "fragment_shader_lamp.glsl");

  // EBO setup
  unsigned int lightVBO, lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &lightVBO);

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

  std::string dirPadding = "\\objects\\nanosuit_simple\\";
  std::string file       = "nanosuit.obj";
  std::string FileDir    = assetDir + dirPadding + file;
  // std::cout << FileDir << std::endl;

  Model NanoSuit = Model::Model(FileDir);

  float timeDelate = 0.0f;

  float r = 0.1f;
  float g = 0.5f;
  float b = 0.3f;
  float a = 1.0f;

  // glm::vec3 lightAmbient  = glm::vec3(0.2f, 0.2f, 0.2f);
  // glm::vec3 lightDiffuse  = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 lightPos(.2f, 1.0f, .3f);
  glm::vec3 lightColor(.5f, .5f, .5f);
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

    glm::vec3 lightDiffuse =
        lightColor * glm::vec3(0.5f);  // decrease the influence
    glm::vec3 lightAmbient = lightDiffuse * glm::vec3(0.2f);  // low influence

    glm::mat4 view;
    glm::mat4 projection;

    view = curCam.GetViewMatrix();

    lightingProgram.use();

    lightingProgram.setVec3("dirLight.ambient", lightAmbient);
    lightingProgram.setVec3("dirLight.diffuse", lightDiffuse);
    lightingProgram.setVec3("dirLight.specular", lightSpecular);
    lightingProgram.setVec3("dirLight.position", -lightPos);

    // point lights setting
    for (int i = 0; i < 4; i++)
    {
      std::string counter    = std::to_string(i);
      std::string lightType  = std::string("pointLights[");
      std::string uniformVal = lightType.append(counter);
      std::string tempHolder = uniformVal;
      lightingProgram.setVec3(uniformVal.append("].ambient").c_str(),
                              lightAmbient);
      uniformVal = tempHolder;

      lightingProgram.setVec3(uniformVal.append("].diffuse").c_str(),
                              lightDiffuse);
      uniformVal = tempHolder;

      lightingProgram.setVec3(uniformVal.append("].specular").c_str(),
                              lightSpecular);
      uniformVal = tempHolder;

      lightingProgram.setVec3(uniformVal.append("].position").c_str(),
                              pointLightPositions[i]);
      uniformVal = tempHolder;
      lightingProgram.setFloat((uniformVal.append("].constant").c_str()), 1.0f);
      uniformVal = tempHolder;
      lightingProgram.setFloat((uniformVal.append("].linear").c_str()), 0.09f);
      uniformVal = tempHolder;
      lightingProgram.setFloat((uniformVal.append("].quaratic").c_str()),
                               0.032f);
    }

    lightingProgram.setVec3("spotLight.ambient", lightAmbient);
    lightingProgram.setVec3("spotLight.diffuse", lightDiffuse);
    lightingProgram.setVec3("spotLight.specular", lightSpecular);
    lightingProgram.setVec3("spotLight.specular", lightSpecular);
    lightingProgram.setVec3("spotLight.direction", curCam.getFront());
    lightingProgram.setFloat("spotLight.constant", 1.0f);
    lightingProgram.setFloat("spotLight.linear", 0.09f);
    lightingProgram.setFloat("spotLight.quadratic", 0.032f);
    lightingProgram.setFloat("spotLight.cutOff", lightCutoff);
    lightingProgram.setFloat("spotLight.outerCutOff", lightOuterCutoff);
    lightingProgram.setVec3("viewPos", curCam.getPosition());
    // lightingProgram.setVec3("material.specular", specular);
    lightingProgram.setFloat("material.glowIntensity", glowIntensity);

    // lightingProgram.setInt("material.diffuse", 0);
    // lightingProgram.setInt("material.specular", 1);
    lightingProgram.setInt("material.shininess", 2);
    lightingProgram.setFloat("material.emit", 0.1f);

    projection = glm::perspective(glm::radians(cameraEuler::camDefault::ZOOM),
                                  float(screenWidth / screenHeight),
                                  0.1f,
                                  100.0f);

    lightingProgram.setMat4("view", view);

    lightingProgram.setMat4("projection", projection);
    glm::mat4 model;
    model = glm::translate(model, lightPos);
    model =
        glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));

    model = glm::scale(model, glm::vec3(.2f));

    lightingProgram.setMat4("model", model);
    NanoSuit.Draw(lightingProgram);

    // render the cube
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    lampProgram.use();

    // view/projection transformations

    lampProgram.setMat4("view", view);
    lampProgram.setMat4("projection", projection);

    // world transformation
    //    glm::mat4 model;
    model = glm::translate(model, lightPos);
    model =
        glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));

    model = glm::scale(model, glm::vec3(.2f));

    lampProgram.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // NanoSuit.Draw(lampProgram);

    // glBindVertexArray(lightVAO);

    // std::cout << "______frame end______" << std::endl;

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
