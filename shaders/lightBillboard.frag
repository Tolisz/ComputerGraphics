#version 460 core

layout(location = 0) out vec4 FragColor;

in vec2 SquarePos;

uniform vec3 colorDiffuse;
uniform vec3 colorSpecular;

const float r = 0.25;
const float R1 = 0.70;
const float R2 = 0.90;
const float dR = 0.1;

vec4 lightColorer()
{
    vec4 diff = vec4(colorDiffuse, 1.0f);
    vec4 spec = vec4(colorSpecular, 1.0f);
    vec4 color = vec4(0.0f);
    float l = length(SquarePos);

    float ts = 1 - smoothstep(0, r, l);
    color += ts * spec;

    float td1 = 0.0f;
    td1 += (1 - smoothstep(R1, R1 + dR, l)) * step(R1, l);
    td1 += smoothstep(R1 - dR, R1, l) * step(l, R1);
    color += td2 * diff;

    float td2 = 0.0f;
    td2 += (1 - smoothstep(R2, R2 + dR, l)) * step(R2, l);
    td2 += smoothstep(R2 - dR, R2, l) * step(l, R2);
    color += td1 * diff;


    return color;
}

void main()
{
    FragColor = lightColorer();
    //FragColor = vec4(colorDiffuse + colorSpecular, 1.0f);
}