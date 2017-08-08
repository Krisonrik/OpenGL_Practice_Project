HVR_WINDOWS_DISABLE_ALL_WARNING
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
using namespace std;

#include <stdlib.h>
#include <string.h>
//#define GLEW_STATIC
#include <GL/glew.h>
HVR_WINDOWS_ENABLE_ALL_WARNING

#include "hvr/loadImg/loadImgManual.hpp"

loadImgManual::loadImgManual(){};
loadImgManual::~loadImgManual(){};
void loadImgManual::loadImgs(const char* imgDirChar,
                             unsigned int& texture,
                             unsigned int& textureUnit)
{
  cv::Mat image;
  std::cout << imgDirChar << std::endl;

  image = cv::imread(imgDirChar, -1);

  if (!image.empty())
  {
    if (image.channels() <= 3)
    {
      cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGB,
                   image.cols,
                   image.rows,
                   0,
                   GL_RGB,
                   GL_UNSIGNED_BYTE,
                   image.ptr());
    }
    else if (image.channels() > 3)
    {
      cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA,
                   image.cols,
                   image.rows,
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   image.ptr());
    }
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
    // return;
  }

  cv::flip(image, image, 0);

  glGenTextures(1, &texture);

  // activate the texture unit first before binding texture
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, texture);

  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // return;
}

void loadImgManual::loadCubemap(std::vector<std::string>& faces,
                                unsigned int& texture,
                                unsigned int& textureUnit)
{
  // unsigned int textureID;
  cv::Mat image;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

  // int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++)
  {
    // unsigned char* data =
    // stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    image = cv::imread(faces[i].c_str(), -1);

    if (!image.empty())
    {
      if (image.channels() <= 3)
      {
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0,
                     GL_RGB,
                     image.cols,
                     image.rows,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     image.ptr());
      }
      else if (image.channels() > 3)
      {
        cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0,
                     GL_RGBA,
                     image.cols,
                     image.rows,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     image.ptr());
      }
    }
    else
    {
      std::cout << "Cubemap texture failed to load at path: " << faces[i]
                << std::endl;
      // stbi_image_free(data);
    }
  }
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  // return textureID;
}
