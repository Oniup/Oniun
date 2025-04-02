#version 450

layout (location = 0) in vec3 a_Pos;

void main()
{
    gl_Position = vec4(a_Pos.xyz, 1.0);
}
