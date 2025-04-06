#version 450

layout (location = 0) in vec3 a_Pos;
layout (location = 0) in vec2 a_UV;

out vec2 UV;

void main()
{
    gl_Position = vec4(a_Pos.xyz, 1.0);
    UV = a_UV;
}
