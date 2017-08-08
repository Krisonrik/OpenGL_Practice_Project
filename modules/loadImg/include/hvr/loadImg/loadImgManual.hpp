#ifndef LOADIMG_HPP
#define LOADIMG_HPP

class HVR_RICHARD_LOADIMGMANUAL_DLL loadImgManual
{
 public:
  loadImgManual();
  void loadImgs(const char* imgDirChar,
                unsigned int& texture,
                unsigned int& textureUnit);
  void loadCubemap(std::vector<std::string>& faces,
                   unsigned int& texture,
                   unsigned int& textureUnit);
  ~loadImgManual();
};

#endif
