#version 460 core

layout(vertices = 4) out;

uniform vec4 outerLevel;
uniform vec2 innerLevel;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelOuter[0] = outerLevel.x;
    gl_TessLevelOuter[1] = outerLevel.y;
    gl_TessLevelOuter[2] = outerLevel.z;
    gl_TessLevelOuter[3] = outerLevel.w;
    
    gl_TessLevelInner[0] = innerLevel.x;
    gl_TessLevelInner[1] = innerLevel.y;
}