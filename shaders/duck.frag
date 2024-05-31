#version 460 core

#define MAX_NUM_OF_LIGHTS 5

// =========================================
in FS_IN
{
    vec3 worldPos;
    vec3 norm;
    vec2 texCoords;
} i;

layout(location = 0) out vec4 FragColor;
// =========================================

struct Light
{
    vec4 position;
    vec4 diffuseColor;
    vec4 specularColor;
};

struct Material
{
    vec3 ka, kd, ks;
    float shininess;
};

layout(std140, binding = 1) uniform LightsBlock {
    vec4 ambientColor;
    Light light[MAX_NUM_OF_LIGHTS];
};
uniform int numberOfLights;

uniform Material material;
uniform vec3 cameraPos;

layout(binding = 0) uniform sampler2D duckTex; 

vec3 Phong(vec3 worldPos, vec3 norm, vec3 view)
{
    vec3 N = normalize(norm);
    vec3 V = normalize(view);

    // ambient 
    vec3 intensity = material.ka * ambientColor.rgb;

    for (int i = 0; i < min(numberOfLights, MAX_NUM_OF_LIGHTS); i++) {
        // diffuse 
        vec3 L = normalize(light[i].position.xyz - worldPos);
        intensity += material.kd * light[i].diffuseColor.rgb * max(dot(N, L), 0.0f);

        // specular
        vec3 R = reflect(-L, N);
        intensity += material.ks * light[i].specularColor.rgb * pow(max(dot(R, V), 0.0f), material.shininess);
    }

    return intensity;
}

void main()
{
    vec3 norm = normalize(i.norm);
    vec3 view = normalize(cameraPos - i.worldPos);
    vec3 duckColor = texture(duckTex, i.texCoords).xyz;

    // Compute Phong's color of duck 
    // =======================================================
    vec3 intensity = Phong(i.worldPos, norm, view);
    vec3 color = intensity * duckColor;

    FragColor = vec4(color, 1.0f);
}