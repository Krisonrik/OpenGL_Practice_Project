#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float specularStrength;
uniform float gloss;
float glossiness;



void main()
{
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
	vec3 result = (ambient + diffuse + specular)*objectColor;

    FragColor = vec4(result,1.0f);

}