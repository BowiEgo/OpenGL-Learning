#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

#define NORMALIZED_TYPE_INVERSION 0
#define NORMALIZED_TYPE_GREYSCALE 1
#define NORMALIZED_TYPE_SHARPEN 2
#define NORMALIZED_TYPE_BLUR 3

uniform int u_Normalized_Type;

out vec2 v_TexCoords;

void main()
{
    float scale = 0.25;
    float unit = (1 - scale) / scale;
    vec2 final_pos = vec2(a_Position.x - unit, - a_Position.y + unit) * scale;

    if (u_Normalized_Type == NORMALIZED_TYPE_GREYSCALE)
        final_pos = vec2(a_Position.x - unit, - a_Position.y + unit / 3) * scale;

    if (u_Normalized_Type == NORMALIZED_TYPE_SHARPEN)
        final_pos = vec2(a_Position.x - unit, - a_Position.y - unit / 3) * scale;

    if (u_Normalized_Type == NORMALIZED_TYPE_BLUR)
        final_pos = vec2(a_Position.x - unit, - a_Position.y - unit) * scale;

    gl_Position = vec4(final_pos, 0.0, 1.0);
    v_TexCoords = a_TexCoords;
}