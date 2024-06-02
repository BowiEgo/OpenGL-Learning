#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform float u_Mix;

void main()
{
    vec4 texColor = texture(u_Texture1, v_TexCoord);
//    FragColor = texColor * vec4(v_Color, 1.0);
    FragColor = mix(texture(u_Texture1, v_TexCoord),
        texture(u_Texture2, vec2(1.0 - v_TexCoord.x, v_TexCoord.y)), u_Mix);

}