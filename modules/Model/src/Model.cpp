HVR_WINDOWS_DISABLE_ALL_WARNING
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
// using namespace std;

#include <stdlib.h>
#include <string.h>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <glm\glm.hpp>

HVR_WINDOWS_ENABLE_ALL_WARNING
#include "hvr\Model\Model.hpp"
#include "hvr\loadImg\loadImg.hpp"
#include "hvr\loadMesh\loadMesh.hpp"
#include "hvr\loadShader\loadShaderNew.hpp"

Model::Model(std::string path, unsigned int &textureUnit)
{
  getTextureUnit(textureUnit);
  loadModel(path);
  setTextureUnit(textureUnit);
}
Model::~Model(){};
void Model::loadModel(std::string path)
{
  Assimp::Importer import;
  const aiScene *scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
  }
}

loadMesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
  std::vector<loadMesh::Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<loadMesh::Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    loadMesh::Vertex vertex;
    // process vertex positions, normals and texture coordinates
    glm::vec3 vector;
    vector.x        = mesh->mVertices[i].x;
    vector.y        = mesh->mVertices[i].y;
    vector.z        = mesh->mVertices[i].z;
    vertex.Position = vector;
    vertices.push_back(vertex);
    if (mesh->mNormals)
    {
      vector.x      = mesh->mNormals[i].x;
      vector.y      = mesh->mNormals[i].y;
      vector.z      = mesh->mNormals[i].z;
      vertex.Normal = vector;
    }
    if (mesh->mTextureCoords[0])
    {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
    }
    else
    {
      vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }
  }
  // process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
    {
      indices.push_back(face.mIndices[j]);
    }
  }
  // process material
  if (mesh->mMaterialIndex > 0)
  {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    textureUnit_ += 1;
    std::vector<loadMesh::Texture> diffuseMaps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textureUnit_ += 1;
    std::vector<loadMesh::Texture> specularMaps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return loadMesh::loadMesh(vertices, indices, textures);
}

std::vector<loadMesh::Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                           aiTextureType type,
                                                           std::string typeName)
{
  std::vector<loadMesh::Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    {
      if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
      {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip)
    {
      loadImg image;
      unsigned int textureId = 0;
      image.loadImgs(str.C_Str(), textureId, textureUnit_, false);
      texture_.id   = textureId;
      texture_.type = (char)type;
      texture_.path = str;
      textures.push_back(texture_);
    }
  }
  return textures;
}

void Model::Draw(loadShaderNew &shader)
{
  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    meshes[i].Draw(shader);
  }
}

void Model::getTextureUnit(unsigned int &textureUnit)
{
  textureUnit_ = textureUnit;
}

void Model::setTextureUnit(unsigned int &textureUnit)
{
  textureUnit = textureUnit_;
}
