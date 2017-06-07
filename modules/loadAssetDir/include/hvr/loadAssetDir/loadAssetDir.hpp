#include <array>

class HVR_RICHARD_LOADASSETDIR_DLL loadAssetDir
{
 public:
  loadAssetDir();
  ~loadAssetDir();
  void loadXML(char* argv, std::string& dir);

  // protected:
  // std::array<int, 5> vecList = {1, 5, 8, 16, 21};
};
