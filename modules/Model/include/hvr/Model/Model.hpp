#ifndef MODEL_HPP
#define MODEL_HPP

//#include "hvr\loadImg\loadImg.hpp"
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include "hvr\loadMesh\loadMesh.hpp"
#include "hvr\loadShader\loadShaderNew.hpp"

class Model
{
 public:
  /*  Functions   */
  HVR_RICHARD_MODEL_DLL
  Model(std::string &path);

  HVR_RICHARD_MODEL_DLL
  ~Model();

  HVR_RICHARD_MODEL_DLL
  void Draw(loadShaderNew &shader);

  HVR_RICHARD_MODEL_DLL
  const std::vector<loadMesh> &getMeshes() const;

 private:
  /*  Model Data  */
  std::vector<loadMesh> meshes;
  std::string directory_;
  /*  Functions   */
  void loadModel(std::string &path);
  void processNode(aiNode *node, const aiScene *scene);
  loadMesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<loadMesh::Texture> loadMaterialTextures(aiMaterial *mat,
                                                      aiTextureType type,
                                                      std::string typeName);
  loadMesh::Texture texture_;
  // unsigned int textureUnit_ = 0;
  std::vector<loadMesh::Texture> textures_loaded;
};

#endif
