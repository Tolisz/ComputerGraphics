#version 460 core

// --------------------------------------------
layout(quads, equal_spacing, ccw) in;

out vec4 color;
// --------------------------------------------

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

float B( int i, float t )
{
    const vec4 bc = vec4( 1.0f, 3.0f, 3.0f, 1.0f );

    float pow1 = (t == 0.0f && i == 0) ? 1.0f : pow( t, float(i) );
    float pow2 = (t == 1.0f && i == 3) ? 1.0f : pow( 1.0f - t, float(3 - i) );
    return bc[i] * pow1 * pow2;
}

vec4 BezierPatch(float u, float v)
{
    vec4 pos = vec4( 0.0 );
    int offset = bezierShape * 16;

    for ( int i = 0; i < 4; ++i ) {
        for ( int j = 0; j < 4; ++j ) {
            pos += B( i, u ) * B( j, v ) * p[4 * i + j + offset];
        }
    }    

    return pos;
}

void main()
{
    float u = gl_TessCoord.x;
    float omu = 1 - u;
    float v = gl_TessCoord.y;
    float omv = 1 - v;

    vec4 pos = BezierPatch(u, v);
    gl_Position = projection * view * pos;

    color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Bilinear interpolation 
    // 
    // gl_Position =projection * view * (
    //     omu * omv * gl_in[0].gl_Position +
    //     u * omv * gl_in[1].gl_Position +
    //     u * v * gl_in[2].gl_Position +
    //     omu * v * gl_in[3].gl_Position );
}