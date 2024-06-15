#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vec4 pos = projectionMatrix * viewMatrix * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;

    v_TexCoords = a_Position;
}