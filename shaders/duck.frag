#version 460 core

// =========================================
in FS_IN
{
    vec3 norm;
    vec2 texCoords;
} i;

layout(location = 0) out vec4 FragColor;
// =========================================

layout(binding = 0) uniform sampler2D duckTex; 

void main()
{
    //FragColor = vec4(i.norm, 1.0f);
    vec3 duckColor = texture(duckTex, i.texCoords).xyz;

    FragColor = vec4(duckColor, 1.0f);
}