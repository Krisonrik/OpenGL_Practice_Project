#ifndef LOADMESH_HPP
#define LOADMESH_HPP

#include "hvr\loadShader\loadShaderNew.hpp"

class loadMesh
{
 public:
  struct Vertex
  {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
  };

  struct Texture
  {
    unsigned int id;
    std::string type;
    aiString path;
  };

  /*  Functions  */
  // HVR_RICHARD_LOADMESH_DLL
  // loadMesh();
  HVR_RICHARD_LOADMESH_DLL
  loadMesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices,
           std::vector<Texture> textures);
  HVR_RICHARD_LOADMESH_DLL
  ~loadMesh();
  HVR_RICHARD_LOADMESH_DLL
  void Draw(loadShaderNew &shader);

 private:
  /*  Mesh Data  */
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;
  /* Render data*/
  unsigned int VAO, VBO, EBO;

  /* Functions */
  void setupMesh();
};

#endif
