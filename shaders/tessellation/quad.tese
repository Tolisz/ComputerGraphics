#version 460 core

layout(quads, equal_spacing, ccw) in;

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

out vec4 color;

float B( int i, float u )
{
    const vec4 bc = vec4( 1, 3, 3, 1 );
    return bc[i] * pow( u, i ) * pow( 1.0 - u, 3 - i ); 
}

// vec4 BezierPatch(float u, float v)
// {
//     vec4 point = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//     int offset = bezierShape * 16;

//     for (int i = 0; i < 4; ++i) {
//         for (int j = 0; j < 4; ++j) {
//             point.xyz += B(j, u) * B(i, v) * p[4 * i + j + offset].xyz;
//         }
//     }

//     return point;
// }

void main()
{
    float u = gl_TessCoord.x;
    float omu = 1 - u;
    float v = gl_TessCoord.y;
    float omv = 1 - v;

    // gl_Position = projection * view * BezierPatch(u, v); 

    vec4 pos = vec4( 0.0 );
    int offset = bezierShape * 16;

    for ( int j = 0; j < 4; ++j ) {
        for ( int i = 0; i < 4; ++i ) {
            pos += B( i, u ) * B( j, v ) * p[4 * i + j + offset];
        }
    }

    gl_Position = projection * view * pos;
    color = vec4(gl_TessCoord.xy, 0.0f, 1.0f);
    // gl_Position =projection * view * (
    //     omu * omv * gl_in[0].gl_Position +
    //     u * omv * gl_in[1].gl_Position +
    //     u * v * gl_in[2].gl_Position +
    //     omu * v * gl_in[3].gl_Position );
}