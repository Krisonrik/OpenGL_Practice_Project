#ifndef SHADER_HPP
#define SHADER_HPP

class HVR_RICHARD_LOADSHADER_DLL loadShader {

public:
    loadShader();
    GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
    ~loadShader();
};


#endif
