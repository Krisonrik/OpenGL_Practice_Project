HVR_WINDOWS_DISABLE_ALL_WARNING

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
#include "hvr/loadImg/loadImgManual.hpp"
//#include "hvr/loadShader/loadShader.hpp"
#include "hvr\Model\Model.hpp"
//#include "hvr\loadMesh\loadMesh.hpp"
#include "hvr\loadShader\loadShaderNew.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
// unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH  = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
cameraEuler camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX     = (float)SCR_WIDTH / 2.0f;
float lastY     = (float)SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE);  // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);  // always pass the depth test (same effect as
                         // glDisable(GL_DEPTH_TEST))
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_STENCIL_TEST);
  // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CCW);

  // build and compile shaders
  // -------------------------
  loadShaderNew shaderDude("vertex_shader_cubemap_dude.glsl",
                           "fragment_shader_cubemap_dude.glsl");

  loadShaderNew shaderSkybox("vertex_shader_cubemap_skybox.glsl",
                             "fragment_shader_cubemap_skybox.glsl");

  float skyboxVertices[] = {
      // positions
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  // cube VAO

  unsigned int cubeVAO, cubeVBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glBindVertexArray(0);

  // load textures
  // -------------
  loadAssetDir dirLoader;
  loadImgManual imgLoader;
  std::string assetDir;
  dirLoader.loadXML("input.xml", assetDir);

  std::string dirPadding = "\\textures\\skybox\\";
  // std::string img        = "container.jpg";
  // std::string imgDir = assetDir + dirPadding + img;
  // std::cout << imgDir << std::endl;
  std::vector<std::string> faces = {"right.jpg",
                                    "left.jpg",
                                    "top.jpg",
                                    "bottom.jpg",
                                    "back.jpg",
                                    "front.jpg"};

  for (int i = 0; i < (int)faces.size(); i++)
  {
    std::string imgDir = assetDir + dirPadding + faces[i];
    // const char* imgDirChar = imgDir.data();
    faces[i] = imgDir;
  }
  // const char* imgDirChar = imgDir.data();

  unsigned int cubemapTexture = 0, textureUnit = 0;
  // unsigned int cubeTexture = 0, floorTexture = 0;
  // unsigned int texture1 = 0, texture2 = 0, texture3 = 0;
  // bool hasAlpha = false;
  imgLoader.loadCubemap(faces, cubemapTexture, textureUnit);
  // std::cout << imgDirChar << std::endl;

  // shader configuration
  // --------------------
  shaderSkybox.use();
  shaderSkybox.setInt("skybox", textureUnit);

  // load Model
  std::string modelPath = assetDir;
  dirPadding            = "\\objects\\nanosuit_simple\\";
  std::string objName   = "nanosuit.obj";
  modelPath += dirPadding + objName;
  Model dude(modelPath);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = (float)glfwGetTime();
    deltaTime          = currentFrame - lastFrame;
    lastFrame          = currentFrame;

    // input
    // -----
    processInput(window);

    // render
    // ------
    // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glDisable(GL_DEPTH_TEST);
    // glDepthMask(GL_FALSE);
    // set uniforms

    glm::mat4 model;
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection =
        glm::perspective(cameraEuler::camDefault::ZOOM,
                         (float)SCR_WIDTH / (float)SCR_HEIGHT,
                         0.1f,
                         10000.0f);
    // model = glm::scale(model, glm::vec3(1000.0f));

    // draw dude
    shaderDude.use();

    shaderDude.setMat4("view", view);
    shaderDude.setMat4("projection", projection);
    shaderDude.setMat4("model", glm::mat4());
    shaderDude.setVec3("cameraPos", camera.getPosition());
    glActiveTexture(GL_TEXTURE0 + 5);
    shaderDude.setInt("skybox", 5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    dude.Draw(shaderDude);

    // draw skybox
    glDepthFunc(GL_LEQUAL);
    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    shaderSkybox.use();
    shaderSkybox.setMat4("view", view);
    shaderSkybox.setMat4("projection", projection);
    shaderSkybox.setMat4("model", glm::mat4());

    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);  // set depth function back to default

    // glDepthMask(GL_TRUE);
    // glEnable(GL_DEPTH_TEST);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);

  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow*, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX      = (float)xpos;
    lastY      = (float)ypos;
    firstMouse = false;
  }

  float xoffset = (float)xpos - lastX;
  float yoffset =
      lastY -
      (float)ypos;  // reversed since y-coordinates go from bottom to top

  lastX = (float)xpos;
  lastY = (float)ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow*, double, double yoffset)
{
  camera.ProcessMouseScroll((float)yoffset);
}
