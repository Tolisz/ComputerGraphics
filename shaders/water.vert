#version 460 core 

layout(location = 0) in vec4 inPos;

out FS_IN
{
    vec3 worldPos;
} o;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    o.worldPos = vec3(inPos);
    gl_Position = projection * view * inPos;
}