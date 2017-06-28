#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D diffuse;
    sampler2D specular;
    sampler2D shininess;
    sampler2D emit;
    float glowIntensity;
    // float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
};
uniform Material material;
uniform Light light;

in vec2 TexCoords;
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
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir),0.0f);
	vec3 diffuse =  light.diffuse * diff * vec3(texture(material.diffuse, TexCoords))+vec3(texture(material.emit, TexCoords))*material.glowIntensity;
	// specular
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflector = reflect(-lightDir, norm);
	float glossiness = pow(2,texture(material.shininess, TexCoords).r*10);
	float spec = pow(max(dot(viewDir,reflector),0.0f),glossiness);
	vec3 specular =  light.specular * spec * vec3(texture(material.specular, TexCoords));


	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result,1.0f);

}
