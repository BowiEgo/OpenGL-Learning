#version 330 core

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 u_Color;
uniform float u_Split_CoordX;

void main()
{
    vec3 final = u_Color;

    if(gl_FragCoord.x < u_Split_CoordX)
        final *= 0.5;

    FragColor = vec4(final, 1.0);
}