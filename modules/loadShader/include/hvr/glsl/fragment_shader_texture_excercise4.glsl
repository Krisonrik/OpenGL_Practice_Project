#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float mixAmount;

void main()
{
    FragColor = mix(texture(ourTexture1, vec2(TexCoord.x,1-TexCoord.y)), texture(ourTexture2, TexCoord), mixAmount);

}