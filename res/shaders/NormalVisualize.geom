#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

layout (std140) uniform Matrices
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
};
uniform mat4 modelMatrix;

in VS_OUT {
    vec3 Normal;
} vs_in[];

const float MAGNITUDE = 0.05;

void GenerateLine(int index, mat3 normalMatrix)
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * gl_in[index].gl_Position;
    EmitVertex();

    vec4 ptOnNormal = viewMatrix * modelMatrix * gl_in[index].gl_Position
        + vec4(normalize(normalMatrix * vs_in[index].Normal) * MAGNITUDE, 0.0);

    gl_Position = projectionMatrix * ptOnNormal;
    EmitVertex();

    EndPrimitive();
}

void main()
{
    mat3 nMat = mat3(transpose(inverse(viewMatrix * modelMatrix)));

    GenerateLine(0, nMat);
    GenerateLine(1, nMat);
    GenerateLine(2, nMat);
}
