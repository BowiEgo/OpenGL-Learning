#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
    gl_Position = vec4(a_Position.x, -a_Position.y, 0.0, 1.0);
    v_TexCoords = a_TexCoords;
}