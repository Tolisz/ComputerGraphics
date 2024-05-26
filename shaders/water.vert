#version 460 core 

// ==================================
layout(location = 0) in vec4 inPos;

out FS_IN
{
    vec3 worldPos;
    vec2 texCoords;
} o;
// ==================================

uniform mat4 view;
uniform mat4 projection;

uniform float a;

void main()
{
    o.worldPos = vec3(inPos);
    o.texCoords = (inPos.xz + a / 2.0f) / a;
    gl_Position = projection * view * inPos;
}