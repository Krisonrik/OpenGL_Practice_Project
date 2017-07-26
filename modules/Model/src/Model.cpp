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

Model::Model(std::string &path)  //, unsigned int &textureUnit)
{
  loadModel(path);
}
Model::~Model(){};
void Model::loadModel(std::string &path)
{
  Assimp::Importer import;
  const aiScene *scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }
  // std::cout << path << std::endl;
  directory_ = path.substr(0, path.find_last_of('\\'));
  // std::cout << directory_ << std::endl;
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
      vec.x            = mesh->mTextureCoords[0][i].x;
      vec.y            = mesh->mTextureCoords[0][i].y;
      vertex.TexCoords = vec;
      // std::cout << vec.x << std::endl;
      // std::cout << vec.y << std::endl;
    }
    else
    {
      std::cout << "UV not loaded" << std::endl;

      vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }
    vertices.push_back(vertex);
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
    std::vector<loadMesh::Texture> diffuseMaps = loadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<loadMesh::Texture> specularMaps = loadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    std::vector<loadMesh::Texture> normalMaps =
        loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
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
    str       = directory_ + "\\" + std::string(str.C_Str());
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

      image.loadImgs(str.C_Str(), textureId, true);
      texture_.id = textureId;
      // std::cout << textureId << std::endl;
      texture_.type = typeName;
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
