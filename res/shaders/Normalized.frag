#version 330 core
out vec4 FragColor;
  
in vec2 v_TexCoords;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct TexturedMaterial {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emmision;
    float shininess;
};

#define NORMALIZED_TYPE_INVERSION 0
#define NORMALIZED_TYPE_GREYSCALE 1
#define NORMALIZED_TYPE_SHARPEN 2
#define NORMALIZED_TYPE_BLUR 3

uniform TexturedMaterial u_Material;
uniform int u_Normalized_Type;

const float offset = 1.0 / 300.0;

vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), vec2( 0.0f,  offset), vec2( offset,  offset),
    vec2(-offset,    0.0f), vec2( 0.0f,    0.0f), vec2( offset,    0.0f),
    vec2(-offset, -offset), vec2( 0.0f, -offset), vec2( offset, -offset)
);

float sharpenKernel[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

float blurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
);

vec3 sampleTex[9];

void main()
{
    vec3 final = vec3(0.0);
    vec3 screenTexture = vec3(texture(u_Material.diffuse, v_TexCoords));

    if (u_Normalized_Type == NORMALIZED_TYPE_INVERSION)
    {
        vec3 inversion = 1.0 - screenTexture;
        final = inversion;
    }
    else if (u_Normalized_Type == NORMALIZED_TYPE_GREYSCALE)
    {
        vec3 average = vec3(screenTexture.r * 0.2126 + screenTexture.g * 0.7152 + screenTexture.b * 0.0722);
        final = average;
    }
    else if (u_Normalized_Type == NORMALIZED_TYPE_SHARPEN)
    {
        for (int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(u_Material.diffuse, v_TexCoords.st + offsets[i]));
        }
        vec3 col = vec3(0.0);
        for (int i = 0; i < 9; i++)
            col += sampleTex[i] * sharpenKernel[i];

        final = col;
    }
    else if (u_Normalized_Type == NORMALIZED_TYPE_BLUR)
    {
        for (int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(u_Material.diffuse, v_TexCoords.st + offsets[i]));
        }
        vec3 col = vec3(0.0);
        for (int i = 0; i < 9; i++)
            col += sampleTex[i] * blurKernel[i];

        final = col;
    }

    FragColor = vec4(final, 1.0);
}