#version 330 core
layout (location = 0) out vec4 g_Position;
layout (location = 1) out vec4 g_Normal;
layout (location = 2) out vec4 g_AlbedoSpec;

in vec2 v_TexCoords;
in vec3 v_FragPosition;
in vec3 v_Normal;

uniform sampler2D u_Texture_Diffuse1;
uniform sampler2D u_Texture_Specular1;

void main()
{    
    g_Position = vec4(v_FragPosition, 1.0);
    g_Normal = vec4(normalize(v_Normal), 1.0);
    g_AlbedoSpec.rgb = texture(u_Texture_Diffuse1, v_TexCoords).rgb;
    g_AlbedoSpec.a = texture(u_Texture_Specular1, v_TexCoords).r;
}