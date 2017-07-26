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
#include <assimp\Importer.hpp>
#include <glm\glm.hpp>

HVR_WINDOWS_ENABLE_ALL_WARNING
#include "hvr/loadMesh/loadMesh.hpp"
#include "hvr\loadShader\loadShaderNew.hpp"

// loadMesh::loadMesh(){};

loadMesh::loadMesh(std::vector<Vertex> vertices,
                   std::vector<unsigned int> indices,
                   std::vector<Texture> textures)
{
  vertices_ = vertices;
  indices_  = indices;
  textures_ = textures;

  setupMesh();

  // return;
}
loadMesh::~loadMesh(){};

void loadMesh::setupMesh()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER,
               vertices_.size() * sizeof(Vertex),
               &vertices_[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices_.size() * sizeof(unsigned int),
               &indices_[0],
               GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}

void loadMesh::Draw(loadShaderNew &shader)
{
  // bind appropriate textures
  unsigned int diffuseNr  = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr   = 1;
  unsigned int heightNr   = 1;
  for (unsigned int i = 0; i < textures_.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 +
                    i);  // active proper texture unit before binding
                         // retrieve texture number (the N in diffuse_textureN)
    std::stringstream ss;
    std::string number;
    std::string name = textures_[i].type;
    // ss << i + 1;
    if (name == "texture_diffuse")
      ss << diffuseNr++;  // transfer unsigned int to stream
    else if (name == "texture_specular")
      ss << specularNr++;  // transfer unsigned int to stream
    else if (name == "texture_normal")
      ss << normalNr++;  // transfer unsigned int to stream
    else if (name == "texture_height")
      ss << heightNr++;  // transfer unsigned int to stream
    number = ss.str();
    // now set the sampler to the correct texture unit
    shader.setInt(("material." + name).c_str(), i);
    // std::cout << "material." + name << std::endl;
    // glUniform1i(glGetUniformLocation(shader.getID(), (name +
    // number).c_str()),
    //            i);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    // std::cout << textures_[i].id << std::endl;
  }

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, (GLsizei)indices_.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  // std::cout << "end part" << std::endl;

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}
