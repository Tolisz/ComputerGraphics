#version 460 core 

layout(lines_adjacency) in;
layout(line_strip, max_vertices = 256) out;

layout(std140, binding = 0) uniform MatricesBlock {
    mat4 view;
    mat4 projection;
};

void main() 
{
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;

    gl_Position = p0 * view * projection;
    EmitVertex();

    gl_Position = p1 * view * projection;
    EmitVertex();

    gl_Position = p2 * view * projection;
    EmitVertex();

    gl_Position = p3 * view * projection;
    EmitVertex();

    EndPrimitive();
}