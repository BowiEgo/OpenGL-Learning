#version 330 core
out vec4 FragColor;

uniform vec3 u_Color;

in vec3 v_Normal;
in vec2 v_TexCoords;

void main()
{
    FragColor = vec4(u_Color, 1.0);
}