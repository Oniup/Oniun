#version 450

out vec4 FragColor;
in vec2 UV;

uniform sampler2D Diffuse;

void main()
{
    FragColor = texture(Diffuse, UV);
}
