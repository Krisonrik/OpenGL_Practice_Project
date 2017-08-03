#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 0.1f;
float far = 100.0f;

float LinearizeDepth(float depth){
    float z = depth * 2.0f - 1.0f; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
 //FragColor = vec4(vec3(texture(texture1, TexCoords))*1/(1-vec3(gl_FragCoord.z)*.5f),1.0f);
    float depth = LinearizeDepth(gl_FragCoord.z)/far; // divide by far far demonstration
    // FragColor = vec4(vec3(depth),1.0f);
     FragColor = vec4(vec3(texture(texture1, TexCoords)),gl_FragCoord.z);
}
