#version 330 core
out vec4 FragColor;

in vec2 v_TexCoords;

uniform float u_Exposure;

uniform sampler2D u_Texture_Diffuse1;
uniform float u_Split_CoordX;

void main()
{
    vec3 final = vec3(0.0);

    const float gamma = 2.2;
    vec3 hdrColor = texture(u_Texture_Diffuse1, v_TexCoords).rgb;
    final = hdrColor;

    if(v_TexCoords.x < u_Split_CoordX)
    {
        // Reinhard色调映射
        vec3 mapped = vec3(1.0) - exp(-hdrColor * u_Exposure);
        // Gamma校正
        mapped = pow(mapped, vec3(1.0 / gamma));
        final = mapped;
    }

    FragColor = vec4(final, 1.0);
}