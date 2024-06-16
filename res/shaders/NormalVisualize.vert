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

out VS_OUT {
    vec4 Normal;
} vs_out;

void main()
{
    vec4 modelPosition = modelMatrix * vec4(a_Position, 1.0);
    vec4 viewPosition = viewMatrix * modelPosition;
    vec4 projectionPosition = projectionMatrix * viewPosition;

    gl_Position = projectionPosition;
    mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    vs_out.Normal = projectionMatrix * vec4(normalMatrix * a_Normal, 0.0);

    // gl_Position = vec4(a_Position, 1.0);
    // vs_out.Normal = a_Normal;
}