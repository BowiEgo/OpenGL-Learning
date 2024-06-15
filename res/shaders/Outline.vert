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

uniform vec3 u_CameraPosition;

uniform float u_OutlineWidth;
uniform int u_OutlineDrawType;

out vec3 v_FragPosition;
out vec3 v_Normal;
out vec2 v_TexCoords;

void main()
{
    vec4 worldPosition = modelMatrix * vec4(a_Position, 1.0);
    float camDist = length(u_CameraPosition - vec3(worldPosition));

    vec4 modelPosition;
    if (u_OutlineDrawType == 0)
    {
        mat4 scaleMatrix = mat4(1.0f);
        float scaleN = 1.0 + u_OutlineWidth * camDist * 0.01;
        scaleMatrix[0][0] = scaleN;
        scaleMatrix[1][1] = scaleN;
        scaleMatrix[2][2] = scaleN;

        modelPosition = modelMatrix * scaleMatrix * vec4(a_Position, 1.0);
    }
    else if (u_OutlineDrawType == 1)
        modelPosition = modelMatrix * vec4(a_Position + a_Normal * u_OutlineWidth * camDist * 0.035, 1.0);

    vec4 viewPosition = viewMatrix * modelPosition;
    vec4 projectionPosition = projectionMatrix * viewPosition;

    gl_Position = projectionPosition;

    v_FragPosition = vec3(modelPosition);
    v_Normal = a_Normal;
    v_TexCoords = a_TexCoords;
}