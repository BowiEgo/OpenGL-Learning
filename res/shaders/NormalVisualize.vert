#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out VS_OUT {
    vec3 Normal;
} vs_out;

void main()
{
    gl_Position = vec4(a_Position, 1.0);
    vs_out.Normal = a_Normal;
}