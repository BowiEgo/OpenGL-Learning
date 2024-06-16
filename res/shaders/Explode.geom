#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoords;
} gs_in[];

out vec3 v_FragPosition;
out vec3 v_Normal;
out vec2 v_TexCoords;

uniform float u_ExplodeProgress;
uniform float u_Time;

vec4 Explode(vec4 position, vec3 normal)
{
    float magnitude = 1.0;
    vec3 direction = normal * magnitude * u_ExplodeProgress;
    // vec3 direction = normal * magnitude * (sin(u_Time) + 1.0) / 2.0;
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[1].gl_Position - gl_in[2].gl_Position);
    return normalize(cross(b, a));
}

void main()
{
    vec3 normal = GetNormal();

    gl_Position = Explode(gl_in[0].gl_Position, normal);
    v_FragPosition = gs_in[0].FragPosition;
    v_Normal = gs_in[0].Normal;
    v_TexCoords = gs_in[0].TexCoords;
    EmitVertex();

    gl_Position = Explode(gl_in[1].gl_Position, normal);
    v_FragPosition = gs_in[1].FragPosition;
    v_Normal = gs_in[1].Normal;
    v_TexCoords = gs_in[1].TexCoords;
    EmitVertex();

    gl_Position = Explode(gl_in[2].gl_Position, normal);
    v_FragPosition = gs_in[2].FragPosition;
    v_Normal = gs_in[2].Normal;
    v_TexCoords = gs_in[2].TexCoords;
    EmitVertex();

    EndPrimitive();
}
