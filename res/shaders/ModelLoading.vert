#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 v_FragPosition;
out vec3 v_Normal;
out vec2 v_TexCoords;

void main()
{
    vec4 modelPosition = modelMatrix * vec4(a_Position, 1.0);
    vec4 viewPosition = viewMatrix * modelPosition;
    vec4 projectionPosition = projectionMatrix * viewPosition;

    gl_Position = projectionPosition;
    v_FragPosition = vec3(modelPosition);
    v_Normal = a_Normal;
    v_TexCoords = a_TexCoords;
}