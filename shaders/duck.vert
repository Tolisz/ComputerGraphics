#version 460 core

// ==================================
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inTexCoord;

out FS_IN
{
    vec3 norm;
    vec2 texCoords;
} o;
// ==================================

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    o.norm = inNorm;
    // o.norm = (transpose(inverse(model)) * vec4(inNorm, 0.0f)).xyz; 
    o.texCoords = inTexCoord;
    gl_Position = projection * view * model * vec4(inPos, 1.0f);
}