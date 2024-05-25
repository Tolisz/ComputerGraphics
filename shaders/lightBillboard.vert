#version 460 core 

layout(location = 0) in vec3 in_Pos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(in_Pos, 1.0f);
}