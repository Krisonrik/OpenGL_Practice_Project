#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
// in vec3 Normal;
// in vec3 FragPos;

void main()
{
    FragColor = vec4(ourColor,1.0f);
}