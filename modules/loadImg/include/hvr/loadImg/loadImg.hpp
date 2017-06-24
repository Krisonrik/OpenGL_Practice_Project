#ifndef LOADIMG_H
#define LOADIMG_H

class HVR_RICHARD_LOADIMG_DLL loadImg
{
 public:
  loadImg();
  void loadImgs(const char* imgDirChar,
                unsigned int texture,
                unsigned int textureUnit,
                bool hasAlpha);
  ~loadImg();
};

#endif
