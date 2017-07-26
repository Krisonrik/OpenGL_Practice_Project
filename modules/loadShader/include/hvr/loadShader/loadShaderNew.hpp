#ifndef LOADSHADERNEW_HPP
#define LOADSHADERNEW_HPP
HVR_WINDOWS_DISABLE_ALL_WARNING
#include "glm/glm.hpp"
HVR_WINDOWS_ENABLE_ALL_WARNING
class loadShaderNew
{
 public:
  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  loadShaderNew(const std::string &vertexPath,
                const std::string &fragmentPath,
                const std::string &geometryPath = "");

  // loadShaderNew(const loadShaderNew &loadshadernew);

  HVR_RICHARD_LOADSHADERNEW_DLL
  ~loadShaderNew();
  // activate the shader
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void use();
  // utility uniform functions
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setBool(const std::string &name, bool value) const;
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setInt(const std::string &name, int value) const;
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setFloat(const std::string &name, float value) const;
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setVec2(const std::string &name, float x, float y) const;
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setVec3(const std::string &name, float x, float y, float z) const;
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setVec4(
      const std::string &name, float x, float y, float z, float w) const;
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  // ------------------------------------------------------------------------
  HVR_RICHARD_LOADSHADERNEW_DLL
  void setMat4(const std::string &name, const glm::mat4 &mat) const;

  HVR_RICHARD_LOADSHADERNEW_DLL
  unsigned int getID();

 private:
  // 1. retrieve the vertex/fragment source code from filePath
  unsigned int ID;
  std::string vertexCode;
  std::string fragmentCode;
  std::string geometryCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  std::ifstream gShaderFile;

  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(GLuint shader, std::string type);
};
#endif
