#ifndef MODEL_HPP
#define MODEL_HPP

#include "hvr\loadShader\loadShaderNew.hpp"

class Model
{
 public:
  /*  Functions   */
  HVR_RICHARD_LOADMODEL_DLL
  Model(std::string path, unsigned int &textureUnit);

  HVR_RICHARD_LOADMODEL_DLL
  ~Model();

  HVR_RICHARD_LOADMODEL_DLL
  void Draw(loadShaderNew &shader);

  void getTextureUnit(unsigned int &textureUnit);
  void setTextureUnit(unsigned int &textureUnit);

 private:
  /*  Model Data  */
  std::vector<loadMesh> meshes;
  std::string directory;
  /*  Functions   */
  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  loadMesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<loadMesh::Texture> loadMaterialTextures(aiMaterial *mat,
                                                      aiTextureType type,
                                                      std::string typeName);
  loadMesh::Texture texture_;
  unsigned int textureUnit_ = 0;
};

#endif
