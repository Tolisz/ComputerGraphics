#version 460 core 

layout(location = 0) in vec3 inPos;

layout(std140, binding = 0) uniform Matrices 
{
    mat4 view;
    mat4 projection;
};

void main()
{
    //gl_Position = vec4(inPos / 2.0f, 1.0f);
    gl_Position = projection * view * vec4(inPos, 1.0f);
}