#version 460 core 

layout(location = 0) out vec4 FragColor;

in vec4 color;

void main() 
{
    FragColor = color;
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}