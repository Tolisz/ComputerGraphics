#version 460 core 

layout(location = 0) in vec3 iSquarePos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 billboardPos;

// billboard's size in pixels
const ivec2 billboardSize = ivec2(400, 400);
//const ivec2 billboardSize = ivec2(25, 25);
uniform ivec2 screenSize;

out vec2 SquarePos;

void main()
{
    // compute billboard's center in screen space 
    vec4 projBillboardPos = projection * view * vec4(billboardPos, 1.0f);
    projBillboardPos /= projBillboardPos.w;   

    // move billboard's center torwards the point iSquarePos
    // also take into accound the size of the billboard in screen space  
    projBillboardPos.xy += iSquarePos.xy * billboardSize / vec2(screenSize);     
    gl_Position = projBillboardPos;

    // Interpolate squarePos for fragment shader computations
    SquarePos = iSquarePos.xy;
}