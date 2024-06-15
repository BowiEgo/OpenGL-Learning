#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

layout (std140) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

uniform mat4 modelMatrix;

out VS_OUT
{
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    vec4 modelPosition = modelMatrix * vec4(a_Position, 1.0);
    vec4 viewPosition = viewMatrix * modelPosition;
    vec4 projectionPosition = projectionMatrix * viewPosition;

    gl_Position = projectionPosition;
    vs_out.FragPosition = vec3(modelPosition);
    vs_out.Normal = a_Normal;
    vs_out.TexCoords = a_TexCoords;
}