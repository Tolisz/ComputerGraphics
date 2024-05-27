#version 460 core

layout(location = 0) in vec3 iPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(iPos, 1.0f);    
}