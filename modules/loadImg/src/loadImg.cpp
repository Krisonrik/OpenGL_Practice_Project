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

#include "hvr/loadImg/loadImg.hpp"

loadImg::loadImg(){};
loadImg::~loadImg(){};
void loadImg::loadImgs(const char* imgDirChar,
                       unsigned int& texture,
                       bool hasAlpha)
{
  cv::Mat image;
  std::cout << imgDirChar << std::endl;
  if (hasAlpha)
  {
    image = cv::imread(imgDirChar, -1);
    cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
  }
  else
  {
    image = cv::imread(imgDirChar, 1);
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
  }

  cv::flip(image, image, 0);

  glGenTextures(1, &texture);

  /* glActiveTexture(
      GL_TEXTURE0 +
      textureUnit);*/  // activate the texture unit first before binding texture
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
                   GL_RGBA,
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

  // return;
}
