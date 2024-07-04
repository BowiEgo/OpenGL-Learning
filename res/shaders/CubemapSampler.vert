#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 v_WorldPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    vec4 modelPosition = modelMatrix * vec4(a_Position, 1.0);
    gl_Position = projectionMatrix * viewMatrix * modelPosition;
    v_WorldPosition = a_Position;
}