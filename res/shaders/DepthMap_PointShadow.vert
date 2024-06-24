#version 330 core
layout (location = 0) in vec3 a_Position;

uniform mat4 modelMatrix;

void main()
{
    gl_Position = modelMatrix * vec4(a_Position, 1.0);
}