// #version 330 core
// out vec4 FragColor;
  
// uniform vec3 objectColor;
// uniform vec3 lightColor;

// void main()
// {
//     FragColor = vec4(lightColor * objectColor, 1.0);
// }

#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	vec4 texColor = mix(texture(ourTexture1, vec2(TexCoord.x,1-TexCoord.y)), texture(ourTexture2, TexCoord), 0.2);
    FragColor = vec4(vec3(texColor.x,texColor.y,texColor.z)*lightColor,1.0);

}