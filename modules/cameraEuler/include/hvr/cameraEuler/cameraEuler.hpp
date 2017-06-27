#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum Camera_Movement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class cameraEuler
{
 public:
  struct HVR_RICHARD_CAMERAEULER_DLL camDefault
  {
    // Default camera values
    static const float YAW;
    static const float PITCH;
    static const float SPEED;
    static const float SENSITIVTY;
    static const float ZOOM;
  };
  // camDefault camDef;

  HVR_RICHARD_CAMERAEULER_DLL
  cameraEuler(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
              glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
              float yaw          = camDefault::YAW,
              float pitch        = camDefault::PITCH);
  HVR_RICHARD_CAMERAEULER_DLL
  cameraEuler(float posX,
              float posY,
              float posZ,
              float upX,
              float upY,
              float upZ,
              float yaw,
              float pitch);
  HVR_RICHARD_CAMERAEULER_DLL
  ~cameraEuler();
  HVR_RICHARD_CAMERAEULER_DLL
  glm::mat4 GetViewMatrix();
  HVR_RICHARD_CAMERAEULER_DLL
  void ProcessKeyboard(Camera_Movement direction, float deltaTime);
  HVR_RICHARD_CAMERAEULER_DLL
  void ProcessMouseMovement(float xoffset,
                            float yoffset,
                            GLboolean constrainPitch = true);
  HVR_RICHARD_CAMERAEULER_DLL
  void ProcessMouseScroll(float yoffset);

  HVR_RICHARD_CAMERAEULER_DLL
  glm::vec3 getPosition();

 private:
  // Camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Eular Angles
  float Yaw;
  float Pitch;
  // Camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;
  // Calculates the front vector from the Camera's (updated) Eular Angles
  void updateCameraVectors();
};

#endif
