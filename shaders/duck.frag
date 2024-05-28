#version 460 core

// =========================================
in FS_IN
{
    vec2 texCoords;
} i;

layout(location = 0) out vec4 FragColor;
// =========================================


void main()
{
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}