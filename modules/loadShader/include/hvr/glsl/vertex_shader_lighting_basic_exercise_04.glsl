#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;


out vec3 ourColor; // output a color to the fragment shader


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float specularStrength;
uniform float gloss;
float glossiness;
vec3 Normal;
vec3 FragPos;  

void main()
{
    gl_Position = projection*view*model*vec4(aPos, 1.0f);
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model)))*aNormal;

    vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir),0.0f);
	vec3 diffuse = diff * lightColor;
	vec3 ambient = ambientStrength*lightColor;
	vec3 viewDir = normalize(-FragPos);
	vec3 reflector = reflect(-lightDir, norm);
	glossiness = pow(gloss,2);
	float spec = pow(max(dot(viewDir,reflector),0.0f),glossiness);
	vec3 specular = specularStrength * spec * lightColor;
	ourColor = (ambient + diffuse + specular)*objectColor;
}
