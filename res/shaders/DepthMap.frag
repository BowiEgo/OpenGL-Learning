#version 330 core
out vec4 FragColor;
  
in vec2 v_TexCoords;

uniform vec3 u_CameraPosition;

uniform sampler2D u_Texture_Diffuse1;

void main()
{
    vec3 final = vec3(0.0);
    vec3 screenTexture = vec3(texture(u_Texture_Diffuse1, v_TexCoords));

    final = screenTexture;

    FragColor = vec4(final, 1.0);
}