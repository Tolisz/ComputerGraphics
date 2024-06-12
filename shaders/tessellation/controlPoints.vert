#version 460 core 

layout(std140, binding = 0) uniform Matrices 
{
    mat4 view;
    mat4 projection;
};

layout(std140, binding = 1) uniform ControlPoints
{
    // 0 - flat, 1 - up, 2 - down
    vec4 p[3 * 16];
};

uniform int bezierShape;

void main() 
{
    gl_Position = projection * view * p[gl_VertexID + 16 * bezierShape];
}