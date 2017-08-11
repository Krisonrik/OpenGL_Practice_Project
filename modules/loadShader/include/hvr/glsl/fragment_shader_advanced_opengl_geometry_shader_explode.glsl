#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D texture_diffuse;
    sampler2D texture_specular;
    int shininess;
    float emit;
    float glowIntensity;
    // float shininess;
};


in vec2 TexCoords;


uniform vec3 viewPos;
uniform vec3 cameraPos;
uniform Material material;

void main()
{
	
    FragColor = vec4(vec3(texture(material.texture_diffuse, TexCoords)),1.0f);

    // FragColor = vec4(vec3(texture(material.texture_specular, TexCoords)),1.0f);

}