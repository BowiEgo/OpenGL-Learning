#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_Color;
out vec2 v_TexCoord;

uniform mat4 u_MVP; // MVP means model-view-projection matrix

void main()
{
    gl_Position = u_MVP * a_Position;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
}

#shader fragment
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
//    color = texColor * vec4(v_Color, 1.0);
    FragColor = mix(texture(u_Texture1, v_TexCoord),
        texture(u_Texture2, vec2(1.0 - v_TexCoord.x, v_TexCoord.y)), u_Mix);
}