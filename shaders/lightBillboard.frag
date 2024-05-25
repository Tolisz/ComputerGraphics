#version 460 core

layout(location = 0) out vec4 FragColor;

uniform vec3 colorDiffuse;
uniform vec3 colorSpecular;


void main()
{
    FragColor = vec4(colorDiffuse + colorSpecular, 1.0f);
}