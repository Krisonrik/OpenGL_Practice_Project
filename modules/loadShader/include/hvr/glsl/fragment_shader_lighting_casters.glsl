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
	vec3 direction;

	vec3 ambient;
	vec3 specular;
	vec3 diffuse;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

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
	float distant = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distant + light.quadratic * (distant * distant));
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0f, 1.0f);

	// if(theta > light.cutOff){
		// ambient
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
		ambient *= intensity;
		// diffuse

		float diff = max(dot(norm, lightDir),0.0f);
		vec3 diffuse =  light.diffuse * diff * vec3(texture(material.diffuse, TexCoords))+vec3(texture(material.emit, TexCoords))*material.glowIntensity;
		diffuse *= intensity;
		// specular
		vec3 viewDir = normalize(viewPos-FragPos);
		vec3 reflector = reflect(-lightDir, norm);
		float glossiness = pow(2,texture(material.shininess, TexCoords).r*10);
		float spec = pow(max(dot(viewDir,reflector),0.0f),glossiness);
		vec3 specular =  light.specular * spec * vec3(texture(material.specular, TexCoords));
		specular *= intensity;

		vec3 result = ambient + diffuse + specular;
	    FragColor = vec4(result,1.0f);

	// }
	// else {
	// 	FragColor = vec4(light.ambient * vec3(texture(material.diffuse,TexCoords)),1.0f);
	// }

}
