HVR_WINDOWS_DISABLE_ALL_WARNING

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

HVR_WINDOWS_ENABLE_ALL_WARNING

#include "hvr/cameraEuler/cameraEuler_fps.hpp"
const float cameraEuler_fps::camDefault::YAW        = -90.0f;
const float cameraEuler_fps::camDefault::PITCH      = 0.0f;
const float cameraEuler_fps::camDefault::SPEED      = 2.5f;
const float cameraEuler_fps::camDefault::SENSITIVTY = 0.1f;
const float cameraEuler_fps::camDefault::ZOOM       = 45.0f;
// Constructor with vectors
cameraEuler_fps::cameraEuler_fps(glm::vec3 position,
                                 glm::vec3 up,
                                 float yaw,
                                 float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f))
    , MovementSpeed(camDefault::SPEED)
    , MouseSensitivity(camDefault::SENSITIVTY)
    , Zoom(camDefault::ZOOM)
{
  Position = position;
  WorldUp  = up;
  Yaw      = yaw;
  Pitch    = pitch;
  updateCameraVectors();
}

// Constructor with scalar values
cameraEuler_fps::cameraEuler_fps(float posX,
                                 float posY,
                                 float posZ,
                                 float upX,
                                 float upY,
                                 float upZ,
                                 float yaw,
                                 float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f))
    , MovementSpeed(camDefault::SPEED)
    , MouseSensitivity(camDefault::SENSITIVTY)
    , Zoom(camDefault::ZOOM)
{
  Position = glm::vec3(posX, posY, posZ);
  WorldUp  = glm::vec3(upX, upY, upZ);
  Yaw      = yaw;
  Pitch    = pitch;
  updateCameraVectors();
}

cameraEuler_fps::~cameraEuler_fps()
{
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 cameraEuler_fps::GetViewMatrix()
{
  // return glm::lookAt(Position, Position + Front, Up);
  return myLookAt(Position, Position + Front, Up);
}

// Processes input received from any keyboard-like input system. Accepts input
// parameter in the form of camera defined ENUM (to abstract it from windowing
// systems)
void cameraEuler_fps::ProcessKeyboard(Camera_Movement direction,
                                      float deltaTime)
{
  float velocity    = MovementSpeed * deltaTime;
  glm::vec3 tempPos = glm::vec3();
  if (direction == FORWARD)
  {
    tempPos   = Front * velocity;
    tempPos.y = 0.0f;
    Position += tempPos;
  }
  if (direction == BACKWARD)
  {
    tempPos   = Front * velocity;
    tempPos.y = 0.0f;
    Position -= tempPos;
  }
  if (direction == LEFT)
  {
    tempPos   = Right * velocity;
    tempPos.y = 0.0f;
    Position -= tempPos;
  }

  if (direction == RIGHT)
  {
    tempPos   = Right * velocity;
    tempPos.y = 0.0f;
    Position += tempPos;
  }
}

// Processes input received from a mouse input system. Expects the offset value
// in both the x and y direction.
void cameraEuler_fps::ProcessMouseMovement(float xoffset,
                                           float yoffset,
                                           GLboolean constrainPitch)
{
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Eular angles
  updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input
// on the vertical wheel-axis
void cameraEuler_fps::ProcessMouseScroll(float yoffset)
{
  if (Zoom >= 1.0f && Zoom <= 45.0f) Zoom -= yoffset;
  if (Zoom <= 1.0f) Zoom = 1.0f;
  if (Zoom >= 45.0f) Zoom = 45.0f;
}

void cameraEuler_fps::updateCameraVectors()
{
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front   = glm::normalize(front);
  // Also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(
      Front, WorldUp));  // Normalize the vectors, because their length gets
                         // closer to 0 the more you look up or down which
                         // results in slower movement.
  Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 cameraEuler_fps::myLookAt(glm::vec3 position,
                                    glm::vec3 target,
                                    glm::vec3 up)
{
  glm::mat4 lookAtVec = glm::mat4();

  glm::mat4 translate = glm::translate(lookAtVec, -position);
  glm::vec3 camAim    = glm::normalize(position - target);
  glm::vec3 camRight  = glm::cross(glm::normalize(up), camAim);
  glm::vec3 camUp     = glm::cross(camAim, camRight);

  glm::mat4 rotation;
  rotation[0][0] = camRight.x;  // First column, first row
  rotation[1][0] = camRight.y;
  rotation[2][0] = camRight.z;
  rotation[0][1] = camUp.x;  // First column, second row
  rotation[1][1] = camUp.y;
  rotation[2][1] = camUp.z;
  rotation[0][2] = camAim.x;  // First column, third row
  rotation[1][2] = camAim.y;
  rotation[2][2] = camAim.z;
  lookAtVec      = rotation * translate;

  // std::cout << glm::to_string(lookAtVec) << std::endl;
  return lookAtVec;
}
