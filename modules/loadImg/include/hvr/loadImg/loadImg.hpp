#ifndef LOADIMG_HPP
#define LOADIMG_HPP

class HVR_RICHARD_LOADIMG_DLL loadImg
{
 public:
  loadImg();
  void loadImgs(const char* imgDirChar, unsigned int& texture, bool hasAlpha);
  ~loadImg();
};

#endif
