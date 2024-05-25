#version 460 core

#define M_PI 3.1415926535897932384626433832795

layout(location = 0) out vec4 FragColor;

in vec2 SquarePos;

uniform vec3 colorDiffuse;
uniform vec3 colorSpecular;

const float c_r = 0.35;
const float c_R1 = 0.60;
const float c_R2 = 0.90;
const float c_dR1 = 0.15;
const float c_dR2 = 0.10;
const float c_N = 14;     // Number of segments
const float c_earlySat = 2.0f;

float genCircle(float R, float dR, float l)
{
    float t = 0.0f;
    t += (1 - smoothstep(R, R + dR, l)) * step(R, l);
    t += smoothstep(R - dR, R, l) * step(l, R);
    return t;
}

float genAlpha(float A, float dA, float alpha)
{
    float earlySaturate = dA / c_earlySat;  // must be between [0.0f, dA)
    float t = 0.0f;
    t += (1 - smoothstep(A + earlySaturate, A + dA, alpha)) * step(A, alpha);
    t += smoothstep(A - dA, A - earlySaturate, alpha) * step(alpha, A);
    return t;    
}

vec4 lightColorer()
{
    vec4 diff = vec4(colorDiffuse, 1.0f);
    vec4 spec = vec4(colorSpecular, 1.0f);
    vec4 color = vec4(0.0f);
    float l = length(SquarePos);

    // central point color
    float ts = 1 - smoothstep(0, c_r, l);
    color += ts * spec;

    float td1 = genCircle(c_R1, c_dR1, l); // inner circle color
    float td2 = genCircle(c_R2, c_dR2, l); // outer circle color

    float da = (2.0f * M_PI) / c_N;   // segment's angle
    float a = atan(-SquarePos.x, -SquarePos.y); // angle from Up Y axis to Right X axis
    a += M_PI;  // from [-PI, +PI] to [0, 2 * PI]
    a = mod(a + da / 2, 2 * M_PI); // Rotate d_alpha/2 left

    float b = da / 2.0f;

    float begin = floor(a / da);
    float beginR = floor((a + b) / da);
    float beginL = floor((a - b) / da);
    
    bool isEven = mod(begin, 2.0f) == 0.0f; 
    bool isEvenR = mod(beginR, 2.0f) == 0.0f;
    bool isEvenL = mod(beginL, 2.0f) == 0.0f;
    
    float end = ceil(a / da); 
    float center_a = ((begin + end) * da) / 2.0f;
    
    float ta1 = isEven  ? genAlpha(center_a, da / 2.0f + b, a) : 
                isEvenR ? genAlpha(center_a + da, da / 2.0f + b, a) : 
                isEvenL ? genAlpha(center_a - da, da / 2.0f + b, a) : 0.0f;

    color += ta1 * td1 * diff;
    color += td2 * diff;

    return color;
}

void main()
{
    FragColor = lightColorer();
}