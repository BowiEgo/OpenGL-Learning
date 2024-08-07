#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec3 u_Color;

in vec3 v_Normal;
in vec2 v_TexCoords;

void main()
{
    FragColor = vec4(u_Color, 1.0);

    BrightColor = vec4(vec3(0.0), 1.0);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
    {
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
}