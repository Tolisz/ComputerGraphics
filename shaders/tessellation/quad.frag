#version 460 core 

// --------------------------------------------
layout(location = 0) out vec4 FragColor;

in FS_IN
{
    vec3 worldPos;
    vec3 norm;
} i;
// --------------------------------------------

struct Material
{
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shininess;
};

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
};

uniform Material mat;
uniform Light lgt;

uniform vec3 viewPos; 
uniform bool UsePhong;

vec3 PhongIllumination(vec3 worldPos, vec3 N, vec3 V)
{
    vec3 color = mat.ka * lgt.ambient;

    // diffuse 
    vec3 L = normalize(lgt.position - worldPos);
    color += mat.kd * lgt.diffuse * max(dot(N, L), 0.0f);

    // specular
    vec3 R = reflect(-L, N);
    color += mat.ks * lgt.specular * pow(max(dot(R, V), 0.0f), mat.shininess);

    return color;
} 

void main() 
{
    if (!UsePhong) {
        FragColor = vec4(lgt.diffuse, 1.0f);
        return;
    }

    vec3 N = normalize(i.norm);
    vec3 V = normalize(viewPos - i.worldPos);

    vec3 color = PhongIllumination(i.worldPos, N, V);
    FragColor = vec4(color, 1.0f);
}