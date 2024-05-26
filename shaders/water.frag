#version 460 core 

#define MAX_NUM_OF_LIGHTS 5

// =========================================
in FS_IN
{
    vec3 worldPos;
    vec2 texCoords;
} i;

layout(location = 0) out vec4 FragColor;
// =========================================

struct Light
{
    vec3 position;
    vec3 diffuseColor;
    vec3 specularColor;
};

struct Material
{
    vec3 ka, kd, ks;
    float shininess;
};

uniform int numberOfLights;
uniform vec3 ambientColor;
uniform Light light[MAX_NUM_OF_LIGHTS];
uniform Material material;

uniform vec3 cameraPos;
uniform vec3 objectColor;

layout(binding = 0) uniform sampler2D normalTex;

vec3 Phong(vec3 worldPos, vec3 norm, vec3 view)
{
    vec3 N = normalize(norm);
    vec3 V = normalize(view);

    // ambient 
    vec3 intensity = material.ka * ambientColor;

    for (int i = 0; i < min(numberOfLights, MAX_NUM_OF_LIGHTS); i++) {
        // diffuse 
        vec3 L = normalize(light[i].position - worldPos);
        intensity += material.kd * light[i].diffuseColor * max(dot(N, L), 0.0f);

        // specular
        vec3 R = reflect(-L, N);
        intensity += material.ks * light[i].specularColor * pow(max(dot(R, V), 0.0f), material.shininess);
    }

    return intensity;
}



void main()
{   
    vec3 norm = texture(normalTex, i.texCoords).xyz;
    vec3 view = cameraPos - i.worldPos;

    vec3 intensity = Phong(i.worldPos, norm, view);
    FragColor = vec4(intensity * objectColor, 1.0f);
    // FragColor = vec4(texture(normalTex, i.texCoords).xyz, 1.0f);
    // FragColor = vec4(texture(normalTex, i.texCoords).xyz, 1.0f);
}