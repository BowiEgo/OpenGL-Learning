#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 7) in mat4 a_InstanceModelMatrix;

layout (std140) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};

uniform bool u_Is_Instance;
uniform mat4 modelMatrix;
uniform mat4 u_LightSpaceMatrix;

out vec3 v_FragPosition;
out vec3 v_Normal;
out vec2 v_TexCoords;

out VS_OUT {
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

void main()
{
    vec4 modelPosition;
    if (u_Is_Instance)
    {
        modelPosition = a_InstanceModelMatrix * vec4(a_Position, 1.0);
    } else {
        modelPosition = modelMatrix * vec4(a_Position, 1.0);
    }
    vec4 viewPosition = viewMatrix * modelPosition;
    vec4 projectionPosition = projectionMatrix * viewPosition;

    gl_Position = projectionPosition;
    v_FragPosition = vec3(modelPosition);
    v_Normal = a_Normal;
    v_TexCoords = a_TexCoords;
    vs_out.FragPosition = vec3(modelPosition);
    vs_out.Normal = a_Normal;
    vs_out.TexCoords = a_TexCoords;
    vs_out.FragPosLightSpace = u_LightSpaceMatrix * vec4(vs_out.FragPosition, 1.0); // use for directional light
}