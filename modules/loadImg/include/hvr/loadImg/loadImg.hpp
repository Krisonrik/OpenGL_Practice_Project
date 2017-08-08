#ifndef LOADIMG_HPP
#define LOADIMG_HPP

class HVR_RICHARD_LOADIMG_DLL loadImg
{
 public:
  loadImg();
  void loadImgs(const char* imgDirChar, unsigned int& texture, bool hasAlpha);
  void loadCubemap(std::vector<std::string>& faces, unsigned int& textureID);
  ~loadImg();
};

#endif
