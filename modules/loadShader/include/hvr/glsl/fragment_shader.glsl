// #version 330 core
// out vec4 FragColor;
  
// in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

// void main()
// {
//     FragColor = vertexColor;
// } 

// #version 330 core
// out vec4 FragColor;
  
// uniform vec4 ourColor; // we set this variable in the OpenGL code.

// void main()
// {
//     FragColor = ourColor;
// }   

#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
  
void main()
{
    FragColor = vec4(ourColor, 1.0);
}