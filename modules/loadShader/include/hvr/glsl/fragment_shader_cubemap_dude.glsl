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

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  


struct SpotLight {
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

#define NR_POINT_LIGHTS 4 

uniform Material material;
uniform DirLight dirLight;
 
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

in vec2 TexCoords;
in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 Position;

uniform vec3 objectColor;
uniform vec3 lightColor;
// uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 cameraPos;
uniform samplerCube skybox;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 lightDir); 


void main()
{
	// vec3 norm = normalize(Normal);
	// vec3 viewDir = normalize(viewPos - FragPos);


	// vec3 result = CalcDirLight(dirLight, norm, viewDir);

	// for(int i = 0; i< NR_POINT_LIGHTS; i++){
	// 	result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	// }

	// result += CalcSpotLight(spotLight,norm,FragPos,viewDir);
    // FragColor = vec4(vec3(0.5f),1.0f);
    // FragColor = vec4(result,1.0f);


    // reflaction
    // vec3 vecI = normalize(Position - cameraPos);
    // vec3 vecR = reflect(vecI, normalize(Normal));


    // refraction
    float ratio = 1.00/1.52;
    vec3 vecI = normalize(Position - cameraPos);
    vec3 vecR = refract(vecI, normalize(Normal), ratio);
    FragColor = vec4(vec3(texture(material.texture_diffuse, TexCoords))*0.2+vec3(texture(material.texture_specular, TexCoords))*texture(skybox, vecR).rgb*0.8,1.0f);

    // FragColor = vec4(vec3(texture(material.texture_diffuse, TexCoords)),1.0f);

    // FragColor = vec4(vec3(texture(material.texture_specular, TexCoords)),1.0f);

}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
   	float glossiness = pow(2,material.shininess);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), glossiness);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float glossiness = pow(2,material.shininess);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), glossiness);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 lightDir) {
	float distant = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distant + light.quadratic * (distant * distant));
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0f, 1.0f);

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));
	ambient *= intensity;
	// diffuse

	float diff = max(dot(normal, lightDir),0.0f);
	vec3 diffuse =  light.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords))+vec3(material.emit)*material.glowIntensity;
	diffuse *= intensity;
	// specular
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflector = reflect(-lightDir, normal);
    float glossiness = pow(2,material.shininess);

	float spec = pow(max(dot(viewDir,reflector),0.0f),glossiness);
	vec3 specular =  light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
	specular *= intensity;

	return ambient + diffuse + specular;
} 