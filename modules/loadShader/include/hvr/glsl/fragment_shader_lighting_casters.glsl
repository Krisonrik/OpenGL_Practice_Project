#version 330 core
out vec4 FragColor;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
};

uniform Light light;

in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
// uniform vec3 lightPos;
uniform vec3 viewPos;



void main()
{
	// ambient
	vec3 ambient = light.ambient * material.ambient;
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir),0.0f);
	vec3 diffuse =  light.diffuse * (diff * material.diffuse);
	// specular
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflector = reflect(-lightDir, norm);
	float glossiness = pow(2,material.shininess);
	float spec = pow(max(dot(viewDir,reflector),0.0f),glossiness);
	vec3 specular =  light.specular * (spec * material.specular);


	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result,1.0f);

}
