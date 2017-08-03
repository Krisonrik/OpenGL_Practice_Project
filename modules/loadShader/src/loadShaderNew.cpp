HVR_WINDOWS_DISABLE_ALL_WARNING
#include <GL\glew.h>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
HVR_WINDOWS_ENABLE_ALL_WARNING
#include "hvr\loadShader\loadShaderNew.hpp"

// constructor generates the shader on the fly
// ------------------------------------------------------------------------
loadShaderNew::loadShaderNew(const std::string &vertexPath,
                             const std::string &fragmentPath,
                             const std::string &geometryPath)
{
  // std::cout << vertexPath << std::endl;
  // std::cout << fragmentPath << std::endl;
  // open files
  vShaderFile.open(vertexPath);
  fShaderFile.open(fragmentPath);
  std::stringstream vShaderStream, fShaderStream;
  // read file's buffer contents into streams
  vShaderStream << vShaderFile.rdbuf();
  fShaderStream << fShaderFile.rdbuf();
  // close file handlers
  vShaderFile.close();
  fShaderFile.close();
  // convert stream into string
  vertexCode   = vShaderStream.str();
  fragmentCode = fShaderStream.str();

  // std::cout << fragmentCode << std::endl;

  // if geometry shader path is present, also load a geometry shader
  if (geometryPath != "")
  {
    gShaderFile.open(geometryPath);
    std::stringstream gShaderStream;
    gShaderStream << gShaderFile.rdbuf();
    gShaderFile.close();
    geometryCode = gShaderStream.str();
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  // 2. compile shaders
  unsigned int vertex, fragment;
  // int success;
  // char infoLog[512];
  // vertex shader

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");
  // if geometry shader is given, compile geometry shader
  unsigned int geometry = 0;
  if (geometryPath != "")
  {
    const char *gShaderCode = geometryCode.c_str();
    geometry                = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    checkCompileErrors(geometry, "GEOMETRY");
  }
  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  if (geometryPath != "") glAttachShader(ID, geometry);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");
  // delete the shaders as they're linked into our program now and no longer
  // necessery
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (geometryPath != "") glDeleteShader(geometry);
}

// loadShaderNew::loadShaderNew(const loadShaderNew & loadshadernew)
//{
//    ID = loadshadernew.ID;
//
//}

loadShaderNew::~loadShaderNew()
{
}

// activate the shader
// ------------------------------------------------------------------------
void loadShaderNew::use()
{
  glUseProgram(ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void loadShaderNew::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void loadShaderNew::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void loadShaderNew::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void loadShaderNew::setVec2(const std::string &name,
                            const glm::vec2 &value) const
{
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void loadShaderNew::setVec2(const std::string &name, float x, float y) const
{
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void loadShaderNew::setVec3(const std::string &name,
                            const glm::vec3 &value) const
{
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void loadShaderNew::setVec3(const std::string &name,
                            float x,
                            float y,
                            float z) const
{
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void loadShaderNew::setVec4(const std::string &name,
                            const glm::vec4 &value) const
{
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void loadShaderNew::setVec4(
    const std::string &name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void loadShaderNew::setMat2(const std::string &name, const glm::mat2 &mat) const
{
  glUniformMatrix2fv(
      glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void loadShaderNew::setMat3(const std::string &name, const glm::mat3 &mat) const
{
  glUniformMatrix3fv(
      glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void loadShaderNew::setMat4(const std::string &name, const glm::mat4 &mat) const
{
  glUniformMatrix4fv(
      glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned int loadShaderNew::getID()
{
  return ID;
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void loadShaderNew::checkCompileErrors(GLuint shader, std::string type)
{
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout
          << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout
          << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
    }
  }
}
